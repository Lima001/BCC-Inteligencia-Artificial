/*
    Programa de testes adaptado do Exemplo-03 para considerar SLP com mais de um neurônio
*/

#include "lib.hr"

int main(){

    int neuronios = 5;
    int dimensao = 10;
    int quantidade = 100;

    Dataset d(dimensao, quantidade, neuronios);
    std::cout << "<<Dataset>>" << std::endl;
    d.printDataset();
    d.printModelo();
    
    SingleLayerPerceptron slp(dimensao, neuronios);

    Matriz<double>* parametros = d.getParametros();
    Matriz<double>* alvos = d.getCategorias();

    slp.treinar(parametros, alvos);

    std::cout << "<<Preivisão dos parâmetros>>" << std::endl;
    slp.prever(parametros).print();
    std::cout <<std::endl;

    std::cout << "<<SLP Treinada>>" << std::endl;
    slp.print();
    std::cout << "Iterações performadas: " << slp.getIteracoes() << std::endl;

    return 0;
}