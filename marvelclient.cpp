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
    
    //send message to server  
    //message will have the form TYPE%DATA%NUMBER% for server to parse
    string message = sendType + sendData + sendNumber;
    sendfifo.openwrite();
    sendfifo.send(message);
    
    //receive feedback from server
    //$END is placed at the end of the message to tell when the message terminates
    //feedback comes line by line, is separated by commas for the javascript to parse and place into a table. 
    cout << "Content-type: text/plain\n\n";
    string reply="";
    recfifo.openread();
    bool end = false;
    while (end == false)
    {
    	reply = recfifo.recv();
	if (reply == "$END")
	{
		end = true;
		break;
	}
		cout << reply;
	}
    recfifo.fifoclose();
    sendfifo.fifoclose();
    return 0;
}
