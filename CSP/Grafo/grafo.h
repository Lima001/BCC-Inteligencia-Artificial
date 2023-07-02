#ifndef GRAFO_H
#define GRAFO_H

#include "componentesGrafo.h"
#include "matriz.h"

typedef struct {
    std::shared_ptr<Vertice> v1 = nullptr;
    std::shared_ptr<Vertice> v2 = nullptr;
} ParVertice;


class Grafo {

    public:

        int ordem;
        int tamanho;
        bool dirigido;
        ListaEncadeada<Vertice> lista_vertices;

        Grafo(bool dirigido=false):
            ordem(0), tamanho(0), dirigido(dirigido){
        }

        ~Grafo(){
            No<Vertice> *tmp = lista_vertices.inicio;
            
            while (tmp){
                tmp = tmp->proximo;
                lista_vertices.inicio->elemento->~Vertice();
                
                delete lista_vertices.inicio;
                
                lista_vertices.inicio = tmp;
            }
        }

        int getOrdem(){
            return ordem;
        }

        int getTamanho(){
            return tamanho;
        }
        
        ListaEncadeada<Vertice> vertices(){
            ListaEncadeada<Vertice> l;
            No<Vertice> *nv;

            for (nv=lista_vertices.inicio; nv; nv=nv->proximo)
                l.inserirNo(nv->elemento);

            return l;
        }

        ListaEncadeada<Aresta> arestas(){
            ListaEncadeada<Aresta> l;
            std::unordered_map<int, bool> hash;

            No<Vertice> *nv;
            No<Aresta> *na;

            for (nv=lista_vertices.inicio; nv; nv=nv->proximo){
                for (na=nv->elemento->arestas_entrada.inicio; na; na=na->proximo){
                    if (hash.find(na->elemento->id) == hash.end()){
                        l.inserirNo(na->elemento);
                        hash[na->elemento->id] = true;
                    }
                }
            }

            return l;
        }
        
        void insereV(){
            std::shared_ptr<Vertice> novo_vertice = std::make_shared<Vertice>(Vertice(ordem));
            lista_vertices.inserirNo(novo_vertice);
            ordem++;
        }

        // Usado para 'copiar' a estrutura base de um vertice - não considera arestas
        void insereCopiaV(std::shared_ptr<Vertice> v){
            std::shared_ptr<Vertice> novo_vertice = std::make_shared<Vertice>(Vertice(v->id));
            lista_vertices.inserirNo(novo_vertice);
            ordem++;
        }
        
        void insereA(int u, int j, int custo=0){
            std::shared_ptr<Vertice> vu = getV(u);
            std::shared_ptr<Vertice> vj = getV(j);

            std::shared_ptr<Aresta> nova_aresta = std::make_shared<Aresta>(Aresta(tamanho, vu, vj, custo));
            
            if (dirigido){
                vu->arestas_saida.inserirNo(nova_aresta);
                vj->arestas_entrada.inserirNo(nova_aresta);
            }
            else {
                vu->arestas_entrada.inserirNo(nova_aresta);
                vj->arestas_entrada.inserirNo(nova_aresta);
            }

            tamanho++;
        }

        ListaEncadeada<Vertice> adj(int id_vertice){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);
            ListaEncadeada<Vertice> l;

            No<Aresta> *na;

            if (dirigido){
                for (na=v->arestas_saida.inicio; na; na=na->proximo)
                    l.inserirNo(na->elemento->v2);
            }
            
            else {
                
                for (na=v->arestas_entrada.inicio; na; na=na->proximo){
                    if (na->elemento->v1->id == id_vertice)
                        l.inserirNo(na->elemento->v2);
                    else if (na->elemento->v2->id == id_vertice)
                        l.inserirNo(na->elemento->v1);
                }

            }

            return l;
        }

        std::shared_ptr<Aresta> getA(int id_v1, int id_v2){
            std::shared_ptr<Vertice> v1 = lista_vertices.getById(id_v1);
            No<Aresta> *na;

            if (dirigido){
                
                for (na=v1->arestas_saida.inicio; na; na=na->proximo){
                    if (na->elemento->v2->id == id_v2)
                        return na->elemento;
                }
                
                return nullptr;
            }

            else {
                
                for (na=v1->arestas_entrada.inicio; na; na=na->proximo){
                    if (na->elemento->v1->id == id_v2 || na->elemento->v2->id == id_v2)
                        return na->elemento;
                }

                return nullptr;
            }
        }

        std::shared_ptr<Vertice> getV(int id_vertice){
            return lista_vertices.getById(id_vertice);
        }
        
        int grauE(int id_vertice){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);
            return v->arestas_entrada.tamanho;
        }

        int grauS(int id_vertice){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);

            if (dirigido)
                return v->arestas_saida.tamanho;
            else
                return v->arestas_entrada.tamanho;
        }
        
        ParVertice verticesA(int id_aresta){
            ParVertice pv;
            ListaEncadeada<Aresta> la = arestas();

            No<Aresta> *na;

            for (na=la.inicio; na; na=na->proximo){
                if (na->elemento->id == id_aresta){
                    pv.v1 = na->elemento->v1;
                    pv.v2 = na->elemento->v2;
                    break;
                }
            }

            return pv;
        }
        
        std::shared_ptr<Vertice> oposto(int id_vertice, int id_aresta){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);
            std::shared_ptr<Aresta> a = arestas().getById(id_aresta);
            
            if (a->v1->id == id_vertice)
                return a->v2;
            else if (a->v2->id == id_vertice)
                return a->v2;
            
            assert(false);
        }
        
        ListaEncadeada<Aresta>& arestasE(int id_vertice){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);
            return v->arestas_entrada;
        }

        ListaEncadeada<Aresta>& arestasS(int id_vertice){
            std::shared_ptr<Vertice> v = lista_vertices.getById(id_vertice);
            
            if (dirigido)
                return v->arestas_saida;
            else
                return v->arestas_entrada;
        }

        void print(){
            No<Vertice> *nv;
            No<Aresta> *na;

            std::cout << "<<< Grafo >>>" << std::endl;

            for (nv=lista_vertices.inicio; nv; nv=nv->proximo){
                std::cout << "Vertice " << nv->elemento->id << std::endl;
                
                if (dirigido){
                    
                    std::cout << " Arestas de Saida" << std::endl;
                    
                    for (na=nv->elemento->arestas_saida.inicio; na; na=na->proximo)
                        std::cout << "->\t" << *(na->elemento) << std::endl;
                    
                    std::cout << " Arestas de Entrada" << std::endl;

                }

                for (na=nv->elemento->arestas_entrada.inicio; na; na=na->proximo)
                    std::cout << "->\t" << *(na->elemento) << std::endl;
            }
        }

        Matriz<int> getMatrizAdj(){
            Matriz<int> m = Matriz<int>(ordem,ordem,0);
            
            if (dirigido){
                for (Aresta &a: arestas())
                    m[a.v1->id][a.v2->id]++;
            }
            else {
                for (Aresta &a: arestas()){
                    m[a.v1->id][a.v2->id]++;
                    m[a.v2->id][a.v1->id]++;
                }
            }
            
            return m;
        }

        Matriz<double> getMatrizCusto(){
            Matriz<double> m = Matriz<double>(ordem,ordem,std::numeric_limits<double>::infinity());

            for (int i=0; i<ordem; i++)
                m[i][i] = 0;

            if (dirigido){
                for (Aresta &a: arestas())
                        m[a.v1->id][a.v2->id] = a.custo;
            }
            
            else {
                for (Aresta &a: arestas()){
                    m[a.v1->id][a.v2->id] = a.custo;
                    m[a.v2->id][a.v1->id] = a.custo;
                }
            }
            
            return m;
        }

};

#endif