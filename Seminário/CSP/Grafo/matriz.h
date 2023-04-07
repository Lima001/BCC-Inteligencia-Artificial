#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <assert.h>
#include <vector>
#include <limits>

template <typename T>
class Matriz {
    
    protected:

        unsigned int n_linha;
        unsigned int n_coluna;

        // Reseta a estrutura da matriz
        void limpar(){
            
            if (ptr != nullptr){

                for (int i=0; i<n_linha; i++){
                    delete ptr[i];
                }

                delete[] ptr;
                ptr = nullptr;
            }

            n_linha = 0;
            n_coluna = 0;

        }

        // Aloca memória dinâmica e preenche a matriz com o valor do parâmetro vdefault
        void alocar(unsigned int n_linha_, unsigned n_coluna_, T vdefault){

            limpar();
            
            n_linha = n_linha_;
            n_coluna = n_coluna_;
            
            // Alocação dinâmica da Memória criando um Ponteiro para Ponteiro (Interprete esse ponteiro como o array de linhas) 
            ptr = new T *[n_linha];

            for (int i=0; i<n_linha; i++){
                // Criação das colunas através do uso de ponteiros
                ptr[i] = new T[n_coluna];

                // Definição de valores default para as colunas
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] = vdefault;
                }
            }
        }
        
        void copiar(const Matriz &m){
            for (int  i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    ptr[i][j] = m.ptr[i][j];
                }
            }
        }

    public:
        
        T **ptr = nullptr;

        Matriz(){
            alocar(0,0,0);
        }

        Matriz(unsigned int n_linha, unsigned int n_coluna, T vdefault){
            alocar(n_linha, n_coluna, vdefault);
        }
        
        Matriz(const Matriz<T> &m){
            alocar(m.n_linha, m.n_coluna,0);
            copiar(m);
        };
        
        ~Matriz(){
            limpar();
        }

        void print(){
            std::cout << "Linhas: " << n_linha << std::endl;
            std::cout << "Colunas: " << n_coluna << std::endl;

            for (int i=0; i<n_linha; i++){
                for (int j=0; j<n_coluna; j++){
                    std::cout << ptr[i][j] << "\t";
                }
                std::cout << std::endl;
            }
        }

        Matriz& operator=(const Matriz<T> &m){
            alocar(m.n_linha, m.n_coluna,0); 
            copiar(m);
            
            return *this;
        }

        Matriz& operator=(const Matriz<T> &&m){
            if (&m == this){
                return *this;
            }
            
            alocar(m.n_linha, m.n_coluna,0); 
            copiar(m);
            
            return *this;
        }

        T*& operator[](int index){
            assert(index >= 0 && index <= n_linha);
            return ptr[index];    
        }

};

#endif