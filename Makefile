CC      = gcc
CFLAGS  = -O2 -Wall -std=c99
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows
SRCS    = main.c utils.c
OBJS    = $(patsubst %.c, %.o, $(SRCS))
TARGET  = TicTacToe

all: $(TARGET)

.PHONY: clean

$(TARGET): $(OBJS)
	@echo Linking...
	$(CC) $^ -Llib $(LDFLAGS) -o bin\$@

%.o: %.c
	@echo Compiling $<...
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

clean:
	$(RM) *.o
	$(RM) bin\main
