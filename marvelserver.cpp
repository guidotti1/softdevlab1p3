
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

        //determines what outmessage is based upon whether we are searching for all matches or looking at individual entry
        if (charNumInt == 0)
        {
            characters = ourMap.userSearch(ctype, data);
            outMessage = ourMap.readMatches(characters);
        }
        else
        {
            dataEntry charSelection = characters[charNumInt-1];
            outMessage = charSelection.returnData();
        }
        bool end = false;
        stringstream s2(outMessage);
        string outWord;
        sendfifo.openwrite(); 
        //while loop sends all of the outMessage line by line. ? is placed as delimiter for each line
        while (end == false)
        {
            //the lines in the data are separated by ? as a delimiter. this way the server sends message line by line.
            getline(s2, outWord, '?');
            if (outWord == "$END")
            {
                //end of message marked by $END
                sendfifo.send(outWord);
                cout << "last outWord!! :: " << outWord << endl;
                sendfifo.fifoclose();
                recfifo.fifoclose();
                end = true;
                break;
            }
            sendfifo.send(outWord);
            //we send one line at a time across the fifo
            cout << "sending outWord :: " << outWord << endl;
        }
        //results in their entirety 
        cout << " results :: " << outMessage << endl;
        }
}
