#ifndef ADALINE_H
#define ADALINE_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "matriz.h"

#define UPPER_BOUND_ADALINE 10.f
#define LOWER_BOUND_ADALINE -10.f

void linear(Matriz<double>* in, Matriz<double>* out){
    for (int i=0; i<in->nLinha(); i++)
        (*out)[i][0] = (*in)[i][0];
}

class Adaline {

    private:

        void clean(){
            if (weights){
                delete weights;
                weights = nullptr;
            }
        }

    public:

        Matriz<double>* weights;

        Adaline(){
            weights = nullptr;
        }

        ~Adaline(){
            clean();
        }

        void initAdaline(int n, int pn){
            clean();
            weights = new Matriz<double>(n,pn+1);
            std::srand(time(0));

            for (int j=0; j<n; j++){
                for (int k=0; k<pn+1; k++){
                    (*weights)[j][k] = LOWER_BOUND_ADALINE+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND_ADALINE-LOWER_BOUND_ADALINE)));
                }
            }
        }

        void predict(Matriz<double>* p, Matriz<double>* t, Matriz<double>* out){
            *out = (*weights) * (*p);
            linear(out,out);
        }

        void estimateError(Matriz<double>* predicted, Matriz<double>* t, Matriz<double>* out){
            *out = *t - *predicted;
        }

        void train(Matriz<double>* p, Matriz<double>* t, int ni, double alpha){
            Matriz<double> out(t->nLinha(), t->nColuna());
            Matriz<double> error(t->nLinha(), t->nColuna());

            for (int i=0; i<ni; i++){
                std::cout << i << std::endl;
                std::cout << "Predicted " << std::endl;
                predict(p,t,&out);
                out.print();
                estimateError(&out,t,&error);
                std::cout << "Error " << std::endl;
                error.print();
                std::cout << std::endl;
                *weights += (2*alpha*error)*(p->transpose());
            }
            
        }

        void printLayers(){
            std::cout << "Weights and biasses: " << std::endl;
            weights->print();
        }
        
};

#endif