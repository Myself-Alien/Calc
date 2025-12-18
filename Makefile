CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = `pkg-config --cflags --libs gtk4`

TARGET = calc
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

