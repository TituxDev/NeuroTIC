#!/bin/bash

example="$1"
if [ -z "$example" ]; then
    echo -e "Aviable examples:\033[36m"
    ls *.c
    echo -n -e "\033[0m"
    read -p "Select an example (withour .c extension): " example
fi

cd ..
make compile PROJECT_LOCATION=examples PROJECT_NAME=$example PLATFORM=CPU
#valgrind \
./examples/"$example"
rm examples/"$example"
rm examples/lib/*
