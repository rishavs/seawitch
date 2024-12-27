@echo off

REM REM Clear the screen
cls

REM Define variables for source files, compiler, headers folder, and flags
SET COMPILER=gcc
SET HEADERS_FOLDER=include
SET OUTPUT_EXECUTABLE=bin\seawitch
SET TEST_EXECUTABLE=bin\test
SET MAIN_FILE=src\seawitch.c
SET TEST_FILE=spec\runner.c

SET SOURCE_FILES= ^
    src\compiler\errors.c ^
    src\core\dynstring.c ^
    src\core\dynarray.c

    @REM src\seawitch.c ^
    @REM src\compiler\compiler.c ^
    @REM src\compiler\reader.c ^
    @REM src\compiler\lexer.c ^
    @REM src\compiler\parser.c 

SET COMPILER_FLAGS= -Wall -Wextra ^
    -Wno-unused-variable -Wno-unused-parameter ^
    -Wno-unused-function -Wno-sign-conversion ^
    -Wno-newline-eof -Wno-deprecated-declarations

    REM -pedantic -Wall -Wextra ^
    REM -Wconversion -Wdouble-promotion -Wnull-dereference -Wwrite-strings  ^
    REM -Wformat=2 -Wcast-align -Wswitch-enum ^
    REM -fsanitize=undefined -fsanitize-trap ^
    REM -Wno-deprecated-declarations -Wno-newline-eof 

REM Cleanup the previous build
del %TEST_EXECUTABLE%.exe

REM Compile the C code
%COMPILER% %COMPILER_FLAGS% -I%HEADERS_FOLDER% %TEST_FILE% %SOURCE_FILES% -o %TEST_EXECUTABLE%

REM Check if the compilation was successful
IF %ERRORLEVEL% EQU 0 (

    echo Compilation successful. Running the program...

    REM Run the compiled program with the provided arguments
    @REM %OUTPUT_EXECUTABLE% %*
    %TEST_EXECUTABLE%
) ELSE (
    echo Compilation failed.
)
