#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "fifo.h"
#include "extraFunctions.h"

using namespace std;

string receive_fifo = "Marvelreply";
string send_fifo = "Marvelrequest";

int main() {
	//string message, name;
	string reply;
	vector<string> messages;
	messages.push_back("1%PETER%0%");	
	messages.push_back("1%PETER%32%");
  while (1) {


	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
	for (int i =0; i < messages.size(); i++)
	{
		string message = messages[i];
		cout << "Send:" << message << endl;
		sendfifo.openwrite();
		sendfifo.send(message);

		/* Get a message from a server */

		bool end = false;
		recfifo.openread();
		while (end == false)
		{

			cout << "inside while loop"<<endl;
			reply = recfifo.recv();
			if (reply == "$END")
			{
				end = true;	
				recfifo.fifoclose();
				sendfifo.fifoclose();
				break;
			}

		cout << "Server sent: " << reply << endl;
		}
	}




  }

return 0;
}
