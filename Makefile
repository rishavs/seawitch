# Compiler and compiler flags
CC = tcc
CFLAGS = -std=c99 -Wall

# Source files, header files, and output executable
SRC_DIR = src
INC_DIR = include
EXE_DIR = bin

# main header file
HDR = $(SRC_DIR)/seawitch.h 

# List of source files (each file on a new line)
SRC = \
    $(SRC_DIR)/seawitch.c \
    $(SRC_DIR)/compiler/compiler.c 

# Header files to consider for dependencies
INCLUDES = $(wildcard $(HDR_DIR)/*.h)

# Output executable
EXE = $(EXE_DIR)/seawitch.exe

# Clear screen command
CLEAR = clear  # Use 'cls' on Windows

# Build target
all: run

# Build the executable
$(EXE): $(SRC) $(HDR) $(INCLUDES)
	@$(CLEAR)
	$(CC) $(CFLAGS) -o $@ $(SRC)

# Run the executable
run: $(EXE)
	@$(CLEAR)
	./$(EXE)

# Build the executable without clearing the screen
build: $(EXE)

# Clean the project
clean:
	rm -f $(EXE)

.PHONY: all run build clean
