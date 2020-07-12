CC=gcc
CFLAGS=-I.

all: server client

server: libs server.c
	$(CC) server.c -L./ -lrepetition -lpthread -o server.out $(CFLAGS)

client: libs client.c
	$(CC) client.c -L./ -lrepetition -lpthread -o client.out $(CFLAGS)

libs: repetition.c
	$(CC) -c repetition.c -o repetition.out
	ar rcs librepetition.a repetition.out

clean :
	rm client.out server.out

