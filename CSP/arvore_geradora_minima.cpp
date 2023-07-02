#include "Grafo/grafo.h"

// Implementação Estrutura de conjuntos
void makeSet(std::shared_ptr<Vertice> v){
    v->predecessor = v;
}

std::shared_ptr<Vertice> findSet(std::shared_ptr<Vertice> v){    
    if (v->predecessor != v)
        v->predecessor = findSet(v->predecessor);
    
    return v->predecessor;
}

void link(std::shared_ptr<Vertice> x, std::shared_ptr<Vertice> y){

    if (x->rank > y->rank)
        y->predecessor = x;
    
    else {
        x->predecessor = y;
        
        if (x->rank == y->rank)
            y->rank++; 
    }
}

void setUnion(std::shared_ptr<Vertice> x, std::shared_ptr<Vertice> y){
    link(findSet(x),findSet(y));
}

// Implementação do algoritmo de Kruskal
Grafo kruskal(Grafo *g){
    Grafo arvore;
    
    No<Vertice> *nv = g->lista_vertices.inicio;
    
    for (nv; nv; nv=nv->proximo){
        makeSet(nv->elemento);
        arvore.insereCopiaV(nv->elemento);
    }
    
    ListaEncadeada<Aresta> la = g->arestas();
    ListaEncadeada<Aresta> la_ordenada;
    
    No<Aresta> *na = la.inicio;
    No<Aresta> *na_menor;
    double menor;

    for (int i=0; i<la.tamanho; i++){

        menor = std::numeric_limits<double>::infinity();

        for (na=la.inicio; na; na=na->proximo){
            if (!na->elemento->visitada && na->elemento->custo < menor){
                menor = na->elemento->custo;
                na_menor = na;
            }
        }

        la_ordenada.inserirNo(na_menor->elemento);
        na_menor->elemento->visitada = true;
    }

    na = la_ordenada.inicio;

    for (na; na; na=na->proximo){
        if (findSet(na->elemento->v1) != findSet(na->elemento->v2)){
            setUnion(na->elemento->v1, na->elemento->v2);
            arvore.insereA(na->elemento->v1->id,na->elemento->v2->id);
        }
    }

    return arvore;
}

int main(){

    // Criação do grafo
    Grafo g = Grafo();

    // Inserção de vértices
    for (int i=0; i<9; i++)
        g.insereV();

    /*
        Mapeamento lógico dos vértices do exemplo para os vértices do grafo G
        a -> V0
        b -> V1
        c -> V2
        d -> V3
        e -> V4
        f -> V5
        g -> V6
        h -> V7
        i -> V8
    */
    
    // Inserção das arestas
    g.insereA(0,1,4);       // A0
    g.insereA(1,2,8);       // A1
    g.insereA(2,3,7);       // A2
    g.insereA(3,4,9);       // A3
    g.insereA(4,5,10);      // A4
    g.insereA(5,6,2);       // A5
    g.insereA(6,7,1);       // A6
    g.insereA(7,0,8);       // A7
    g.insereA(2,8,2);       // A8
    g.insereA(2,5,4);       // A9
    g.insereA(3,5,14);      // A10
    g.insereA(6,8,6);       // A11
    g.insereA(7,8,7);       // A12
    g.insereA(7,1,11);      // A13

    // Execução do algoritmo de kruskal, armazenando a árvore em um grafo
    Grafo arvore = kruskal(&g);
    
    // Exibindo a árvore
    arvore.print();

    return 0;
}