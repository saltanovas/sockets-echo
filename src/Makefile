# -*- Makefile -*-

CC=gcc
CFLAGS= -c
LDFLAGS= -lm -lpthread
DEPS= client.h server.h helpers.h
OBJ= client.o server.o helpers.o main.o

all: compile

compile: $(OBJ)
	$(CC) $(OBJ) -o Program $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o Program
	