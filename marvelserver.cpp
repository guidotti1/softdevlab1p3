#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "extraFunctions.h"
#include "dataEntry.h"
#include "dataLookup.h"
#include "fifo.h"

using namespace std;

string receive_fifo = "Marvelrequest";
string send_fifo = "Marvelreply";

int main()
{
    string inMessage, outMessage, data, type, charNum;
    int charNumInt;
    //build maps. characters vector stores matching characters for input data
    string filename = "/home/class/SoftDev/marvel/marvel-wikia-data.csv";
    dataLookup ourMap(filename);
    ourMap.createYearAndNameMaps();
    vector<dataEntry> characters;

    //Fifo for communication
    Fifo recfifo(receive_fifo);
    Fifo sendfifo(send_fifo);

    while(1)
        {
        recfifo.openread();
        inMessage = recfifo.recv();
        // parse incoming message
        // form : type%data%charnumber%
        //we getline with % as delimiter to get each of the three pieces of information
        stringstream s (inMessage);
        string word;
        getline(s,word, '%');
        type = word;
        char ctype = type[0];
        getline(s, word, '%');
        data = word;
        getline(s, word, '%');
        charNum = word;
        istringstream iss(charNum);
        iss >> charNumInt;
        //message output so that we can tell what is received
        cout << "Message: " << inMessage << endl;
        //finds the matching characters in the maps based upon the type of search and the data received for said search. 
        characters = ourMap.userSearch(ctype, data);
        //this part of the search identifies all matches for input data. charNumInt == 0 means that we are ONLY identifying the matches
        if (charNumInt == 0)
            {
            //reads the vector of characters so that it can be parsed by javascript and displayed on website
            outMessage = ourMap.readMatches(characters);
            bool end = false;
            stringstream s2(outMessage);
            string outWord;
            //while loop sends all of the outMessage line by line. ? is placed as delimiter for each line
            while (end == false)
            {
                sendfifo.openwrite(); 
                //the lines in the data are separated by ? as a delimiter. this way the server can go line by line.
                getline(s2, outWord, '?');
                if (outWord == "$END")
                {
                    //end of message marked by $END
                    sendfifo.send(outWord);
                    sendfifo.fifoclose();
                    cout << "last outWord!! :: " << outWord << endl;
                    end = true;
                    break;
                }
                sendfifo.send(outWord);
                sendfifo.fifoclose();
                //we send one line at a time across the fifo
                cout << "sending outWord :: " << outWord << endl;
            }
            recfifo.fifoclose();
            //results in their entirety 
            cout << " Results: (charNumInt = 0) " << outMessage << endl;
            }
        else
            {
            //this part is used to allow the user to select an individual character and view their data
            //find the character selection and read their nonempty data values. 
            cout << "in charNumInt ne 0"<<endl;
            dataEntry charSelection = characters[charNumInt-1];
            outMessage = charSelection.returnData();
            sendfifo.openwrite();
            sendfifo.send(outMessage);
            sendfifo.fifoclose();
            recfifo.fifoclose();
            cout << " Results: (charNumInt != 1) " << outMessage << endl;
            }
        }

}
