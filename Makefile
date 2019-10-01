# MakeFile to build the Sample US CENSUS Name Data lookup program 
# For CSC3004 Software Development
# C++ compiler to use
USER= guidotti1
CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14

RM= /bin/rm -f

all: marvelserver marvelclient PutCGI PutHTML

dataEntry.o: dataEntry.cpp dataEntry.h
	$(CC) $(CFLAGS) dataEntry.cpp -c

dataLookup.o: dataLookup.cpp dataLookup.h dataEntry.h
	$(CC) $(CFLAGS) dataLookup.cpp -c

extraFunctions.o: extraFunctions.cpp extraFunctions.h
	$(CC) $(CFLAGS) extraFunctions.cpp -c

marvelserver.o: marvelserver.cpp fifo.h dataEntry.h dataLookup.h extraFunctions.h
	$(CC) -c $(CFLAGS) marvelserver.cpp 
	
marvelclient.o: marvelclient.cpp fifo.h extrafunctions.h
	$(CC) -c $(CFLAGS) marvelclient.cpp 

marvelserver: marvelserver.o fifo.o dataEntry.o dataLookup.o extraFunctions.o
	$(CC) $(CFLAGS) marvelserver.o dataEntry.o dataLookup.o extraFunctions.o fifo.o -o marvelserver

fifo.o: fifo.cpp fifo.h
	g++ -c fifo.cpp
	
marvelclient: marvelclient.o fifo.h extrafunctions.h
	$(CC) marvelclient.o fifo.o extrafunctions.o -o marvelclient -L/usr/local/lib -lcgicc
	
PutCGI: marvelclient
	chmod 757 marvelclient
	cp marvelclient /usr/lib/cgi-bin/$(USER)_marvelLookup.cgi
	cp input.txt /usr/lib/cgi-bin/$(USER)_input.txt
	
	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/
	
PutHTML: 
	cp marvel.html /var/www/html/class/softdev/$(USER)/MarvelLookupServer/
	cp marvel.css /var/www/html/class/softdev/$(USER)/MarvelLookupServer/
	cp marvel.js /var/www/html/class/softdev/$(USER)/MarvelLookupServer/

clean:
	rm -f *.o  main