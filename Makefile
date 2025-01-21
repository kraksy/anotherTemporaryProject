# Compiler and flags
CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -g
LDFLAGS := -lncurses -ljson-c

# Directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

# Target binary
TARGET := $(BUILD_DIR)/game

# Source files and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Default target
all: $(TARGET)

# Linking the binary
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compiling object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

