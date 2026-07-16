# NeuroTIC

A lightweight C library for building, training, saving and loading
neural networks from scratch — no dependencies, no frameworks, just
clean C11.

Built for developers who want to understand and control every layer
of their network, without fighting a framework to do it.

**What you can do with it:**
- Define any topology: layers, neurons, activation functions and
  connections are fully yours to configure
- Train networks and serialize trained models to portable `.ntic`
  binary files
- Load and run models in any C project — from servers to
  microcontrollers — with zero external dependencies

## Quick example

```c
#include "ntcomplete.h"

int main( void ) {
    // 2 inputs -> 4 hidden neurons -> 1 output
    CREATE_NET_FEEDFORWARD( net , 2 , ( (uint16_t []){ 4 , 1 } ) );

    for( layer_t i= 0 ; i < net->layers ; i++ )
        for( uint16_t j= 0 ; j < net->neurons[i] ; j++ )
            net->nn[i][j].fn= NTACT_SIGMOID;
    randnet( net );

    traindata_t data= { .learning_rate= 0.1 , .tolerance= 0.01 , .max_attempts= 100000 , .samples= 4 };
    newtraindata( &data , net );
    // ... fill data.in[][] / data.results[][] with your training samples ...
    backpropagation( net , &data );

    savenet( net , "my_network" );
    return 0;
}
```

[More examples](./examples/)

## Documentation

[User Guide](https://tituxdev.github.io/NeuroTIC/docs/doxy/result/html/index.html)

## Learn how it works from scratch

NeuroTIC was built as part of a hands-on learning project.

- [NeuroticLab](https://neuroticlab.com) — step-by-step guide in C
- [Book on Amazon](https://a.co/d/0dphqSgm) — the same journey in book format
- [NeuroSys](https://neurosys.onrender.com) — design and export `.ntic` files in your browser

## License

NeuroTIC is free for personal, educational and open source use under
[GPL-3.0](./LICENSE).  
For commercial use, see [commercial license](./COMMERCIAL_LICENSE.md).