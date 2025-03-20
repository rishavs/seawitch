# Makefile for Sea Witch Project (Simplified - No Object Directory)

# ------------------------------------------------------------------------------
# Compiler and Flags
# ------------------------------------------------------------------------------

CC = clang
CFLAGS = -Wall -Wextra \
		 -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function \
		 -Wno-sign-conversion -Wno-newline-eof -Wno-deprecated-declarations \
		 -Wno-comment

# Consider reviewing and removing -Wno-* flags. Suppressing warnings should be
# done intentionally and with caution. It's often better to fix the cause
# of the warning.

# Optimization flags (can be added later for release builds, e.g., -O2 or -O3)
# OPT_FLAGS = -O2

# Debug flags (for debug builds, e.g., -g)
DEBUG_FLAGS = -g

# Combine flags (currently using DEBUG_FLAGS, change to OPT_FLAGS for release)
FLAGS = $(CFLAGS) $(DEBUG_FLAGS)


# ------------------------------------------------------------------------------
# Directories and Files
# ------------------------------------------------------------------------------

SRC_DIR		= src
TEST_DIR	= test
INCLUDE_DIR	 = include
BIN_DIR		 = bin

PROJECT_EXECUTABLE  = $(BIN_DIR)/seawitch.exe
TEST_EXECUTABLE	 	= $(BIN_DIR)/tests.exe
PROJECT_ENTRYPOINT  = $(SRC_DIR)/seawitch.c
TEST_ENTRYPOINT	 	= $(TEST_DIR)/runner.c

# List of source files - MANUAL LISTING
SRC_FILES = \
	$(SRC_DIR)/bootstrap.c \

TEST_FILES = \
	$(SRC_DIR)/bootstrap.c \
	$(SRC_DIR)/compiler/compiler.c \
	$(SRC_DIR)/compiler/lexer.c 

# ------------------------------------------------------------------------------
# Targets - Build Process
# ------------------------------------------------------------------------------

all: clear build test # 'all' now builds both 'build' (compiler) and 'test'

# Clear the terminal - convenience target
clear:
	@clear

build: clear build_project # Builds the compiler

test: clear build_tests	# Builds the test executable


# ------------------------------------------------------------------------------
# Build Project Executable (Compiler - 'build' target)
# ------------------------------------------------------------------------------

build_project: $(PROJECT_EXECUTABLE) # Internal target to build seawitch
$(PROJECT_EXECUTABLE): $(SRC_FILES) $(PROJECT_ENTRYPOINT) # Direct compilation and linking
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(SRC_FILES) $(PROJECT_ENTRYPOINT) -o $(PROJECT_EXECUTABLE)
	@echo "Project Executable (Compiler) built: $(PROJECT_EXECUTABLE)"


# ------------------------------------------------------------------------------
# Build Test Executable ('test' target)
# ------------------------------------------------------------------------------

build_tests: $(TEST_EXECUTABLE) # Internal target to build test
$(TEST_EXECUTABLE): $(TEST_FILES) $(PROJECT_ENTRYPOINT) $(SRC_FILES) $(TEST_ENTRYPOINT) # Direct compilation and linking - now includes SRC_FILES for tests to link against
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(TEST_FILES) $(TEST_ENTRYPOINT) -o $(TEST_EXECUTABLE)
	@echo "Test Executable built: $(TEST_EXECUTABLE)"


# ------------------------------------------------------------------------------
# Clean Target
# ------------------------------------------------------------------------------

clean:
	rm -f $(PROJECT_EXECUTABLE) $(TEST_EXECUTABLE) # Clean only executables now
	@echo "Cleaned: Executables removed."


# ------------------------------------------------------------------------------
# Phony Targets - Targets that are not files
# ------------------------------------------------------------------------------

.PHONY: all clear build test directories build_project build_tests clean