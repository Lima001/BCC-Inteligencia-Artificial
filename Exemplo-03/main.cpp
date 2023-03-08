#include <cstdlib>
#include <time.h>
#include "../matriz.h"

// Define os limites utilizados para configurar os valores de parâmetros, pesos e bias
#define UPPER_BOUND 1000
#define LOWER_BOUND -1000

/*
    Classe para gerar e representar um conjunto de dados. Usada para treinar um perceptron
*/
class Dataset {

    private:

        // Conjunto de dados
        Matriz<double> parametros;
        Matriz<double> categorias;

        // Modelo utilizado para gerar os dados
        Matriz<double> pesos;
        double bias;
    
        // Função de ativição do modelo
        double hardlim(double x){
            return (double)(x>=0);
        } 

        void gerarDadosDataset(){
            // Configura uma seed para a chamada da função rand()
            std::srand(time(0));

            // Geração de parâmetros aleatórios
            for (int j=0; j<parametros.nColuna(); j++){
                for (int i=0; i<parametros.nLinha(); i++)
                    parametros[i][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            }

            // Geração de pesos aleatorios
            pesos = Matriz<double>(1,parametros.nLinha());
            for (int j=0; j<pesos.nColuna(); j++)
                pesos[0][j] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));

            // Geração de um bias aleatório
            bias = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));

            // Computa as categorias dos parâmetros gerados com base no modelo aleatório criado
            categorias = pesos*parametros;
            for (int j=0; j<categorias.nColuna(); j++)
                categorias[0][j] = hardlim(categorias[0][j]+bias);
        }

    public:

        // Construtor - Recebe a dimensão dos parâmetros e a quantidade de elementos do dataset
        Dataset(double dimensao, double quantidade){
            parametros = Matriz<double>(dimensao, quantidade);
            categorias = Matriz<double>(1, quantidade);
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
            std::cout << bias << std::endl;
        }

        /*
            TODO: Criar um método que utiliza o modelo do dataset
            para criar novos parâmetros que possam ser usados para
            previsão no perceptron
        */

};

/* 
    Classe usada para representar um perceptron de única camada. Dada um conjunto de dados
    (Dataset), é possível treinar o perceptron e depois realizar previsões para classificação
    de dados
*/
class SingleLayerPerceptron {

    private:

        // Quantidade de iterações que o modelo levou para treinar no dataset
        int iteracoes;
        
        // Definição do modelo
        Matriz<double> pesos;
        double bias;

        // Função ativadora do modelo        
        double hardlim(double x){
            return (double)(x>=0);
        } 

    public:

        // Construtor - recebe a dimensão dos dados que a rede aceita (número de inputs)
        SingleLayerPerceptron(int dimensao){
            pesos = Matriz<double>(1,dimensao);
            
            // Configuração de uma seed para a função rand()
            std::srand(time(0));
            
            // Inicializa o modelo de forma aleatória
            for (int i=0; i<dimensao; i++)
                pesos[0][i] = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
            
            bias = LOWER_BOUND+static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(UPPER_BOUND-LOWER_BOUND)));
        }

        void treinar(Matriz<double>* parametros, Matriz<double>* alvos){
            assert(parametros->nLinha() == pesos.nColuna() && parametros->nColuna() == alvos->nColuna());

            // Armazena o produto interno dos pesos por um parâmetro
            double soma;
            
            // Variáveis utilziadas para armazenar predição e erro
            double a, e;

            // Variáveis lógicas de controle
            bool mudou;             // Armazena a existência de mudanças no modelo após uma iteração de treino
            bool executar = true;   // Controla o laço de treino
            
            iteracoes = 0;
            
            // Treinamento do perceptron
            while (executar){
                mudou = false;
                iteracoes++;

                // Itera sobre os parâmetros
                for (int i=0; i<parametros->nColuna(); i++){
                    
                    soma = 0.0;
                    
                    // Produto interno peso*parâmetro
                    for (int j=0; j<pesos.nColuna(); j++)
                        soma += pesos[0][j] * (*parametros)[j][i];
                    
                    // Previsão
                    a = hardlim(soma+bias);
                    
                    // Erro
                    e = (*alvos)[0][i]-a;

                    // Atualização do modelo em caso de erro
                    if (e != 0){
                        
                        for (int j=0; j<pesos.nColuna(); j++)
                            pesos[0][j] +=  e* (*parametros)[j][i];
                        
                        bias += e-a;
                        
                        // Modelo foi atualizado
                        mudou = true;
                    }
                }
                /*
                    Se após uma iteração o modelo não foi modificado, quebra-se o loop, pois
                    o treino já foi realizado e o modelo ajustado 
                */
                if (!mudou)
                    break;
            }
        }

        // Dada uma matriz de parâmetros (de mesma dimensão usada para treinar o modelo)
        Matriz<double> prever(Matriz<double>* parametros){
            assert(parametros->nLinha() == pesos.nColuna());

            // Matriz contendo inicialmente o produto interno entre cada parâmetro e o peso do modelo
            Matriz<double> previsoes = pesos * (*parametros);

            // Cálculo de predição do modelo
            for (int j=0; j<previsoes.nColuna(); j++)       
                previsoes[0][j] = hardlim(previsoes[0][j]+bias);
            
            return previsoes;
        }

        void print(){
            std::cout << "Pesos:" << std::endl;
            pesos.print();
            std::cout << std::endl;
            std::cout << "Bias:" << std::endl;
            std::cout << bias << std::endl;
        }

        int getIteracoes(){
            return iteracoes;
        }

};

int main(){

    // Configurações do programa
    int dimensao = 5;
    int quantidade = 1000;

    // Criar dataset
    Dataset d(dimensao, quantidade);
    std::cout << "<<Dataset>>" << std::endl;
    d.printDataset();
    d.printModelo();
    
    // Definir um perceptron capaz de processar o dataset
    SingleLayerPerceptron slp(dimensao);

    // Dados de treino - gerados e armazenados pelo dataset
    Matriz<double>* parametros = d.getParametros();
    Matriz<double>* alvos = d.getCategorias();

    // Treinamento do perceptron
    slp.treinar(parametros, alvos);
    
    /*
        Previsão usando os próprios parâmetros de treino - Deve-se obter 100% de precisão,
        caso contrário, o processo de treinamento foi feito de forma incorreta
    */
    std::cout << "<<Preivisão dos parâmetros>>" << std::endl;
    slp.prever(parametros).print();
    std::cout <<std::endl;

    // Exibir o modelo treinado
    std::cout << "<<SLP Treinada>>" << std::endl;
    slp.print();
    std::cout << "Iterações performadas: " << slp.getIteracoes() << std::endl;

    return 0;
}