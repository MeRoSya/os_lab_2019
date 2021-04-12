CC = gcc
CFLAGS =-g

all: multiplying.o libmultiplying.a server.o client.o server client 
	rm multiplying.o server.o client.o

server: server.c Multiplying.h
	$(CC) $(CFLAGS) server.o -pthread -L. -lmultiplying -o server

client: client.c Multiplying.h
	$(CC) $(CFLAGS) client.o -pthread -L. -lmultiplying -o client

libmultiplying.a: multiplying.o
	ar rcs libmultiplying.a multiplying.o

server.o: server.c Multiplying.h
	$(CC) $(CFLAGS) -c server.c -o server.o

client.o: client.c Multiplying.h
	$(CC) $(CFLAGS) -c client.c -o client.o

multiplying.o: Multiplying.c Multiplying.h
	$(CC) -c -g Multiplying.c -o multiplying.o

clean:
	rm server client libmultiplying.a
