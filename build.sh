#!/bin/sh

set -xe # use for debugging, it will output the commands run by this script

clang -Wall -Wextra -o main main.c
