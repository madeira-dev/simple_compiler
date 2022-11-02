#!/bin/bash
echo gcc -Wall -Wa,--execstack -o output_main $1 $2
gcc -Wall -Wa,--execstack -o output_main $1 $2
echo ./output_main
./output_main
