/*
    O exemplo em questão consiste na implementação dos mecanismos
    utilizados por um "Single-Neuron Perceptron" para representar
    3 funções booleanas - Conjunção, Disjunção e Implicação Material.

    Os ajustes da rede foram feitos manualmente - não utiliza-se
    algoritmo de aprendizado - visando compreender e analisar o
    funcionamento de um perceptron simples. Observe que os valores
    podem ser alterados de forma arbitrária, desde que representem
    uma separação correta de R2 em conformidade com cada função
    booleana.

    Obs. Para facilitar a visualização, coloque no plano os pontos
    que representam os parâmetros de entrada, o vetor de pesos e
    a reta de separação de R2.

    Referência: 
    - Hagan, M. T., Demuth, H. B., & Beale, M. H. (2014). Neural Network Design (2nd Edition);
*/

#include "../matriz.h"

double hardlim(double n){
    /*
        hardlim(n) {
            1 se n>=0;
            0 caso contrário;
        }
    */
    return (n>=0);  
}

int main(){

    Matriz<double> W1t = Matriz<double>(1,2);                   // Matriz de pesos transposta - função And (^)
    Matriz<double> W2t = Matriz<double>(1,2);                   // Matriz de pesos transposta - função Or (v)
    Matriz<double> W3t = Matriz<double>(1,2);                   // Matriz de pesos transposta - função Material Implication (->)
    
    Matriz<double> p = Matriz<double>(2,1);                     // Matriz de parâmetros - entradas perceptron

    double b1 = -3;                                             // b específico para função booleana ^
    double b2 = -1;                                             // b específico para função booleana v
    double b3 = 1/2.f;                                             // b específico para função booleana ->


    W1t[0][0] = 2;                                              // Valor de peso arbitrário para W1t
    W1t[0][1] = 2;                                              // Valor de peso arbitrário para W1t

    W2t[0][0] = 2;                                              // Valor de peso arbitrário para W2t
    W2t[0][1] = 2;                                              // Valor de peso arbitrário para W2t

    W3t[0][0] = -1;                                             // Valor de peso arbitrário para W3t
    W3t[0][1] = 1;                                              // Valor de peso arbitrário para W3t


    std::cout << ">>> ";
    std::cin >> p[0][0];                                        // Entrada do 1º parâmetro
    std::cout << ">>> ";
    std::cin >> p[1][0];                                        // Entrada do 2º parâmetro

    std::cout << "hardlim(W1t*p + b1)" << std::endl;
    std::cout << (hardlim((W1t*p)[0][0] + b1)) << std::endl;     // Resultado previsto pelo perceptron função ^

    std::cout << "hardlim(W2t*p + b2)" << std::endl;
    std::cout << (hardlim((W2t*p)[0][0] + b2)) << std::endl;     // Resultado previsto pelo perceptron função v

    std::cout << "hardlim(W3t*p + b3)" << std::endl;
    std::cout << (hardlim((W3t*p)[0][0] + b3)) << std::endl;     // Resultado previsto pelo perceptron função ->

return 0;
} 