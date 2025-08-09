#!/bin/bash
# Script to compile the NeuroTIC library and user project

CC=gcc
CFLAGS="-Iinclude -Wall -Wextra -pedantic -std=c11"
PROJECT_LOCATION="$1"
PROJECT_NAME="$2"
PLATAFORM="$3"

# Validate arguments
if [ -z "$PROJECT_LOCATION" ] || [ -z "$PROJECT_NAME" ] || [ -z "$PLATAFORM" ]; then
    echo "Usage: $(basename "$0") <project_location> <project_name> <platform>"
    exit 1
fi

# Validate paths
if [ ! -d "$PROJECT_LOCATION" ]; then
    echo "Error: Project location '$PROJECT_LOCATION' does not exist."
    exit 1
fi
if [ ! -f "$PROJECT_LOCATION/$PROJECT_NAME.c" ]; then
    echo "Error: Source file '$PROJECT_NAME.c' not found in '$PROJECT_LOCATION'."
    exit 1
fi
if [ ! -d "src/$PLATAFORM" ]; then
    echo "Error: Platform directory 'src/$PLATAFORM' does not exist."
    exit 1
fi 

# Create required directories
mkdir -p obj "$PROJECT_LOCATION/obj" "$PROJECT_LOCATION/lib"

# Scrape includes from main source
declare -a NTIC_INCLUDES
declare -a USR_INCLUDES

INCLUDES=$(grep -oP '#include\s*"\K[^"]+' "$PROJECT_LOCATION/$PROJECT_NAME.c")

for HEADER in $INCLUDES; do
    if [[ $HEADER == NTIC_* ]]; then
        NTIC_INCLUDES+=("$HEADER")
    else
        USR_INCLUDES+=("$HEADER")
    fi
done

# Recursively collect all NTIC_ headers
i=0
while [ $i -lt ${#NTIC_INCLUDES[@]} ]; do
    for DIR in "variants/${NTIC_INCLUDES[$i]}" "include/${NTIC_INCLUDES[$i]}" "src/${NTIC_INCLUDES[$i]%.h}.c" "src/$PLATAFORM/${NTIC_INCLUDES[$i]%.h}.c"; do
        [ -f "$DIR" ] && while read -r line; do
            [[ ! " ${NTIC_INCLUDES[*]} " =~ " $line " ]] && NTIC_INCLUDES+=("$line")
        done < <(grep -oP '^\s*#include\s*"\K[^"]+' "$DIR")
    done
    ((i++))
done

# Compile NTIC core objects
for obj in "${NTIC_INCLUDES[@]}"; do
    for DIR in "src/" "src/$PLATAFORM/"; do
        SRC="$DIR/${obj%.h}.c"
        [ -f "$SRC" ] && $CC $CFLAGS -c "$SRC" -o "obj/${obj%.h}.o"
    done
done

# Recursively collect user headers
i=0
while [ $i -lt ${#USR_INCLUDES[@]} ]; do
    USR_INCLUDES[$i]=$(basename ${USR_INCLUDES[$i]})
    for DIR in "$PROJECT_LOCATION/${USR_INCLUDES[$i]}" "$PROJECT_LOCATION/include/${USR_INCLUDES[$i]}" "$PROJECT_LOCATION/src/${USR_INCLUDES[$i]%.h}.c"; do
        [ -f "$DIR" ] && while read -r line; do
            line=$(basename $line)
            [[ ! " ${USR_INCLUDES[*]} " =~ " $line " ]] && USR_INCLUDES+=("$line")
        done < <(grep -oP '^\s*#include\s*"\K[^"]+' "$DIR")
    done
    ((i++))
done

# Compile user objects
for obj in "${USR_INCLUDES[@]}"; do
    for DIR in "$PROJECT_LOCATION/src/" "$PROJECT_LOCATION/src/$PLATAFORM/"; do
        SRC="$DIR/${obj%.h}.c"
        [ -f "$SRC" ] && $CC $CFLAGS -c "$SRC" -o "$PROJECT_LOCATION/obj/${obj%.h}.o"
    done
done

ls $PROJECT_LOCATION/obj/*.o >/dev/null  2>&1 && ar rcs "$PROJECT_LOCATION/lib/libUSR.a" $PROJECT_LOCATION/obj/*.o && LDFLAGS+=("-lUSR")
ls obj/*.o >/dev/null  2>&1 && ar rcs "$PROJECT_LOCATION/lib/libNTIC.a" obj/*.o && LDFLAGS+=("-lNTIC") && rm -f obj/*.o
LDFLAGS+=("-lm")

# Compile final project binary
$CC $CFLAGS "$PROJECT_LOCATION/$PROJECT_NAME.c" -o "$PROJECT_LOCATION/$PROJECT_NAME" -L"$PROJECT_LOCATION/lib" ${LDFLAGS[@]}
