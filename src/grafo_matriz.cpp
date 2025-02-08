#include "grafo_matriz.h"

no* grafo_matriz::get_no(int id) {
    if (id >= 0 && id < ordem) {
        return &nos[id];
    }
    return nullptr;
}

aresta* grafo_matriz::get_aresta(int origem, int destino) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        if (matriz_adj[origem][destino] != 0) {
            return new aresta(origem, destino, matriz_adj[origem][destino]);
        }
    }
    return nullptr;
}

void grafo_matriz::add_no(float peso) {
    if (ordem < MAX_VERTICES) {
        nos[ordem] = no(ordem, peso);
        ordem++;
    }
}

void grafo_matriz::add_aresta(int origem, int destino, float peso) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        matriz_adj[origem][destino] = peso;
        if (!direcionado) {
            matriz_adj[destino][origem] = peso;
        }
    }
}

aresta** grafo_matriz::get_vizinhos(int id) {
    if (id >= 0 && id < ordem) {
        aresta** vizinhos = new aresta*[ordem];
        int count = 0;
        for (int i = 0; i < ordem; i++) {
            if (matriz_adj[id][i] != 0) {
                vizinhos[count++] = new aresta(id, i, matriz_adj[id][i]);
            }
        }
        return vizinhos;
    }
    return nullptr;
}

void grafo_matriz::remove_no(int id) {
    if (id >= 0 && id < ordem) {
        for (int i = 0; i < ordem; i++) {
            matriz_adj[id][i] = 0;
            matriz_adj[i][id] = 0;
        }
    }
}