/*
    Ekman initial backpropagation example
*/
#include <iostream>
#include <math.h>

// Error function derivative in respect to network output
double error(double y, double f){
    return -(y-f);
}

// Activation functions and their respective  derivatives
double logsig(double x){
    return 1.0/(1.0 + std::exp(-x));
}

double dlogsig(double x){
    double ex = std::exp(x);
    return ex/((ex+1)*(ex+1));
}

double hypertan(double x){
    double ex = std::exp(x);
    double exn = std::exp(-x);
    return (ex-exn)/(ex+exn);
}

double dhypertan(double x){
    double exd = std::exp(2*x);
    return 4*exd/((exd+1)*(exd+1));
}

int main(){
    /*
       Network definitions
    */
    // Inputs
    double x1 = -0.9;
    double x2 = 0.1;

    // Expected output
    double y = 1.0;

    // Weights (Layer G)
    double wxg0 = 0.3;
    double wxg1 = 0.6;
    double wxg2 = -0.1;

    // Weights (Layer F)
    double wgf0 = -0.2;
    double wgf1 = 0.5;

    // Biases - as input extension weigths
    double bg = 1.0;
    double bf = 1.0;

    // Learning rate
    double lr = 0.1;

    /*
        Forward pass
    */
    // Calculating G neuron output
    double zg = wxg0*bg + wxg1*x1 + wxg2*x2;
    double Gzg = hypertan(zg);
    std::cout << "zg: " << zg << std::endl;
    std::cout << "Gzg: " << Gzg << std::endl;

    // Calculating F neuron output
    double zf = wgf0*bf + wgf1*Gzg;
    double Fzf = logsig(zf);
    std::cout << "zf: " << zf << std::endl;
    std::cout << "Fzf: " << Fzf << std::endl;
    std::cout << std::endl;

    // MSE (1/2)(y-f)^2
    std::cout << "MSE: " << (1.0/2.0)*(y-Fzf)*(y-Fzf) << std::endl;

    // Prediction error derivative
    double dmse = -(y-Fzf);
    std::cout << "MSE': " << dmse << std::endl;
    std::cout << std::endl;

    /*
        Backward pass
    */
    // Calculating the erro term for each neuron
    double eft = dmse * dlogsig(zf);
    double egt = eft * wgf1 * dhypertan(zg);
    std::cout << "Error f term: " << eft << std::endl;
    std::cout << "Error g term: " << egt << std::endl;
    std::cout << std::endl;

    // Calculating the gradient for each weight
    double grad_wgf0 = eft*bf;
    double grad_wgf1 = eft*Gzg;
    double grad_wxg0 = egt*bg;
    double grad_wxg1 = egt*x1;
    double grad_wxg2 = egt*x2;
    std::cout << "Grad wxg0: " << grad_wxg0 << std::endl;
    std::cout << "Grad wxg1: " << grad_wxg1 << std::endl;
    std::cout << "Grad wxg2: " << grad_wxg2 << std::endl;
    std::cout << "Grad wgf0: " << grad_wgf0 << std::endl;
    std::cout << "Grad wgf1: " << grad_wgf1 << std::endl;
    std::cout << std::endl;

    // Weight ajdustment
    wxg0 -= lr*grad_wxg0;
    wxg1 -= lr*grad_wxg1;
    wxg2 -= lr*grad_wxg2;
    wgf0 -= lr*grad_wgf0;
    wgf1 -= lr*grad_wgf1;
    std::cout << "New wxg0: " << wxg0 << std::endl;
    std::cout << "New wxg1: " << wxg1 << std::endl;
    std::cout << "New wxg2: " << wxg2 << std::endl;
    std::cout << "New wgf0: " << wgf0 << std::endl;
    std::cout << "New wgf1: " << wgf1 << std::endl; 

    return 0;
}