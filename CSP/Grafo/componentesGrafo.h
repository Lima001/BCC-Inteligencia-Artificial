#ifndef COMPONENTES_GRAFO_H
#define COMPONENTES_GRAFO_H

#include <unordered_map>
#include "listaEncadeada.h"

class Vertice{

    public:

        int id;
        ListaEncadeada<Aresta> arestas_entrada;
        ListaEncadeada<Aresta> arestas_saida;
        
        int cor;
        std::shared_ptr<Vertice> predecessor;
        int distancia;
        int tempo_descoberta = 0;
        int tempo_fechamento = 0;
        int rank;
        double chave;
        bool visitado = false;

        Vertice(int id=-1):
            id(id){
        }

        ~Vertice(){
            arestas_entrada.~ListaEncadeada();
            arestas_saida.~ListaEncadeada();
            predecessor = nullptr;
        }

        friend std::ostream& operator<<(std::ostream &out, const Vertice &v){
            out << "Vertice: " << v.id << " - "
                << "End. Memória: " << &v;
            return out;
        }

};


class Aresta {

    public:

        int id;
        std::shared_ptr<Vertice> v1;
        std::shared_ptr<Vertice> v2;

        // Arestas de grafos não valorados possuem custo definido por padrão para 0.
        // Deixei na classe de Aresta esse atributo pelos mesmos motivos
        // citados anteriormente na classe Vertice.
        double custo;
        bool visitada;

        Aresta(int id, std::shared_ptr<Vertice> v1=nullptr, std::shared_ptr<Vertice> v2=nullptr, double custo=0, bool visitada=false):
            id(id), v1(v1), v2(v2), custo(custo), visitada(visitada){
        }

        ~Aresta(){
            v1.reset();
            v2.reset();
        }

        friend std::ostream& operator<<(std::ostream &out, const Aresta &a){
            out << "Aresta: " << a.id 
                << " (" << a.v1->id << "," 
                << a.v2->id << ")";
            return out;
        }

};

#endif