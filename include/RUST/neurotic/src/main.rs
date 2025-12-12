mod neuron;
mod activation;
use crate::neuron::Neuron;
use crate::activation::ActivationList::{Sigmoid, Step};
use std::cell::Cell;
use std::rc::Rc;

fn main() {
    let data_set: Vec<Vec<f32>> = vec![
        vec![0.0, 0.0],
        vec![0.0, 1.0],
        vec![1.0, 0.0],
        vec![1.0, 1.0],
    ];
    let mut results: Vec<f32>;
    let source_a: Rc<Cell<f32>> = Rc::new(Cell::new(0.0));
    let source_b: Rc<Cell<f32>> = Rc::new(Cell::new(0.0));
    let mut neuron: Vec<Neuron> = vec![
        Neuron::new(2 , Sigmoid),
        Neuron::new(2 , Sigmoid),
        Neuron::new(2 , Step),
    ];
    neuron[0].input.push(source_a.clone());
    neuron[0].input.push(source_b.clone());
    neuron[0].weights= vec![1.0,1.0];
    neuron[0].bias= 1.5;
    neuron[1].input.push(source_a.clone());
    neuron[1].input.push(source_b.clone());
    neuron[1].weights= vec![1.0,1.0];
    neuron[1].bias= 1.5;
    neuron[2].input.push(source_a.clone());
    neuron[2].input.push(source_b.clone());
    neuron[2].weights= vec![1.0,1.0];
    neuron[2].bias= 1.5;
    let mut error: f32;
    let mut total_error: f32 = 1.0;
    let mut epoch: i32 = 0;
    results= vec![1.0, 1.0, 1.0, 0.0];
    while total_error != 0.0 && epoch < 1000 {
        total_error= 0.0;
        epoch+= 1;
        for( i, source) in data_set.iter().enumerate(){
            source_a.set(source[0]);
            source_b.set(source[1]);
            neuron[0].activate();
            error= results[i] - neuron[0].output.get();
            neuron[0].weights[0] += error * source[0] * 0.1;
            neuron[0].weights[1] += error * source[1] * 0.1;
            neuron[0].bias += error * 0.1;
            total_error += error.abs();
        }
    }
    results= vec![0.0, 1.0, 1.0, 1.0];
    total_error= 1.0;
    epoch= 0;
    while total_error >= 0.1 && epoch < 1000 {
        total_error= 0.0;
        epoch+= 1;
        for( i, source) in data_set.iter().enumerate(){
            source_a.set(source[0]);
            source_b.set(source[1]);
            neuron[1].activate();
            error= results[i] - neuron[1].output.get();
            neuron[1].weights[0] += error * source[0] * 0.1;
            neuron[1].weights[1] += error * source[1] * 0.1;
            neuron[1].bias += error * 0.1;
            total_error += error.abs();
        }
    }
    results= vec![0.0, 0.0, 0.0, 1.0];
    total_error= 1.0;
    epoch= 0;
    while total_error >= 0.1 && epoch < 1000 {
        total_error= 0.0;
        epoch+= 1;
        for( i, source) in data_set.iter().enumerate(){
            source_a.set(source[0]);
            source_b.set(source[1]);
            neuron[2].activate();
            error= results[i] - neuron[2].output.get();
            neuron[2].weights[0] += error * source[0] * 0.1;
            neuron[2].weights[1] += error * source[1] * 0.1;
            neuron[2].bias += error * 0.1;
            total_error += error.abs();
        }
    }
    neuron[2].input= vec![neuron[0].output.clone() , neuron[1].output.clone()];
    results= vec![0.0, 1.0, 1.0, 0.0];
    println!("Epocs: {}", epoch);
    for( i, source) in data_set.iter().enumerate(){
        source_a.set(source[0]);
        source_b.set(source[1]);
        for j  in 0..neuron.len(){
            neuron[j].activate();
        }
        neuron[2].show();
        println!("Expected Result: {}", results[i]);
    }
}
