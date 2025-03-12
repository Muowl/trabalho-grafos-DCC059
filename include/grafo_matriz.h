#pragma once
#include "grafo.h"
#include "no.h"
#include "aresta.h"
#include <iostream>
#include <string>

class GrafoMatriz : public Grafo {
private:
    float** matriz_adj;
    no* nos;
    int capacidade;

    void redimensionar_matriz(int nova_capacidade) {
        float** nova_matriz = new float*[nova_capacidade];
        for (int i = 0; i < nova_capacidade; ++i) {
            nova_matriz[i] = new float[nova_capacidade]();
        }

        for (int i = 0; i < num_vertices; ++i) {
            for (int j = 0; j < num_vertices; ++j) {
                nova_matriz[i][j] = matriz_adj[i][j];
            }
        }

        for (int i = 0; i < capacidade; ++i) {
            delete[] matriz_adj[i];
        }
        delete[] matriz_adj;

        matriz_adj = nova_matriz;
        capacidade = nova_capacidade;
    }

public:

    GrafoMatriz(int n = 0, bool dir = false, bool pond = false, const std::string& nome = "");
    ~GrafoMatriz();

    // Métodos da interface Grafo
    void adicionarAresta(int v1, int v2, float peso = 1.0) override;
    void removerAresta(int v1, int v2) override;
    bool existeAresta(int v1, int v2) const override;
    float getPesoAresta(int v1, int v2) const override;
    
    // Métodos adicionais
    void imprimirGrafo() const;
    bool carregarDoArquivo(const std::string& arquivo);
};