//Estrutura comentada em sala pelo professor
//Teoricamente pedido para "implementar nos filhos"

#ifndef GRAFO_H
#define GRAFO_H
#include "auxiliar_grafo.h"

class grafo {
public:
    grafo();
    ~grafo();

    no_grafo* get_no(int id);
    aresta_grafo* get_aresta(int origem, int destino);

    void add_no(float peso);
    void add_aresta(int origem, int destino, float peso);

    aresta_grafo** get_vizinhos(int id);
    void remove_no(int id);

    bool eh_completo();

};

#endif // GRAFO_H