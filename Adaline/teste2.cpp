#include <iostream>
#include "dataset.h"
#include "adaline.h"

int main(){

    const int neuronios = 5;
    const int dimensao = 10;
    const int quantidade = 10;
    const int maximo_iteracoes = 20;
    const double alpha = 0.05;

    Dataset d(dimensao, quantidade, neuronios);

    Matriz<double>* parametros = d.getParametros();
    Matriz<double>* alvos = d.getCategorias();

    Adaline ada;
    ada.initAdaline(neuronios,dimensao);
    ada.printLayers();
    ada.train(parametros,alvos,maximo_iteracoes,alpha);
    
    std::cout << "Targets: " << std::endl;
    alvos->print();
    std::cout << std::endl;

    ada.printLayers();


    return 0;
}