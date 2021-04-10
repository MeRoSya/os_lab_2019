CC = gcc
CFLAGS =-g

all: deadlock mutex factorial

factorial: factorial.c
	$(CC) $(CFLAGS) factorial.c -pthreads -o factorial

deadlock: deadlock.c
	$(CC) $(CFLAGS) deadlock.c -o deadlock

mutex: mutex.c
	$(CC) $(CFLAGS) mutex.c -o mutex

clean:
	rm factorial deadlock mutex
