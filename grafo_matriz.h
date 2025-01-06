#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>


class GrafoMatriz : public Grafo{
    private:
        vector<vector<int>> matriz;
        vector<int> pesos_vertices;
        int ordem;
    public:
        GrafoMatriz(int ord, bool dir = false, bool vp = false, bool ap = false)
            : ordem(ord), direcionado(dir), vertices_ponderados(vp), arestas_ponderadas(ap) {}

        bool eh_bipartido() const override;
        int n_conexo() const override;
        int get_grau(int vertice) const override;
        int get_ordem() const override;
        bool eh_direcionado() const override;
        bool vertice_ponderado() const override;
        bool aresta_ponderada() const override;
        bool eh_completo() const override;
        bool eh_arvore() const override;
        bool possui_articulacao() const override;
        bool possui_ponte() const override;
        void carrega_grafo(const std::string& arquivo) override;
        void novo_grafo(const std::string& descricao, const std::string& saida) override;
};


#endif GRAFO_MATRIZ_H