CC = gcc
CFLAGS = -Wall -D__USE_MINGW_ANSI_STDIO -O3
LIBS = -lSDL2main -lSDL2 -lm

# Uncomment below if compiling using MinGW.
# LIBS = -lmingw32 $(LIBS)

OBJS = main.o

all: sdlbrot

sdlbrot: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c main.h

clean:
	rm -f $(OBJS) sdlbrot
