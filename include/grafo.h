#pragma once
#include <string>
#include <cmath>
#include "leitura.h"

class Grafo {
protected:
    int num_vertices;
    bool direcionado;
    bool ponderado;
    std::string nome;

public:
    Grafo(int n, bool dir = false, bool pond = false, const std::string& nome = "");
    virtual ~Grafo() = default;

    virtual void adicionarAresta(int v1, int v2, float peso = 1.0) = 0;
    virtual void removerAresta(int v1, int v2) = 0;
    virtual bool existeAresta(int v1, int v2) const = 0;
    virtual float getPesoAresta(int v1, int v2) const = 0;
    
    int getNumVertices() const { return num_vertices; }
    bool isDirecionado() const { return direcionado; }
    bool isPonderado() const { return ponderado; }
    std::string getNome() const { return nome; }
};

