#!/bin/bash
# Script to compile the NeuroTIC library and examples

CC=gcc
CFLAGS="-Iinclude -Wall -Wextra -pedantic -std=c11"
LDFLAGS="-lm"
PROJECT_LOCATION="$1"
PROJECT_NAME="$2"
PLATAFORM="$3"

if [ -z $PROJECT_LOCATION ] || [ -z $PROJECT_NAME ] || [ -z $PLATAFORM ]; then
    echo "Usage: $(basename "$0") <project_location> <project_name> <platform>"
    exit 1
fi
if [ ! -d "$PROJECT_LOCATION" ]; then
    echo "Error: Project location '$PROJECT_LOCATION' does not exist."
    exit 1
fi
if [ ! -f "$PROJECT_LOCATION/$PROJECT_NAME.c" ]; then
    echo "Error: Source file '$PROJECT_NAME.c' does not exist in '$PROJECT_LOCATION'."
    exit 1
fi
if [ ! -d "src/$PLATAFORM" ]; then
    echo "Error: Platform directory 'src/$PLATFORM' does not exist."
    exit 1
fi 

INCLUDES=$(grep -oP '#include\s*"\K[^"]+' "$PROJECT_LOCATION/$PROJECT_NAME.c")

for HEADER in $INCLUDES; do
    if [[ $HEADER == NTIC_* ]]; then
        NTIC_INCLUDES+=("$HEADER")
    else
        USR_INCLUDES+=("$HEADER")
    fi
done

i=0
while [ $i -lt ${#NTIC_INCLUDES} ]; do
    for DIR in "variants/${NTIC_INCLUDES[$i]}" "include/${NTIC_INCLUDES[$i]}" "src/${NTIC_INCLUDES[$i]%.h}.c" "src/$PLATAFORM/${NTIC_INCLUDES[$i]%.h}.c"; do
        [ -f "$DIR" ] && while read -r line; do
            for item in ${NTIC_INCLUDES[@]}; do
                [[ "$item" == "$line" ]] && continue 2
            done
            NTIC_INCLUDES+=("$line")
        done < <(grep -oP '^\s*#include\s*"\K[^"]+' "$DIR")
    done
    ((i++))
done

for obj in ${NTIC_INCLUDES[@]}; do
    for DIR in "src/" "src/$PLATAFORM/"; do
        [ -f "$DIR/${obj%.h}.c" ] && gcc -c -Iinclude "$DIR/${obj%.h}.c" -o "build/${obj%.h}.o"
    done
done

ar rcs $PROJECT_LOCATION/lib/libNTIC.a build/*.o
rm build/*.o
gcc -Iinclude $PROJECT_LOCATION/$PROJECT_NAME.c -o $PROJECT_LOCATION/$PROJECT_NAME -L$PROJECT_LOCATION/lib -lm -lNTIC
