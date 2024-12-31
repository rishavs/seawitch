# Compiler and compiler flags
CC = tcc
CFLAGS = -std=c99 -Wall

# Source files, header files, and output executable
SRC_DIR 			= src
TEST_DIR 			= test
INCLUDE_DIR 		= include
BIN_DIR 			= bin
PROJECT_EXECUTABLE 	= $(BIN_DIR)/seawitch
TEST_EXECUTABLE 	= $(BIN_DIR)/test
PROJECT_ENTRYPOINT 	= $(SRC_DIR)/seawitch.c
TEST_ENTRYPOINT 	= $(TEST_DIR)/runner.c

# List of source files (each file on a new line)
SRC = \
    $(SRC_DIR)/errors.c \
    $(SRC_DIR)/core/fxstring.c 

# Targets

all: build

run: build
	$(PROJECT_EXECUTABLE)

build: $(PROJECT_EXECUTABLE)

$(PROJECT_EXECUTABLE): $(SRC) $(PROJECT_ENTRYPOINT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC) $(PROJECT_ENTRYPOINT) -o $(PROJECT_EXECUTABLE)

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)
	
$(TEST_EXECUTABLE): $(SRC) $(TEST_ENTRYPOINT)
    $(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC) $(TEST_ENTRYPOINT) -o $(TEST_EXECUTABLE)

clean:
	rm -f $(PROJECT_EXECUTABLE) $(TEST_EXECUTABLE)

.PHONY: all run build clean
