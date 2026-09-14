CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic
TARGET = dungeon
OBJECTS = main.o game.o map.o linkedList.o terminal.o newSleep.o color.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

main.o: main.c game.h map.h linkedList.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h map.h linkedList.h terminal.h newSleep.h color.h
	$(CC) $(CFLAGS) -c game.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c

linkedList.o: linkedList.c linkedList.h
	$(CC) $(CFLAGS) -c linkedList.c

terminal.o: terminal.c terminal.h
	$(CC) $(CFLAGS) -c terminal.c

newSleep.o: newSleep.c newSleep.h
	$(CC) $(CFLAGS) -c newSleep.c

color.o: color.c color.h
	$(CC) $(CFLAGS) -c color.c

test: $(TARGET)
	sh tests.sh

clean:
	rm -f $(TARGET) $(OBJECTS)
