CC = gcc
CFLAGS = -Wall
LIBS = -lSDL2main -lSDL2

OBJS = main.o

all: sdlbrot

sdlbrot: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c main.h
