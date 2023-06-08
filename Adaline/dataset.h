#include <cstdlib>
#include <time.h>
#include "matriz.h"

#define UPPER_BOUND 1.f
#define LOWER_BOUND -1.f

class Dataset {

    private:

        Matriz<double> parametros;
        Matriz<double> categorias;

        Matriz<double> pesos;
    
        double hardlim(double x){
            return (double)(x>=0);
        } 

        void gerarDadosDataset(){
            std::srand(time(0));

            // Criar parâmetros aleatórios (com 1 expandido para bias)
            for (int j=0; j<parametros.nColuna(); j++){
                
                int i=0;
                for (i; i<parametros.nLinha()-1; i++)
                    parametros[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
                
                parametros[i][j] = 1;
            }

            // Criação da matriz de pesos "expandida" do modelo
            for (int i=0; i<pesos.nLinha(); i++){
                int j=0;
                for (j; j<pesos.nColuna()-1; j++)
                    pesos[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));

                pesos[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));;
            }
            
            categorias = pesos*parametros;
            for (int j=0; j<categorias.nColuna(); j++){
                for (int i=0; i<categorias.nLinha(); i++)
                    categorias[i][j] = hardlim(categorias[i][j]);
            }
        }

    public:

        Dataset(int dimensao, int quantidade, int neuronios=1){
            parametros = Matriz<double>(dimensao+1, quantidade);
            categorias = Matriz<double>(neuronios, quantidade);
            pesos = Matriz<double>(neuronios, dimensao+1);
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
        }

};