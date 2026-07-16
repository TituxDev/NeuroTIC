#!/bin/bash
## @file new.sh
## @brief Interactively scaffolds a new NeuroTIC project.
##
## Ensures the `workspace/` directory exists and is empty (offering to
## clear it otherwise), then prompts for a project name, author, and
## description, and an optional pre-built header pack from `variants/` to
## copy in. Generates `workspace/PROJECT_NAME.h` and
## `workspace/PROJECT_NAME.c`, the latter pre-filled with a complete,
## runnable example: building a feedforward network, initializing and
## training it via backpropagation, evaluating a chosen sample, and
## saving the trained network to disk.
##
## Usage:
##   ./new.sh

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
    ls variants | grep nt.*\.h | sed 's/nt//; s/\.h//'
    echo -e -n "\033[0m"
    read -p "Enter the header pack to use: " HEADER
    [ -z "$HEADER" ] && break
    [ -f "variants/nt$HEADER.h" ] && cp variants/nt$HEADER.h workspace/ && break
    echo -e "\033[31mHeader pack not found. Please choose a valid header pack.\033[0m"
done

touch workspace/$PROJECT_NAME.h
{
echo "/**"
echo " * Filename: $PROJECT_NAME.h"
echo " * Author: $AUTHOR"
echo " * Date: $(date +'%Y-%m-%d')"
echo " */"
echo ""
echo " #include <stdio.h>"
echo " #include <string.h>"
} > workspace/$PROJECT_NAME.h

touch workspace/$PROJECT_NAME.c
{
echo " // Filename: $PROJECT_NAME.c"
echo " // Author: $AUTHOR"
echo " // Date: $(date +'%Y-%m-%d')"
echo " // Description: $DESCRIPTION."
echo ""
echo " // Pro tip: use $PROJECT_NAME.h to store global constants, macros, variables,"
echo " // or include additional headers. Keep your main clean."
echo ""
echo "#include \"$PROJECT_NAME.h\""
[ -n "$HEADER" ] && echo "#include \"nt$HEADER.h\""
echo ""
echo "#define NETWORK_NAME $PROJECT_NAME"
echo "#define OUTPUT_DIR \"./${PROJECT_NAME}_net\""
echo ""
echo "#define INPUTS 1"
echo "#define NEURONS_PER_LAYER 1 // Add more comma-separated values for additional layers"
echo "#define ACTIVATION_FUNCTIONS NTACT_SIGMOID // See ntactivation.h for available functions"
echo ""
echo "#define TRAINING_SAMPLES 1"
echo "#define LEARNING_RATE 0.1"
echo "#define TOLERANCE 0.0"
echo "#define MAX_ATTEMPTS 1"
echo ""
echo "int main( void ) {"
echo ""
echo "    // Your code here"
echo ""
echo "    CREATE_NET_FEEDFORWARD( NETWORK_NAME , INPUTS , ( (uint16_t []){ NEURONS_PER_LAYER } ) );"
echo "    // Select activation functions for each neuron"
echo "    for( layer_t i= 0 ; i < NETWORK_NAME->layers ; i++ ) for( uint16_t j= 0 ; j < NETWORK_NAME->neurons[i] ; j++ )"
echo "        NETWORK_NAME->nn[i][j].fn= ACTIVATION_FUNCTIONS;"
echo "    randnet( NETWORK_NAME );"
echo ""
echo "    // Prepare training data"
echo "    traindata_t data= {"
echo "        .learning_rate= (precision_t)(LEARNING_RATE),"
echo "        .tolerance= (precision_t)(TOLERANCE),"
echo "        .max_attempts= MAX_ATTEMPTS,"
echo "        .samples= TRAINING_SAMPLES,"
echo "    };"
echo "    newtraindata( &data , NETWORK_NAME );"
echo "    // Fill training data with your samples and expected outputs"
echo "    // for( sample_t i= 0 ; i < TRAINING_SAMPLES ; i++ ){"
echo "    //     for( input_t j= 0 ; j < NETWORK_NAME->inputs ; j++ ) data.in[i][j]= ...; // Set input values"
echo "    //     for( uint16_t j= 0 ; j < NETWORK_NAME->neurons[NETWORK_NAME->layers-1] ; j++ ) data.results[i][j]= ...; // Set expected output values"
echo "    // }"
echo ""
echo "    backpropagation( NETWORK_NAME , &data );"
echo ""
echo "    savenet( NETWORK_NAME , OUTPUT_DIR );"
echo ""
echo "    // Reconnect network inputs -- backpropagation() releases its own"
echo "    // internal input buffer when it finishes"
echo "    data_t inputs[NETWORK_NAME->inputs];"
echo "    for( input_t i= 0 ; i < NETWORK_NAME->inputs ; i++ ) NETWORK_NAME->in[i]= &inputs[i];"
echo "    // Fill inputs[] with the values you want to evaluate"
echo "    feedforward( NETWORK_NAME );"
echo ""
echo ""
echo "    // Your code here"
echo ""
echo "    return 0;"
echo "}"
} > workspace/$PROJECT_NAME.c

echo -e "Project created successfully at  \033[32;4mworkspace/$PROJECT_NAME.c\033[0m"

echo ""
