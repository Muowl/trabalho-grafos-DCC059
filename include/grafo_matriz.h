#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H
#define MAX_VERTICES 50

#include "grafo.h"
#include "no.h"
#include "aresta.h"
#include <iostream>

class grafo_matriz: public grafo {
    private:
        float matriz_adj[MAX_VERTICES][MAX_VERTICES];  
        no nos[MAX_VERTICES];
    public:
        grafo_matriz(int ordem, bool direcionado, bool verticesPonderados, bool arestasPonderadas)
            : grafo() {
            this->ordem = ordem;
            this->direcionado = direcionado;
            this->verticesPonderados = verticesPonderados;
            this->arestasPonderadas = arestasPonderadas;

            for (int i = 0; i < MAX_VERTICES; i++) {
                for (int j = 0; j < MAX_VERTICES; j++) {
                    matriz_adj[i][j] = 0;
                }
            }
        }
        no* get_no(int id);
        aresta* get_aresta(int origem, int destino);

        void add_no(float peso);
        void add_aresta(int origem, int destino, float peso);

        aresta** get_vizinhos(int id);
        void remove_no(int id);
}

#endif // GRAFO_MATRIZ_H