# Directories
SRC_DIR = src
INC_DIR = src
DEMO_DIR = demos
# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -g -I$(INC_DIR) -Wall
LDFLAGS = -lm
# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)
DEMO_SRC = $(wildcard $(DEMO_DIR)/*.c)
TARGETS = $(patsubst $(DEMO_DIR)/%.c, %, $(DEMO_SRC))

# Default target
all: $(TARGETS)

# Build each demo as an executable
$(TARGETS): %: $(DEMO_DIR)/%.o $(SRC:.c=.o)
	$(CC) $(CFLAGS) $(SRC:.c=.o) $< -o $@ $(LDFLAGS)

$(SRC:.c=.o): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DEMO_DIR)/%.o: $(DEMO_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGETS) $(SRC_DIR)/*.o $(DEMO_DIR)/*.o
