#!/bin/bash
## \file compile.sh
## \brief Compilation helper for NeuroTIC projects.
##
## This script resolves project dependencies for a given user project,
## compiles NeuroTIC and user source files, generates static libraries,
## and links the final executable.
##
## Usage:
##   ./compile.sh <project_location> <project_name> <platform>
##
## Example:
##   ./compile.sh ./MyProject main x86_64


## \brief Project parameters.
## \param PROJECT_LOCATION Directory of the user project.
## \param PROJECT_NAME Main C file name without extension.
## \param PLATFORM Target platform folder.
PROJECT_LOCATION="$1"
PROJECT_NAME="$2"
PLATFORM="$3"

## \brief Verify that required arguments are provided.
if [ -z "$PROJECT_LOCATION" ] || [ -z "$PROJECT_NAME" ] || [ -z "$PLATFORM" ]; then
    echo "Usage: $(basename "$0") <project_location> <project_name> <platform>"
    exit 1
fi

## Compiler and flags
CC=gcc
CFLAGS="-Iinclude/$PLATFORM -Wall -Wextra -Wno-missing-field-initializers -pedantic -std=c11 -O3 -march=native -fno-exceptions -fstrict-aliasing"
## \brief Check that project folder exists.
if [ ! -d "$PROJECT_LOCATION" ]; then
    echo "Error: Project location '$PROJECT_LOCATION' does not exist."
    exit 1
fi

## \brief Check that the main source file exists.
if [ ! -f "$PROJECT_LOCATION/$PROJECT_NAME.c" ]; then
    echo "Error: Source file '$PROJECT_NAME.c' not found in '$PROJECT_LOCATION'."
    exit 1
fi

## \brief Check that the platform folder exists.
if [ ! -d "src/$PLATFORM" ]; then
    echo "Error: Platform directory 'src/$PLATFORM' does not exist."
    exit 1
fi 

## Create object and library directories
mkdir -p obj "$PROJECT_LOCATION/obj" "$PROJECT_LOCATION/lib"

## \brief Arrays to track NeuroTIC and user headers
declare -a NTIC_INCLUDES
declare -a USR_INCLUDES

## \brief Parse #include directives in main project file
INCLUDES=$(grep -oP '#include\s*"\K[^"]+' "$PROJECT_LOCATION/$PROJECT_NAME.c")
for HEADER in $INCLUDES; do
    if [[ $HEADER == nt* ]]; then
        NTIC_INCLUDES+=("$HEADER")
    else
        USR_INCLUDES+=("$HEADER")
    fi
done

## \brief Resolve recursive NeuroTIC dependencies
i=0
while [ $i -lt ${#NTIC_INCLUDES[@]} ]; do
    for DIR in "variants/${NTIC_INCLUDES[$i]}" "include/${NTIC_INCLUDES[$i]}" "src/${NTIC_INCLUDES[$i]%.h}.c" "src/$PLATFORM/${NTIC_INCLUDES[$i]%.h}.c"; do
        [ -f "$DIR" ] && while read -r line; do
            [[ ! " ${NTIC_INCLUDES[*]} " =~ " $line " ]] && NTIC_INCLUDES+=("$line")
        done < <(grep -oP '^\s*#include\s*"\K[^"]+' "$DIR")
    done
    ((i++))
done

## \brief Compile NeuroTIC source files into object files
for obj in "${NTIC_INCLUDES[@]}"; do
    for DIR in "src/" "src/$PLATFORM/"; do
        SRC="$DIR/${obj%.h}.c"
        [ -f "$SRC" ] && $CC $CFLAGS -c "$SRC" -o "obj/${obj%.h}.o"
    done
done

## \brief Resolve user dependencies recursively
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

## \brief Compile user source files into object files
for obj in "${USR_INCLUDES[@]}"; do
    for DIR in "$PROJECT_LOCATION/src/" "$PROJECT_LOCATION/src/$PLATFORM/"; do
        SRC="$DIR/${obj%.h}.c"
        [ -f "$SRC" ] && $CC $CFLAGS -c "$SRC" -o "$PROJECT_LOCATION/obj/${obj%.h}.o"
    done
done

## \brief Build static libraries
ls $PROJECT_LOCATION/obj/*.o >/dev/null  2>&1 && ar rcs "$PROJECT_LOCATION/lib/libUSR.a" $PROJECT_LOCATION/obj/*.o && LDFLAGS+=("-lUSR")
ls obj/*.o >/dev/null  2>&1 && ar rcs "$PROJECT_LOCATION/lib/libNTIC.a" obj/*.o && LDFLAGS+=("-lNTIC") && rm -f obj/*.o
LDFLAGS+=("-lm")

## \brief Link final executable
$CC $CFLAGS "$PROJECT_LOCATION/$PROJECT_NAME.c" -o "$PROJECT_LOCATION/$PROJECT_NAME" -L"$PROJECT_LOCATION/lib" ${LDFLAGS[@]}
