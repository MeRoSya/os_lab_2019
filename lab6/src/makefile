CC = gcc
CFLAGS =-g

all: multiplying.o server client 

server: server.c Multiplying.h
	$(CC) $(CFLAGS) server.c -pthreads multiplying.o -o server

client: client.c Multiplying.h
	$(CC) $(CFLAGS) client.c multiplying.o -o client

multiplying.o: Multiplying.c Multiplying.h
	$(CC) -c -g Multiplying.c

clean:
	rm server client multiplying.o
