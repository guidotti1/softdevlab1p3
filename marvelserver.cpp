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
    //build maps. characters stores matching characters for input data
    string filename = "input.txt";
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
        stringstream s (inMessage);
        string word;
        getline(s,word, '%');
        type = word;
        char ctype = type[0];
        getline(s, word, '%');
        data = word;
        getline(s, word, '%');
        charNum = word;

        characters = ourMap.userSearch(ctype, data);
        if (charNumInt == 0)
            {
            string output = ourMap.readMatches(characters);
            sendfifo.openwrite();
            sendfifo.send(output);
            sendfifo.fifoclose();
            recfifo.fifoclose();
            }
       // else
        //    {
        //    cout<<characters[charNumInt-1];
        //    }



        }

}
