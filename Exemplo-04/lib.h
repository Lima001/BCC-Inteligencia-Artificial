/*
    Adaptação do Exemplo-03 para SLP com mais de um neurônio.
*/

#include <cstdlib>
#include <time.h>
#include "../matriz.h"

#define UPPER_BOUND 1000
#define LOWER_BOUND -1000

class Dataset {

    private:

        Matriz<double> parametros;
        Matriz<double> categorias;

        Matriz<double> pesos;
        Matriz<double> bias;
    
        double hardlim(double x){
            return (double)(x>=0);
        } 

        void gerarDadosDataset(){
            std::srand(time(0));

            for (int j=0; j<parametros.nColuna(); j++){
                for (int i=0; i<parametros.nLinha(); i++)
                    parametros[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            }

            for (int i=0; i<pesos.nLinha(); i++){
                for (int j=0; j<pesos.nColuna(); j++)
                    pesos[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));

                bias[i][0] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            }
            
            categorias = pesos*parametros;
            for (int j=0; j<categorias.nColuna(); j++){
                for (int i=0; i<categorias.nLinha(); i++)
                    categorias[i][j] = hardlim(categorias[i][j]+bias[i][0]);
            }
        }

    public:

        Dataset(int dimensao, int quantidade, int neuronios=1){
            parametros = Matriz<double>(dimensao, quantidade);
            categorias = Matriz<double>(neuronios, quantidade);
            pesos = Matriz<double>(neuronios, dimensao);
            bias = Matriz<double>(neuronios, 1);
            gerarDadosDataset();
        }

        Matriz<double>* getParametros(){
            return &parametros;
        }

        Matriz<double>* getCategorias(){
            return &categorias;
        }

        void printDataset(){
            std::cout << "Parâmetros:" << std::endl;
            parametros.print();
            std::cout << std::endl;
            std::cout << "Categorias:" << std::endl;
            categorias.print();
            std::cout << std::endl;
        }

        void printModelo(){
            std::cout << "Pesos:" << std::endl;
            pesos.print();
            std::cout << std::endl;
            std::cout << "Bias:" << std::endl;
            bias.print();
            std::cout << std::endl;
        }

};

class SingleLayerPerceptron {

    private:

        int iteracoes;
        
        Matriz<double> pesos;
        Matriz<double> bias;

        double hardlim(double x){
            return (double)(x>=0);
        } 

    public:

        SingleLayerPerceptron(int dimensao, int neuronios=1){
            pesos = Matriz<double>(neuronios,dimensao);
            bias = Matriz<double>(neuronios,1);

            std::srand(time(0));
            
            for (int i=0; i<neuronios; i++){
                for (int j=0; j<dimensao; j++)
                    pesos[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            
                bias[i][0] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            }
        }

        void treinar(Matriz<double>* parametros, Matriz<double>* alvos){
            //assert(parametros->nLinha() == pesos.nColuna() && parametros->nColuna() == alvos->nColuna());

            double soma;
            
            double a, e;

            bool mudou;   
            bool executar = true;
            
            iteracoes = 0;
            
            while (executar){
                mudou = false;
                iteracoes++;

                for (int j=0; j<parametros->nColuna(); j++){
                    for (int i=0; i<pesos.nLinha(); i++){
                        
                        soma = 0.0;
                        
                        for (int k=0; k<pesos.nColuna(); k++)
                            soma += pesos[i][k] * (*parametros)[k][j];
                        
                        a = hardlim(soma+bias[i][0]);
                        
                        e = (*alvos)[i][j]-a;

                        if (e != 0){
                            
                            for (int k=0; k<pesos.nColuna(); k++)
                                pesos[i][k] +=  e* (*parametros)[k][j];
                            
                            bias[i][0] += e-a;
                            
                            mudou = true;
                        }
                    }
                }

                if (!mudou)
                    break;
            }
        }

        Matriz<double> prever(Matriz<double>* parametros){
            //assert(parametros->nLinha() == pesos.nColuna());

            Matriz<double> previsoes = pesos * (*parametros);

            for (int j=0; j<previsoes.nColuna(); j++){       
                for (int i=0; i<previsoes.nLinha(); i++)
                    previsoes[i][j] = hardlim(previsoes[i][j]+bias[i][0]);
            
            }
            
            return previsoes;
        }

        void print(){
            std::cout << "Pesos:" << std::endl;
            pesos.print();
            std::cout << std::endl;
            std::cout << "Bias:" << std::endl;
            bias.print();
            std::cout << std::endl;
        }

        int getIteracoes(){
            return iteracoes;
        }

};