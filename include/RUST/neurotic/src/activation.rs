use std::cell::Cell;

#[allow(dead_code)]
fn activ_step( x: f32 ) -> Cell<f32> {
    if x >= 0.0 { 1.0 } else { 0.0 }.into()
}
#[allow(dead_code)]
fn activ_step_d( _x: f32 ) -> Cell<f32> {
    1.0.into()
}

#[allow(dead_code)]
fn activ_relu( x: f32 ) -> Cell<f32> {
    if x >= 0.0 { x } else { 0.0 }.into()
}
#[allow(dead_code)]
fn activ_relu_d( x: f32 ) -> Cell<f32> {
    if x >= 0.0 { 1.0 } else { 0.0 }.into()
}

#[allow(dead_code)]
fn activ_sigmoid( x: f32 ) -> Cell<f32> {
    (1.0 / (1.0 + (-x).exp())).into()
}
#[allow(dead_code)]
fn activ_sigmoid_d( x: f32 ) -> Cell<f32> {
    let fx = 1.0 / (1.0 + (-x).exp());
    (fx * (1.0 - fx)).into()
}

#[derive(Copy, Clone, Debug)]
pub enum ActivationList {
    #[allow(dead_code)]
    Step,
    #[allow(dead_code)]
    ReLU,
    #[allow(dead_code)]
    Sigmoid,
}
impl ActivationList {
    pub fn functions(&self) -> (fn(f32) -> Cell<f32>, fn(f32) -> Cell<f32>) {
        match self {
            ActivationList::ReLU => (activ_relu, activ_relu_d),
            ActivationList::Step => (activ_step, activ_step_d),
            ActivationList::Sigmoid => (activ_sigmoid, activ_sigmoid_d),
        }
    }
}