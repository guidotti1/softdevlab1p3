#include <iostream>
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

string receive_fifo = "Marvelreply";
string send_fifo = "Marvelrequest";

int main() {
	//string message, name;
	string reply;

  //while (1) {
	string message = "1%PETER%0%";

	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	cout << "Send:" << message << endl;
	sendfifo.openwrite();
	sendfifo.send(message);

	/* Get a message from a server */
	recfifo.openread();
	reply = recfifo.recv();

	recfifo.fifoclose();
	sendfifo.fifoclose();
	cout << "Server sent: " << reply << endl;
//  }


}
