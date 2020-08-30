#!/bin/bash

#echo Fetching latest source...

#git pull

echo Compiling...
gcc -lSDL2 main.c -o main

echo Done!

