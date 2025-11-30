# Data Flow System

Description of how NeuroTIC represents and constructs the data-flow wiring between layers. It explains the metadata that defines each buffer, how sources are specified, and how the system builds the pointer structures that neurons use as their inputs.

## 1. Purpose of the Wiring System

Its purpose is to give developers precise, explicit control over how data moves through the network. Every buffer, every connection, and every neuron input is defined in a way that can be inspected, reasoned about, and tuned. Instead of hiding behavior behind opaque abstractions, the wiring system exposes the internal structure of the model so developers can extend, optimize, or redesign it with the same freedom they’d expect when working directly in low-level C.

## 2. How the Wiring System Works

All information about internal data flow is stored inside net_s.

> net_s.in is an array of pointers; each entry may reference any external or internal data source.  
> Neurons in the first layer do not allocate inputs; each nn[0][n].in simply aliases net_s.in.  
> For every subsequent layer, a wiring_s struct describes how that layer’s buffers behave: how many there are, their type, and where each pointer in them comes from.  
> Each neuron selects which buffer to use through its bff_idx field.  
> buildnet() consumes the wiring description, allocates buffers, sets up their pointer sources, and finally binds each neuron's in field to the correct buffe

```c
// Layer 0
for N = 0 .. net_s.neurons[0]-1:
    net_s.nn[0][N].in = net_s.in

// Layers 1 to last layer
for L = 1 .. net_s.layers-1:
    for N = 0 .. net_s.neurons[L]-1:
        int idx = net_s.nn[L][N].bff_idx
        net_s.nn[L][N].in = net_s.bff[idx]
```

## 3. Buffer Description (wiring_s)

Each wiring_s entry inside net_s.wiring describes how a layer's buffers behave.
The array_type field determines whether the buffer is:

- an alias to an existing data source, or
- a newly allocated mixed buffer that gathers pointers from multiple sources.

Valid types: `'I'`, `'O'`, `'N'`, `'M'`.
`Undefined` values results in `bff = NULL`.

**3.1 Alias Types**  

`Types 'I' and 'O'`  
These types do not allocate memory.
They simply expose global arrays already owned by net_s.

- 'I' → alias to net_s.in
- 'O' → alias to net_s.out

Rules:

- bff[L][A] = net_s.in or net_s.out
- size[L][A] is set to the size of that array
- All source-descriptor fields (src_type, src_layer, src_index) are ignored
- These aliases allow any layer to consume external inputs or final outputs directly.

`Type 'N'`

- Type 'N' aliases an already-existing internal buffer in net_s.bff.
- No memory is allocated.
- The alias is resolved using the first source descriptor:

```c
ref_layer = src_layer[A][0]
ref_index = src_index[A][0]
bff[L][A] = bff[ref_layer][ref_index]
```

- size[L][A] is copied from the referenced buffer
- src_type is ignored (the alias depends only on which buffer is targeted)

This mechanism allows reuse of buffer arrays and creation of non-linear or recurrent topologies without duplicating memory.

### 3.2 Source Types

> buildnet() automatically wires:  
> all neurons of layer 0 → nn[0][N].in = net_s.in  
> all outputs of the last layer → net_s.out[N] = nn[last][N].out

These two arrays define the external boundary of the network.

`Type 'M'`  
Defines a new allocated buffer inside:

```c
bff[L][A][ size[L][A] ]
```

Each entry in this buffer comes from a source described by the corresponding scr_* descriptor.
This buffer becomes the input array for the neurons of the next layer:

```c
nn[L+1][N].in = bff[L][A]
```

At build time, for every element k of the buffer, the source descriptor resolves a pointer:

> **scr_type = 'I' or 'O'**  
> The element points directly to net_s.in[scr_index] or net_s.out[scr_index].  
> scr_layer is ignored (implicitly refers to the global in/out arrays).

> **scr_type = 'N'**  
> The element references the output of a specific neuron:

```c
bff[L][A][k] = &nn[scr_layer][scr_index].out
```

> Any undefined scr_type generates a NULL pointer.

> Type 'M' is the only type that actually constructs new buffers.  
> All other types only reference existing structures.

### 4. Examples

**4.1 Classic Feed-Forward**  

```c
uint16_t L= net_s.layers - 1;
memtrack( net_s.wiring= malloc (L * sizeof( wiring_s ) ) );
for( uint16_t i= 0 ; i < L ; i++ ){
    uint16_t count= net_s.neurons[i];
    net_s.wiring[i].arrays= 1;
    memtrack( net_s.wiring[i].array_type= malloc( sizeof( uint8_t ) ) );
    net_s.wiring[i].array_type[0]= 'M';
    memtrack( net_s.wiring[i].size= malloc( sizeof( uint32_t ) ) );
    net_s.wiring[i].size[0]= count;
    memtrack( net_s.wiring[i].src_type= malloc( sizeof( uint8_t * ) ) );
    memtrack( net_s.wiring[i].src_layer= malloc( sizeof( uint16_t * ) ) );
    memtrack( net_s.wiring[i].src_index= malloc( sizeof( uint16_t * ) ) );
    memtrack( net_s.wiring[i].src_type[0]= malloc( count * sizeof( uint8_t ) ) );
    memtrack( net_s.wiring[i].src_layer[0]= malloc( count * sizeof( uint16_t ) ) );
    memtrack( net_s.wiring[i].src_index[0]= malloc( count * sizeof( uint16_t ) ) );
    for( uint16_t j= 0 ; j < count ; j++ ){
        net_s.wiring[i].src_type[0][j]= 'N';
        net_s.wiring[i].src_layer[0][j]= i;
        net_s.wiring[i].src_index[0][j]= j;
        net_s.nn[i][j].bff_idx= 0;
    }
}
```

**No Secuential Connection**  

```c
//net_s.layers= 3
//ner_s.neurons[]= { 3 , 4 , 4}
uint16_t L = net_s.layers - 1;

/* Allocate wiring for all transitions between layers */
memtrack( net_s.wiring = malloc( L * sizeof( wiring_s ) ) );

/* Layer-to-layer array counts
   wiring[0] → 1 buffer (normal M)
   wiring[1] → 2 buffers (M + I) */
net_s.wiring[0].arrays = 1;
net_s.wiring[1].arrays = 2;

/* Initialize each wiring block */
for( uint16_t i = 0 ; i < L ; i++ ){

    /* Allocate array types for this transition */
    memtrack( net_s.wiring[i].array_type = malloc( net_s.wiring[i].arrays * sizeof( uint8_t ) ) );

    /* First array always type 'M' */
    net_s.wiring[i].array_type[0] = 'M';

    /* Allocate structural arrays for this transition */
    memtrack( net_s.wiring[i].size      = malloc( net_s.wiring[i].arrays * sizeof( uint32_t ) ) );
    memtrack( net_s.wiring[i].src_type  = malloc( net_s.wiring[i].arrays * sizeof( uint8_t  * ) ) );
    memtrack( net_s.wiring[i].src_layer = malloc( net_s.wiring[i].arrays * sizeof( uint16_t * ) ) );
    memtrack( net_s.wiring[i].src_index = malloc( net_s.wiring[i].arrays * sizeof( uint16_t * ) ) );

    /* First array maps exactly all neurons of layer i */
    net_s.wiring[i].size[0] = net_s.neurons[i];

    /* Allocate source descriptors for the first array */
    memtrack( net_s.wiring[i].src_type [0] = malloc( net_s.wiring[i].size[0] * sizeof( uint8_t  ) ) );
    memtrack( net_s.wiring[i].src_layer[0] = malloc( net_s.wiring[i].size[0] * sizeof( uint16_t ) ) );
    memtrack( net_s.wiring[i].src_index[0] = malloc( net_s.wiring[i].size[0] * sizeof( uint16_t ) ) );

    /* Define feed forwards for array 0 */
    for( uint16_t j= 0 ; j < net_s.wiring[i].size[0] ; j++ ){
        net_s.wiring[i].scr_type[0][j]= 'N';
        net_s.wiring[i].scr_layer[0][j]= i;
        net_s.wiring[i].scr_index[0][j]= j;
    }

}

/* Second buffer of wiring[1] is type Input ('I') */
net_s.wiring[1].array_type[1] = 'I';

/* Assign bff_idx for layer 1 (all neurons → buffer 0) */
for( uint32_t i = 0 ; i < net_s.neurons[0] ; i++ ){
    net_s.nn[1][i].bff_idx = 0;
}

/* Layer 2 receives two different buffers:
   - neurons 0 and 1 use buffer 0 (M)
   - neurons 2 and 3 use buffer 1 (I) */
net_s.nn[2][0].bff_idx = 0;
net_s.nn[2][1].bff_idx = 0;
net_s.nn[2][2].bff_idx = 1;
net_s.nn[2][3].bff_idx = 1;
```
