#!/bin/bash
# Script to compile the NeuroTIC library and examples

CC=gcc
CFLAGS="-Iincludes -Wall -Wextra -pedantic -std=c11"
LDFLAGS="-lm"

for f in $(ls src); do \
    $CC $CFLAGS -c src/$f -o objects/${f##*/}.o; \
done

ar rcs libneurotic.a objects/*.o

# Compile examples
for example in examples/*.c; do
    example_name=$(basename $example .c)
    $CC $CFLAGS $example -L. -lneurotic -o examples/$example_name $LDFLAGS
done

