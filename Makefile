# MakeFile to build the Sample US CENSUS Name Data lookup program 
# For CSC3004 Software Development
# C++ compiler to use
CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14

RM= /bin/rm -f

all: marvelLookup PutCGI PutHTML

dataEntry.o: dataEntry.cpp dataEntry.h
	$(CC) $(CFLAGS) dataEntry.cpp -c

dataLookup.o: dataLookup.cpp dataLookup.h dataEntry.h
	$(CC) $(CFLAGS) dataLookup.cpp -c

extraFunctions.o: extraFunctions.cpp extraFunctions.h
	$(CC) $(CFLAGS) extraFunctions.cpp -c

marvelLookup.o: marvelLookup.cpp dataEntry.h dataLookup.h extraFunctions.h
	$(CC) $(CFLAGS) marvelLookup.cpp -c

marvelLookup: marvelLookup.o dataEntry.o dataLookup.o extraFunctions.o
	$(CC) $(CFLAGS) marvelLookup.o -o marvelLookup dataEntry.o dataLookup.o extraFunctions.o -L/usr/local/lib -lcgicc
	
PutCGI: marvelLookup
	chmod 757 marvelLookup
	cp marvelLookup /usr/lib/cgi-bin/$(USER)_marvelLookup.cgi
	cp input.txt /usr/lib/cgi-bin/$(USER)_input.txt
	
	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/
	
PutHTML: 
	cp marvel.html /var/www/html/class/softdev/$(USER)/MarvelLookup/
	cp marvel.css /var/www/html/class/softdev/$(USER)/MarvelLookup/
	cp marvel.js /var/www/html/class/softdev/$(USER)/MarvelLookup/

clean:
	rm -f *.o  main
