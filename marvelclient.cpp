#include <iostream>
//ajax
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
//pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"
#include "extraFunctions.h"

using namespace std;
using namespace cgicc;
//values returned by results_select??
#define ONE "OneWord"
#define YEAR "Year"
#define MULTI "MultiWord"

string receive_fifo = "Marvelreply";
string send_fifo = "Marvelrequest";

int main()
{
    //cgi object
    Cgicc cgi;
    //main
    //gets info from web page
    form_iterator itts = cgi.getElement("type_select");
    form_iterator itname = cgi.getElement("name");
    form_iterator itcharNumber = cgi.getElement("charNumber");

    //fifos for communication
    Fifo recfifo(receive_fifo);
    Fifo sendfifo(send_fifo);

    string sendType;
    string userType = **itts;
    char cType = userType[0];
    if (cType == '1')
        {
        sendType = "1%";
        }
    else if (cType == '2')
        {
        sendType = "2%";
        }
    else if (cType == '3')
        {
        sendType = "3%";
        }

    string sendData = **itname;
    sendData += "%";
    string sendNumber = **itcharNumber;
    sendNumber += "%";
    toUpperCase(sendData);
    string message = sendType + sendData + sendNumber;
    sendfifo.openwrite();
    sendfifo.send(message);
    //send message to server  to get results

    //get message from server

    //if (sendNumber == "0%")
    //{


    
    cout << "Content-type: text/plain\n\n";
    bool end = false;
    while (end == false)
    {
        recfifo.openread();
        string results = recfifo.recv();
            if (results == "$END")
            {
                recfifo.fifoclose();
                sendfifo.fifoclose();
                end = true;
                break;
            }
        cout << results << endl;
        recfifo.fifoclose();
    }
    //}
    //else 
    //{
    //    recfifo.openread();
    //    sendfifo.fifoclose();
    //    string results = recfifo.recv();
    //    cout << results << endl;
    //   recfifo.fifoclose();
    //}        
    return 0;
}
