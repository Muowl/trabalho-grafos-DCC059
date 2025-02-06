#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include "no.h"
#include "aresta.h"

class grafo_matriz: public grafo {
    private:
        static const int MAX_VERTICES = 100;
        float matriz_adj[MAX_VERTICES][MAX_VERTICES];  
        no nos[MAX_VERTICES];
    public:
        grafo_matriz(int ordem, bool direcionado, bool verticesPonderados, bool arestasPonderadas)
            : grafo() {
            this->ordem = ordem;
            this->direcionado = direcionado;
            this->verticesPonderados = verticesPonderados;
            this->arestasPonderadas = arestasPonderadas;

            for (int i = 0; i < MAX_VERTICES; ++i) {
                for (int j = 0; j < MAX_VERTICES; ++j) {
                    matriz_adj[i][j] = 0;
                }
            }
        }
        ~grafo_matriz() {}
        void imprimir_matriz_adj();
}

#endif // GRAFO_MATRIZ_H