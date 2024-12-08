CC =gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g
LDFLAGS = -lncurses
TARGET = game
SRC = ratan.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
