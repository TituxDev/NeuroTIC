/**
 * @file one_hot.c
 * @brief Example: Train a neural network to learn a one-hot encoding function.
 * @author Oscar Sotomayor (Titux)
 * @date 2026
 *
 * This example builds a 3-layer feedforward neural network (1-4-8-16) and trains it
 * to learn a one-hot encoding mapping. Given an integer input value in the range 0-15,
 * the network must activate exactly one output neuron corresponding to that value while
 * keeping all other outputs inactive.
 *
 * Expected output:
 *
 * ```sh
 * ~/NeuroTIC/examples$ time bash test.sh one_hot
 * Compiling project in location: examples
 * Project name: one_hot
 * Platform: CPU
 *
 * 26384 bytes written to examples/one_hot
 *
 * Attempts: 56792
 *
 *     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 * 0   1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 * 1   0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 * 2   0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0
 * 3   0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0
 * 4   0  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0
 * 5   0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  0
 * 6   0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  0
 * 7   0  0  0  0  0  0  0  1  0  0  0  0  0  0  0  0
 * 8   0  0  0  0  0  0  0  0  1  0  0  0  0  0  0  0
 * 9   0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0
 * 10  0  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0
 * 11  0  0  0  0  0  0  0  0  0  0  0  1  0  0  0  0
 * 12  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0  0
 * 13  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0
 * 14  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0
 * 15  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1
 *
 * real    0m1.171s
 * user    0m0.990s
 * sys     0m0.185s
 * ```
 *
 * @code{.c}
 */

 #include "ntcomplete.h"
 #include <stdio.h>

#define NETWORK_NAME network

#define INPUTS 1
#define NEURONS_PER_LAYER 4,8,16 // ... , ... , ... 

#define TRAINING_SAMPLES 16
#define LEARNING_RATE 0.1
#define TOLERANCE 0.0
#define MAX_ATTEMPTS 10000000

int main( void ){

  // Define topology
  net_s NETWORK_NAME={
      .inputs= INPUTS,
      .layers= sizeof( (uint16_t []){ NEURONS_PER_LAYER } ) / sizeof( uint16_t )
  };
  newnet( &NETWORK_NAME , (uint16_t []){NEURONS_PER_LAYER} , NETWORK_NAME.layers );
  newfeedforward( &NETWORK_NAME );
  buildnet( &NETWORK_NAME );

  // Select activation functions for each neuron
  for( uint16_t i= 0 ; i < NETWORK_NAME.neurons[0] ; i++ ) NETWORK_NAME.nn[0][i].fn= NTACT_SIGMOID;
  for( uint16_t i= 0 ; i < NETWORK_NAME.neurons[1] ; i++ ) NETWORK_NAME.nn[1][i].fn= NTACT_SIGMOID;
  for( uint16_t i= 0 ; i < NETWORK_NAME.neurons[NETWORK_NAME.layers-1] ; i++ ) NETWORK_NAME.nn[NETWORK_NAME.layers-1][i].fn= NTACT_BOOLEAN;
  
  // Initialize weights
  randnet( &NETWORK_NAME );

  // Prepare training data
  traindata_t data={
      .learning_rate= (precision_t)(LEARNING_RATE),
      .tolerance= (precision_t)(TOLERANCE),
      .max_attempts= MAX_ATTEMPTS,
      .samples= TRAINING_SAMPLES,
  };

  // Allocate training data
  newtraindata( &data , &NETWORK_NAME );

  // Fill training data with your samples and expected outputs
  for( uint8_t i= 0 ; i < TRAINING_SAMPLES ; i++ ){
    data.in[i][0]= i;
    data.results[i][i]= 1; // Set expected output values
  }

  // Train the network
  printf( "Attempts: %li" , backpropagation( &NETWORK_NAME , &data ) );

  printf( "\n\n   " );
  for( uint8_t i= 0 ; i < TRAINING_SAMPLES ; i++ ) if( printf( " %i" , i ) < 3 ) printf( " " );
  for( uint8_t i= 0 ; i < TRAINING_SAMPLES ; i++ ){
    if( printf( "\n%i" , i ) < 3 ) printf( " " );
    NETWORK_NAME.in[0][0]= i;
    feedforward( &NETWORK_NAME );
    for( uint16_t j= 0 ; j < TRAINING_SAMPLES ; j++ ) printf( "  %.0f" , *NETWORK_NAME.out[j] );
  } 

  printf( "\n" );
  return 0;
}

/** @endcode */
