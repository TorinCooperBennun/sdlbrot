CC = gcc
CFLAGS = -Wall -D__USE_MINGW_ANSI_STDIO -O3
LIBS =
# Uncomment the line directly below if compiling using MinGW.
LIBS += -lmingw32
LIBS += -lSDL2main -lSDL2 -lm

OBJS = main.o
OUT = sdlbrot.exe
# Add .exe to the line directly above if compiling using MinGW.

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c main.h

clean:
	rm -f $(OBJS) $(OUT)