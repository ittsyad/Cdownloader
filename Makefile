CC=gcc
STANDART=-std=c99
all:
	$(CC) $(STANDART) socket.c -o a -lpthread

:
	
