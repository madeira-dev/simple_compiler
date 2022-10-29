#!/bin/bash
gcc -Wall -Wa,--execstack -o output_main $1 $2
./output_main
