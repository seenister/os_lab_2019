all: factorial deadlock

factorial: factorial.c
	gcc -o factorial.out factorial.c -lpthread -lm -std=c99
deadlock: deadlock.c
	gcc -o deadlock.out deadlock.c -lpthread -std=c99
