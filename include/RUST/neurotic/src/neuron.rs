use std::cell::Cell;
use std::rc::Rc;
use crate::activation::ActivationList;

pub struct Neuron{
    pub input: Vec<Rc<Cell<f32>>>,
    pub weights: Vec<f32>,
    pub bias: f32,
    pub activation: ActivationList,
    pub output: Rc<Cell<f32>>,
}

impl Neuron {
    pub fn new(inputs: usize , function: ActivationList) -> Neuron {
        Neuron {
            input: vec![],
            weights: vec![0.0; inputs],
            output: Rc::new(Cell::new(0.0)),
            bias: 0.0,
            activation: function,
        }
    }

    pub fn activate(&mut self) {
        let sum = self.input.iter().zip(self.weights.iter())
        .map(|(i, w)| i.get() * w)
        .sum::<f32>() + self.bias;
        self.output.set(ActivationList::functions(&self.activation).0(sum).get());
    }

    pub fn show(&self) {
        println!("Neuron Input: {:?}", self.input);
        println!("Neuron Weights: {:?}", self.weights);
        println!("Neuron Bias: {}", self.bias);
        println!("Neuron Output: {}", self.output.get());
    }
}