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

SET SOURCE_FILES=src\core\errors.c ^
    src\core\dyn_fx_string.c ^
    src\core\dynarray.c ^
    
    @REM src\core\dynstring.c 
    @REM src\core\dynarray.c
    @REM src\seawitch.c ^
    @REM src\compiler\compiler.c ^
    @REM src\compiler\reader.c ^
    @REM src\compiler\lexer.c ^
    @REM src\compiler\parser.c 

SET COMPILER_FLAGS= -Wall -Wextra ^
    -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion ^
    -Wno-newline-eof -Wno-deprecated-declarations -Wno-comment

@REM Testing flags. Only used during testing
SET TESTING_FLAGS= 
@REM -Wno-maybe-uninitialized 

    REM -pedantic -Wall -Wextra ^
    REM -Wconversion -Wdouble-promotion -Wnull-dereference -Wwrite-strings  ^
    REM -Wformat=2 -Wcast-align -Wswitch-enum ^
    REM -fsanitize=undefined -fsanitize-trap ^
    REM -Wno-deprecated-declarations -Wno-newline-eof 

REM Cleanup the previous build
@REM del %TEST_EXECUTABLE%.exe

REM Compile the C code
%COMPILER% %COMPILER_FLAGS% %TESTING_FLAGS% -I%HEADERS_FOLDER% %TEST_FILE% %SOURCE_FILES% -o %TEST_EXECUTABLE%

REM Check if the compilation was successful
IF %ERRORLEVEL% EQU 0 (

    echo Compilation successful. Running the program...

    REM Run the compiled program with the provided arguments
    @REM %OUTPUT_EXECUTABLE% %*
    %TEST_EXECUTABLE%
) ELSE (
    echo Compilation failed.
)
