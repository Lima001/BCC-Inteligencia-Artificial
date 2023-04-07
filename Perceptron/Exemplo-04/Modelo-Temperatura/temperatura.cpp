/*
    Implementação de um exemplo com dados "práticos" que simulam uma aplicação real
    de SLP's. Dada uma tupla contendo uma temperatura ideal e a temperatura atual,
    a SLP deve decidir se deve-se acionar o sistema de resfriamento, ou aquecimento
    (ou até mesmo, nenhum dos dois - caso as temperaturas sejam iguais). O problema
    visa simular de forma simples a manutenção de uma temperatura em um ambiente controlado.

    Para mais informações sobre os dados utilizados, verifique o arquivo "sobre.txt"
*/

#include <string.h>
#include <fstream>
#include "../lib.h"

Matriz<double> lerMatrizArquivo(std::string caminho_arquivo, int n_linha, int n_coluna){
    Matriz<double> m(n_linha, n_coluna);
    
    std::fstream arquivo(caminho_arquivo, std::ios_base::in);

    for (int i=0; i<n_linha; i++){
        for (int j=0; j<n_coluna; j++){
            arquivo >> m[i][j]; 
        }
    }
    // Use para conferir visualmente se a matriz lida corresponde com o esperado
    //m.print();
    return m;
}

void escreverMatrizArquivo(std::string caminho_arquivo, Matriz<double>* m){
    std::fstream arquivo(caminho_arquivo, std::ios_base::out);

    for (int i=0; i<m->nLinha(); i++){
        for (int j=0; j<m->nColuna(); j++){
            arquivo << (*m)[i][j] << "\t";
        }
        arquivo << std::endl;
    }
}

int main(){

    Matriz<double> parametrosTreino = lerMatrizArquivo("Arquivos/parametrosTreino.txt", 2, 20);
    Matriz<double> classTreino = lerMatrizArquivo("Arquivos/classificacaoTreino.txt", 2, 20);
    Matriz<double> parametrosTeste = lerMatrizArquivo("Arquivos/parametrosTeste.txt", 2, 5);
    Matriz<double> previsoes;
    Matriz<double> validacao;

    SingleLayerPerceptron slp(2,2);
    slp.treinar(&parametrosTreino, &classTreino);
    
    previsoes = slp.prever(&parametrosTeste);
    validacao = slp.prever(&parametrosTreino);

    /*
        Observações:

        a)  Os dados de validação referem-se a predição dos dados que foram usados
            como entrada para o treinamento. Dessa forma, deve-se observar uma precisão
            de 100% na predição.

        b)  Os dados referentes às previsões dos parâmetros de teste podem implicar em
            uma precisão de 100%, uma vez que o treino aplicado pode não ter sido suficiente
            para adequar o modelo.

            Propositalmente, optou-se por um conjunto de dados pequeno para forçar uma precisão
            imperfeita. Altere o exemplo - use mais dados, ou considere dados mais concisos - para
            ver como o treino do modelo varia.
    */
    escreverMatrizArquivo("Arquivos/previsao.txt", &previsoes);
    escreverMatrizArquivo("Arquivos/validacao.txt", &validacao);
    
    return 0;
}