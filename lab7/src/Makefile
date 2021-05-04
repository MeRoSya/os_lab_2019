CC = gcc
CFLAGS =-g

all: tcpserver.o tcpclient.o tcpserver tcpclient udpserver.o udpclient.o udpserver udpclient
	rm tcpserver.o tcpclient.o udpclient.o udpserver.o

tcpserver: tcpserver.c
	$(CC) $(CFLAGS) tcpserver.o -o tcpserver

tcpclient: tcpclient.c
	$(CC) $(CFLAGS) tcpclient.o -o tcpclient

tcpserver.o: tcpserver.c
	$(CC) $(CFLAGS) -c tcpserver.c -o tcpserver.o

tcpclient.o: tcpclient.c
	$(CC) $(CFLAGS) -c tcpclient.c -o tcpclient.o

udpserver: udpserver.c
	$(CC) $(CFLAGS) udpserver.o -o udpserver

udpclient: udpclient.c
	$(CC) $(CFLAGS) udpclient.o -o udpclient

udpserver.o: udpserver.c
	$(CC) $(CFLAGS) -c udpserver.c -o udpserver.o

udpclient.o: udpclient.c
	$(CC) $(CFLAGS) -c udpclient.c -o udpclient.o

clean:
	rm tcpserver tcpclient udpclient udpserver
