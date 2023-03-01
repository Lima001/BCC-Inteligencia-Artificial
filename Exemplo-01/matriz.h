#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <assert.h>
#include <vector>

template <class T> class Matriz {

    /*
        Definições básicas
    */

    protected:

        unsigned int n_linha;                       // Número de linhas
        unsigned int n_coluna;                      // Número de colunas

        // Resetar Matriz a estado básico - sem linhas nem colunas
        void limpar(){
            if (ptr != nullptr){
                
                // Desalocação da Memória Dinamicamente alocada
                for (int i=0; i<n_linha; i++){
                    delete ptr[i];
                }

                delete[] ptr;
                ptr = nullptr;
            }
            // Configuração de Estado Básico
            n_linha = 0;
            n_coluna = 0;
        }

        // Alocar memória dinâmica para Matriz
        void alocar(unsigned int n_linha_, unsigned n_coluna_){
            // Desalocação da Memória atual da Matriz - Permite mudança na estrutura da Matriz
            limpar();
            
            n_linha = n_linha_;
            n_coluna = n_coluna_;
            
            // Alocação criando um Ponteiro para Ponteiro (linhas) 
            ptr = new T *[n_linha];

            for (int i=0; i<n_linha; i++){
                // Criação das colunas através do uso de ponteiros
                ptr[i] = new T[n_coluna];

                // Definição de valores default para as colunas
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] = 0;
                }
            }
        }
  
        // Copiar os elementos de uma Matriz
        void copiar(const Matriz<T> &m){
            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] = m.ptr[i][j];
                }
            }
        }

    public:
        
        T **ptr = nullptr;                       //  Ponteiro para Ponteiros  - Linhas e Colunas / Array de Arrays        

        // Construtor default        
        Matriz(){
            alocar(0,0);
        }

        // Construtor default - com parâmetros
        Matriz(unsigned int n_linha, unsigned int n_coluna){
            alocar(n_linha, n_coluna);
        }
        
        // Construtor de cópia
        Matriz(const Matriz<T> &m){
            alocar(m.n_linha, m.n_coluna);
            copiar(m);
        };
        
        // Destrutor
        ~Matriz(){
            limpar();
        }

        // Imprimir/exibir Matriz
        void print(){
            //std::cout << "Linhas: " << n_linha << std::endl;
            //std::cout << "Colunas: " << n_coluna << std::endl;

            for (int i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    std::cout << ptr[i][j] << "\t";
                }
                std::cout << std::endl;
            }
        }

        // Preencher uma linha inteira da Matriz com valores
        void definir_linha(unsigned int index, const std::vector<T> &linha){
            assert(index >= 0 && index < n_linha && linha.size() == n_coluna);
            
            int cont = 0;

            for (T valor : linha){
                ptr[index][cont] = valor;
                cont++;
            }
        }

        /*
            Sobrecarga de Operadores
        */

        // Operador de Atibuição de Adição com Matriz
        Matriz<T>& operator+=(const Matriz<T> &m){
            assert(n_linha == m.n_linha && n_coluna == m.n_coluna);
            
            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] += m.ptr[i][j];
                }
            }
            
            return *this;
        }

        // Operador de Adição com Matriz
        Matriz<T> operator+(const Matriz<T> &m){
            assert(n_linha == m.n_linha && n_coluna == m.n_coluna);
            return Matriz(*this) += m;
        }

        // Operador de Atibuição de Adição com escalar
        Matriz<T>& operator+=(const T escalar){
            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] += escalar;
                }
            }
            
            return *this;
        }

        // Operador de Adição com escalar
        Matriz<T> operator+(const T escalar){
            return Matriz(*this) += escalar;
        }

        // Operador de Atibuição de subtração com Matriz
        Matriz<T>& operator-=(const Matriz<T> &m) {
            assert(n_linha == m.n_linha && n_coluna == m.n_coluna);

            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] -= m.ptr[i][j];
                }
            }
        
            return *this;
        }

        // Operador de subtração com Matriz
        Matriz<T> operator-(const Matriz<T> &m) {
            assert(n_linha == m.n_linha && n_coluna == m.n_coluna);
            return Matriz(*this) -= m;
        }

        // Operador de Atibuição de subtração com escalar
        Matriz<T>& operator-=(const T escalar) {
            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] -= escalar;
                }
            }
        
            return *this;
        }

        // Operador subtração com escalar
        Matriz<T> operator-(const T escalar) {
            return Matriz(*this) -= escalar;
        }

        // Operador de Atibuição de multiplicação com escalar
        Matriz<T>& operator*=(const T escalar){
            for (int i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] *= escalar;
                }
            }

            return *this;
        }

        // Operador de Atibuição de multiplicação com escalar a esquerda
        friend Matriz<T> operator*(const T escalar, const Matriz<T> &m){
		    Matriz a = m;
            a *= escalar;
            
            return a;
	    }

        // Operador de Atibuição de multiplicação com escalar a direita
        friend Matriz operator*(const Matriz<T> &m, const T escalar){
            Matriz a = m;
            a *= escalar;
            
            return a;
	    }

        // Operador de multiplicação com Matriz
        Matriz<T> operator*(const Matriz<T> &m){
            assert(n_coluna == m.n_linha);

            Matriz resultado = Matriz(n_linha, m.n_coluna);

            for (int i=0; i<n_linha; i++){
                for (int j=0; j<m.n_coluna; j++){
                    for (int k=0; k<m.n_linha; k++){
                        resultado[i][j] += ptr[i][k] * m.ptr[k][j];
                    }
                }
            }

            return resultado;
        }

        // Operador de atribuição por cópia
        Matriz<T>& operator=(const Matriz<T> &m){
            alocar(m.n_linha, m.n_coluna); 
            copiar(m);
            
            return *this;
        }

        // Operador de atribuição por movimentação
        Matriz<T>& operator=(const Matriz<T> &&m){
            if (&m == this){
                return *this;
            }
            
            alocar(m.n_linha, m.n_coluna); 
            copiar(m);
            
            return *this;
        }

        // Operador de acesso
        T*& operator[](unsigned int index){
            assert(index >= 0 && index < n_linha);
            return ptr[index];    
        }

        unsigned int nLinha(){
            return n_linha;
        }

        unsigned int nColuna(){
            return n_coluna;
        }
};

#endif