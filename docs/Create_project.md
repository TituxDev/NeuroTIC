# How to Create a NeuroTIC Project

## I. Define a New Project

From the framework’s root directory, run:

```sh
make crate
```

This interactive script will:

• Clear the workspace if it contains previous content
• Ask for project details:
– Project name
– Author name (optional)
– Brief description
– Header pack to use

Header Packs

Header packs are bundles of framework headers that simplify inclusion in your project.
The script automatically detects available packs from the `variants/` directory.

### Example Workflow

```sh
$ make create

# If workspace/ already contains files, the script will ask whether to delete them.

Enter the new project name: my_app
Author name (default: your-username):
Brief project description: My awesome project
Available header packs:
basic
complete
Enter the header pack to use: complete

Project created successfully at workspace/my_app.c
```

### Project Structure Created

After running `make create`, you will have:

```css
workspace/
 ├── ntcomplete.h   (selected header pack)
 ├── my_app.c       (main application file)
 └── my_app.h       (project header file)
 ```

### Generated Files

**my_app.c**  

```c
/**
 * Filename: my_app.c
 * Author: your-username
 * Date: 2025-11-20
 * Description: My awesome project.
*/

/**
 * Pro tip:
 * Use my_app.h to store global constants, macros, variables,
 * or include additional headers. Keep your main clean.
 */
#include "my_app.h"
#include "ntcomplete.h"

int main( void ){

    // Your code here

    return 0;
}
```

**my_app.h** *initially empty*

```c
/**
 * Filename: my_app.h
 * Author: your-username
 * Date: 2025-11-20
 */
```

## II. API Usage

### 1. Declare Network Structure

```c
net_s network= {
    .inputs= 2,
    .layers= 3
};
NEWNET( network , ( (uint16_t []){ 2 , 4 , 1 } ) );
```

This produces a network with 2 inputs and 3 layers containing 2, 4, and 1 neurons.  
*You may also declare a `uint16_t` array manually and pass it to `NEWNET`.*

### 2. Define Connections

```c
newfeedforward( &network );
// OR
newdense( &network );
```

*You can also define connections manually.  
See [Define_connections.md](Wiring.md) for details.*

### 3. Build the Network

```c
buildnet( &network );
```

After this step, the entire topology is allocated, linked, and ready.

### 4, Set activation funcions

```c
for( uint16_t i = 0 ; i < network.layers ; i++ )
    for( uint16_t j = 0 ; j < network.neurons[i] ; j++ )
        network.nn[i][j].fn = NTACT_SIGMOID;
```

*Function labels are defined in the `ntact_function_id_t` enum located in  
[include/ntactivation.h](../include/ntactivation.h).*

### 5. Initalize Weights

```c
randnet( &network );
```

### 6. Prepare Training Data

```c
traindata_t data = {
    .learning_rate = 0.1f,
    .tolerance     = 0.5f,
    .max_attempts  = 10000,
    .samples       = 100
};
newtraindata( &data , &network );
```

This allocates two matrices:

- `data.in[samples][network.inputs]`
- `data.out[samples][network.neurons[last_layer]]`

Fill these matrices with your dataset.

### 7. Network Training

```c
backpropagation( &network , &data );
```

### 8. Attach Input Sources

```c
float source[network.inputs];
for( uint32_t i= 0 ; i < network.inputs ; i++ )
    network.in[i]= &source[i];
```

*Each `network.in[i]` is a pointer to a `float` and may reference any external variable, buffer, or data source.*

### 9. Run the network

```c
// Set source values
feedforward( &network );
```

### 10. Network Persistence

```c
savenet( &network , "my_network" );
// A my_network.ntic file will be generated

net_s network_copy= loadnet( "my_network" );
```

### 11. Read Results

```c
for( uint16_t i= 0 ; i < network.neurons[last_layer] ; i++ )
    printf( "%f" , *network[i] );
```

### Memory Liberation

All memory allocated internally by NeuroTIC is automatically freed on a clean program exit.  
If you want your own pointer to be included in the automatic free list:

```c
int *ptr = malloc( sizeof( int ) );
memtrack( ptr );
```

*Memory reallocation is not handled by this system*  

## III. Compile the Project

From the framework’s root directory:

```sh
make compile PROJECT_LOCATION=workspace PROJECT_NAME=my_app PLATFORM=CPU
```

## Functional Example

See [examples/logic_gates.c](../examples/logic_gates.c) for a complete working demonstration.
