//Estrutura comentada em sala pelo professor
//Teoricamente pedido para "implementar nos filhos"

#ifndef GRAFO_H
#define GRAFO_H
#include "no.h"
#include "aresta.h"

class grafo {
public:
    grafo();
    ~grafo();

    no* get_no(int id);
    aresta* get_aresta(int origem, int destino);

    void add_no(float peso);
    void add_aresta(int origem, int destino, float peso);

    aresta** get_vizinhos(int id);
    void remove_no(int id);

    bool eh_completo();

};

#endif // GRAFO_H