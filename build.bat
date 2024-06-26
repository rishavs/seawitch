@echo off

@REM REM Clear the screen
cls

REM Define variables for source files, compiler, headers folder, and flags
SET COMPILER=clang
SET HEADERS_FOLDER=includes
SET OUTPUT_EXECUTABLE=bin\seawitch.exe

SET SOURCE_FILES= src\seawitch.c ^
    src\compiler\compiler.c ^
    src\compiler\reader.c ^
    src\compiler\lexer.c ^
    src\compiler\parser.c 

SET COMPILER_FLAGS=-std=c99 -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -Wno-newline-eof -Wno-deprecated-declarations

    @REM -pedantic -Wall -Wextra ^
    @REM -Wconversion -Wdouble-promotion -Wnull-dereference -Wwrite-strings  ^
    @REM -Wformat=2 -Wcast-align -Wswitch-enum ^
    @REM -fsanitize=undefined -fsanitize-trap ^
    @REM -Wno-deprecated-declarations -Wno-newline-eof 

REM Compile the C code
%COMPILER% %COMPILER_FLAGS% -I%HEADERS_FOLDER% %SOURCE_FILES% -o %OUTPUT_EXECUTABLE%

REM Check if the compilation was successful
IF %ERRORLEVEL% EQU 0 (

    echo Compilation successful. Running the program...

    REM Run the compiled program with the provided arguments
    %OUTPUT_EXECUTABLE% %*
) ELSE (
    echo Compilation failed.
)
