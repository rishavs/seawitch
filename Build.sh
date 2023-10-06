#!/bin/bash

# Define variables for source files, compiler, headers folder, and flags
COMPILER="tcc"
SOURCE_FILES="
    src/seawitch.c 
    src/compiler/compiler.c 
    src/compiler/reader.c
"
HEADERS_FOLDER="src/includes"
COMPILER_FLAGS="-std=c99 -Wall -Wdeprecated-declarations"
OUTPUT_EXECUTABLE="bin/seawitch.exe"

# Compile the C code
$COMPILER $SOURCE_FILES $COMPILER_FLAGS -I$HEADERS_FOLDER -o $OUTPUT_EXECUTABLE

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    
    # Run the compiled program with the provided arguments
    ./$OUTPUT_EXECUTABLE "$@"
else
    echo "Compilation failed."
fi
