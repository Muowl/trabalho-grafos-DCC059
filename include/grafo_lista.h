#pragma once
#include "grafo.h"
#include "lista_encadeada.h"
#include "no.h"
#include "aresta.h"
#include <string>

// Classe derivada que utiliza lista encadeada para armazenar vértices e arestas
class grafo_lista : public grafo {
private:
    // Usando a lista encadeada para armazenar vértices e arestas
    ListaEncadeada<no> vertices;   // cada vértice possui id e peso
    ListaEncadeada<aresta> arestas; // cada aresta armazena origem, destino e peso
public:
    grafo_lista() : grafo() {}
    virtual ~grafo_lista() {}
};
