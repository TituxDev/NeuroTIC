#!/bin/bash
## @file test.sh
## @brief Builds and runs one of the bundled examples.
##
## Must be run from within `examples/`. Prompts for (or accepts as its
## one argument) an example's filename without its `.c` extension,
## compiles it for the CPU platform via the `compile` make target,
## executes the resulting binary, then removes the compiled binary and
## the library artifacts it produced.
##
## Usage:
##   ./test.sh [example]

example="$1"
if [ -z "$example" ]; then
    echo -e "Aviable examples:\033[36m"
    ls *.c
    echo -n -e "\033[0m"
    read -p "Select an example (withour .c extension): " example
fi

cd ..
make compile PROJECT_LOCATION=examples PROJECT_NAME=$example PLATFORM=CPU || exit 1
#valgrind \
./examples/"$example"
rm examples/"$example"
rm examples/lib/*
