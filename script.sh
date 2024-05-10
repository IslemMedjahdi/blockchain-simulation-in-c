#!/bin/bash

rm -f  a.out

gcc main.c -o a.out -lssl -lcrypto

if [ $# -gt 0 ]; then
    ./a.out "$@"
else
    ./a.out
fi