#!/bin/sh

set -xe # use for debugging, it will output the commands run by this script

clang -Wall -Wextra -o twice.bin twice.c -lm
clang -Wall -Wextra -o gates.bin gates.c -lm
clang -Wall -Wextra -o xor.bin xor.c -lm
