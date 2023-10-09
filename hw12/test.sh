#!/bin/bash

# Test when the getRand program cannot be found
make clean
./getRand 1000 random_words.txt
if [ $? -eq 0 ]; then
    echo "Error: 'getRand' program somehow exists."
else
    ./multithreading
    if [ $? -eq 0 ]; then
        echo "Error: 'multithreading' somehow exists."
    else
        echo "Success: 'getRand' and 'multithreading' program not found."
    fi
fi
make clean

# Test 0 word
make
./getRand 0 random_words.txt
if [ $? -eq 0 ]; then
    ./multithreading
    if [ $? -eq 0 ]; then
        if [ -z "$(find . -maxdepth 1 -type f -name 'output_batch*' -print -quit)" ]; then
            echo "Success: No output file is created since the input file is empty."
        else
            echo "Error: Some output files are created despite the input file is empty."
        fi
    else
        echo "Error: 'multithreading' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 1000 words
make
./getRand 1000 random_words.txt
if [ $? -eq 0 ]; then
    ./multithreading
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 10 ]; then
            echo "Success: There are 10 output files."
        else
            echo "Error: There are not 10 output files."
        fi
    else
        echo "Error: 'multithreading' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean

# Test 10000 words
make
./getRand 10000 random_words.txt
if [ $? -eq 0 ]; then
    ./multithreading
    if [ $? -eq 0 ]; then
        if [ "$(find . -maxdepth 1 -type f -name 'output_batch*' | wc -l)" -eq 100 ]; then
            echo "Success: There are 100 output files."
        else
            echo "Error: There are not 100 output files."
        fi
    else
        echo "Error: 'multithreading' program failed."
    fi
else
    echo "Error: 'getRand' program failed."
fi
make clean