#!/bin/bash
# This script creates a new project in the workspace directory.

mkdir -p workspace/{include,src,obj,lib} && touch workspace/{include,src,obj,lib}/.gitkeep
echo ""

if [ -n "$(find workspace -type f ! -name .gitkeep)" ] || [ -n "$(find workspace -type d -empty)" ]; then
    echo -e "\033[31mError: The workspace directory is not empty."
    read -p "Do you want to remove all files in the workspace directory? (y/n): " OPT
    if [[ "$OPT" == "y" || "$OPT" == "Y" ]]; then
        find workspace -type f ! -name .gitkeep -delete
        find workspace -type d -empty -delete
        echo -e "\033[32mWorkspace cleared.\033[0m\n"
    else
        echo -e "\033[31mOperation cancelled. Exiting.\033[0m\n"
        exit 1
    fi
fi

while true; do
    read -p "Enter the new project name: " PROJECT_NAME
    [[ -z "$PROJECT_NAME" ]] && echo -e "\033[31mProject name cannot be empty.\033[0m" && continue
    [[ ! "$PROJECT_NAME" =~ ^[a-zA-Z0-9_]+$ ]] && echo -e "\033[31mProject name can only contain letters, numbers, and underscores.\033[0m" && continue
    break
done
read -r -p "Author name (default: $(whoami)): " AUTHOR
AUTHOR=${AUTHOR:-$(whoami)}
read -r -p "Brief project description: " DESCRIPTION
while true; do
    echo -e "Available header packs:\033[34m"
    ls variants | grep NTIC_.*\.h | sed 's/NTIC_//; s/\.h//'
    echo -e -n "\033[0m"
    read -p "Enter the header pack to use: " HEADER
    [ -z "$HEADER" ] && break
    [ -f "variants/NTIC_$HEADER.h" ] && cp variants/NTIC_$HEADER.h workspace/ && break
    echo -e "\033[31mHeader pack not found. Please choose a valid header pack.\033[0m"
done

touch workspace/$PROJECT_NAME.h
{
echo "/**"
echo " * Filename: $PROJECT_NAME.h"
echo " * Author: $AUTHOR"
echo " * Date: $(date +'%Y-%m-%d')"
echo " */"
} > workspace/$PROJECT_NAME.h

touch workspace/$PROJECT_NAME.c
{
echo "/**"
echo " * Filename: $PROJECT_NAME.c"
echo " * Author: $AUTHOR"
echo " * Date: $(date +'%Y-%m-%d')"
echo " * Description: $DESCRIPTION."
echo "*/"
echo ""
echo "/**"
echo " * Pro tip: use $PROJECT_NAME.h to store global constants, macros, variables,"
echo " * or include additional headers. Keep your main clean."
echo " */"
echo "#include \"$PROJECT_NAME.h\""
[ -n "$HEADER" ] && echo "#include \"NTIC_$HEADER.h\""
echo ""
echo "int main( void ){"
echo ""
echo "    // Your code here"
echo ""
echo "    return 0;"
echo "}"
} > workspace/$PROJECT_NAME.c

echo -e "Project created successfully at  \033[32;4mworkspace/$PROJECT_NAME.c\033[0m"

echo ""