#declare the variable
CC=gcc

CFLAGS=-c -Wall -Werror -g

all: server

server: server.o treeSaveSys.o utilitys.o txtSaveSys.o
	$(CC) server.o treeSaveSys.o utilitys.o txtSaveSys.o -o server

server.o: server.c
	$(CC) $(CFLAGS) server.c

treeSaveSys.o: treeSaveSys.c
	$(CC) $(CFLAGS) treeSaveSys.c

utilitys.o: utilitys.c
	$(CC) $(CFLAGS) utilitys.c	

txtSaveSys.o: txtSaveSys.c
	$(CC) $(CFLAGS) txtSaveSys.c


clean:
	rm -rf *o server
	
