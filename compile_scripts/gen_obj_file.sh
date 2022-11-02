#!/bin/bash
echo gcc -c $1.s
gcc -c $1.s
echo objdump -d $1.o
objdump -d $1.o
