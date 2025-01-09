#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

class GrafoMatriz : public Grafo
{
private:
    int **matriz;
    int ordem;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;

    // Helper para alocar a matriz
    void alocarMatriz(int n)
    {
        // Libera a matriz antiga se houver
        if (matriz)
        {
            for (int i = 0; i < ordem; i++)
            {
                delete[] matriz[i];
            }
            delete[] matriz;
        }
        ordem = n;
        matriz = new int *[ordem];
        for (int i = 0; i < ordem; i++)
        {
            matriz[i] = new int[ordem];
            for (int j = 0; j < ordem; j++)
            {
                matriz[i][j] = 0;
            }
        }
    }

public:
    // Construtor padrão sem qualificação
    GrafoMatriz()
        : matriz(nullptr), ordem(0), direcionado(false),
          vertices_ponderados(false), arestas_ponderadas(false) {}

    // Construtor com parâmetros
    GrafoMatriz(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas)
        : matriz(nullptr), ordem(0), direcionado(direcionado),
          vertices_ponderados(vertices_ponderados), arestas_ponderadas(arestas_ponderadas)
    {
        alocarMatriz(ordem); // chama helper
    }

    ~GrafoMatriz() override
    {
        if (matriz)
        {
            for (int i = 0; i < ordem; i++)
            {
                delete[] matriz[i];
            }
            delete[] matriz;
        }
    }

    bool eh_bipartido() const override;
    int n_conexo() const override;
    int get_grau(int vertice) const override;
    // Ajuste para retornar apenas "ordem"
    int get_ordem() const override { return ordem; }
    bool eh_direcionado() const override;
    bool vertice_ponderado() const override;
    bool aresta_ponderada() const override;
    bool eh_completo() const override;
    bool eh_arvore() const override;
    bool possui_articulacao() const override;
    bool possui_ponte() const override;
    void carrega_grafo(const std::string &arquivo) override;
    void novo_grafo(const std::string &descricao, const std::string &saida) override;
};

#endif // GRAFO_MATRIZ_H