CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses -lcjson

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/core.c $(SRC_DIR)/menu.c $(SRC_DIR)/classpick.c $(SRC_DIR)/settings.c $(SRC_DIR)/enemy.c $(SRC_DIR)/map.c $(SRC_DIR)/ascii_art.c
OBJ = $(SRC:.c=.o)

TARGET = $(BIN_DIR)/game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
