/*
    XOR function mapping 
*/
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>

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
        Random number generation definitions
    */
    double lowerBound = -1.5;
    double upperBound = 1.5;
    std::srand(time(0));

    /*
       Network definitions
    */
    // Inputs
    double x1[] = {0.0, 0.0, 1.0, 1.0};
    double x2[] = {0.0, 1.0, 0.0, 1.0};

    // Expected output
    double y []= {0.0, 1.0, 1.0, 0.0};

    // Weights (Neuron F)
    double wxf0 = 0.0;
    double wxf1 = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));
    double wxf2 = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));

    // Weights (Neuron G)
    double wxg0 = 0.0;
    double wxg1 = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));
    double wxg2 = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));

    // Weights (Neuron H)
    double wh0 = 0.0;
    double wfh = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));
    double wgh = lowerBound+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(upperBound-lowerBound)));

    std::cout << "Original wxf0: " << wxf0 << std::endl;
    std::cout << "Original wxf1: " << wxf1 << std::endl;
    std::cout << "Original wxf2: " << wxf2 << std::endl;
    std::cout << "Original wxg0: " << wxg0 << std::endl;
    std::cout << "Original wxg1: " << wxg1 << std::endl;
    std::cout << "Original wxg2: " << wxg2 << std::endl;
    std::cout << "Original wh0: " << wh0 << std::endl;
    std::cout << "Original wfh: " << wfh << std::endl;
    std::cout << "Original wgh: " << wgh << std::endl;
    std::cout << std::endl;

    // Biases - as input extension weigths
    double bg = 1.0;
    double bf = 1.0;
    double bh = 1.0;

    // Learning rate
    double lr = 0.001;

    // Maximum iterations
    int maxIteration = 100000000;

    // Loop internal variable definitions
    int i, j;
    double zg, Gzg, zf, Fzf, zh, Hzh, dmse, eft, egt, eht;
    double  grad_wxf0, grad_wxf1, grad_wxf2, 
            grad_wxg0, grad_wxg1, grad_wxg2, 
            grad_wh0, grad_wfh, grad_wgh;

    for (i=0; i<maxIteration; i++){
        // Iterating over the training examples
        for (j=0; j<4; j++){
            /*
                Forward pass
            */
            // Calculating G neuron output
            zg = wxg0*bg + wxg1*x1[j] + wxg2*x2[j];
            Gzg = hypertan(zg);

            // Calculating F neuron output
            zf = wxf0*bf + wxf1*x1[j] + wxf2*x2[j];
            Fzf = hypertan(zf);

            // Calculating H neuron output
            zh = wh0*bh + wfh*Fzf + wgh*Gzg;
            Hzh = logsig(zh);

            // MSE (1/2)(y-h)^2
            //std::cout << "(" << i << "," << j << ") - MSE: " << (1.0/2.0)*(y[j]-Hzh)*(y[j]-Hzh) << std::endl;

            // Prediction error derivative
            dmse = -(y[j]-Hzh);

            /*
                Backward pass
            */
            // Calculating the erro term for each neuron
            eht = dmse * dlogsig(zh);
            eft = eht * wfh * dhypertan(zf);
            egt = eht * wgh * dhypertan(zg);


            // Calculating the gradient for each weight
            grad_wh0 = eht*bh; 
            grad_wfh = eht * Fzf; 
            grad_wgh = eht * Gzg;
            grad_wxf0 = eft * bf;
            grad_wxf1 = eft * x1[j];
            grad_wxf2 = eft * x2[j]; 
            grad_wxg0 = egt * bg; 
            grad_wxg1 = egt * x1[j]; 
            grad_wxg2 = egt * x2[j]; 

            // Weight ajdustment
            wxf0 -= lr * grad_wxf0;
            wxf1 -= lr * grad_wxf1;
            wxf2 -= lr * grad_wxf2;
            wxg0 -= lr * grad_wxg0;
            wxg1 -= lr * grad_wxg1;
            wxg2 -= lr * grad_wxg2;
            wh0 -= lr * grad_wh0;
            wfh -= lr * grad_wfh;
            wgh -= lr * grad_wgh;
        }
    }

    std::cout << "New wxf0: " << wxf0 << std::endl;
    std::cout << "New wxf1: " << wxf1 << std::endl;
    std::cout << "New wxf2: " << wxf2 << std::endl;
    std::cout << "New wxg0: " << wxg0 << std::endl;
    std::cout << "New wxg1: " << wxg1 << std::endl;
    std::cout << "New wxg2: " << wxg2 << std::endl;
    std::cout << "New wh0: " << wh0 << std::endl;
    std::cout << "New wfh: " << wfh << std::endl;
    std::cout << "New wgh: " << wgh << std::endl;
    std::cout << std::endl;

    // Visualizing the prediction for the training examples
    for (j=0; j<4; j++){
        std::cout << "x1: " << x1[j] << "; " << "x2: " << x2[j] << std::endl;
        std::cout << "Target: " << y[j] << std::endl;
        std::cout << "Prediction: " 
            <<  logsig(
                        wh0
                        +wfh*hypertan(wxf0+wxf1*x1[j]+wxf2*x2[j])
                        +wgh*hypertan(wxg0+wxg1*x1[j]+wxg2*x2[j])
                    ) 
            << std::endl;
    }

    return 0;
}