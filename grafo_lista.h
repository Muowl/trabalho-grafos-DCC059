#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include "grafo.h"
#include <list>
#include <vector>
#include <fstream>
#include <stdexcept>

class GrafoLista : public Grafo {
private:
    struct Aresta {
        int destino;
        int peso;
        Aresta(int d, int p) : destino(d), peso(p) {}
    };

    struct Vertice {
        int id;
        std::list<Aresta> adjacentes;
        Vertice(int i) : id(i) {}
    };

    std::vector<Vertice> vertices;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;

public:
    GrafoLista(bool dir = false, bool vp = false, bool ap = false)
        : direcionado(dir), vertices_ponderados(vp), arestas_ponderadas(ap) {}

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

#endif // GRAFO_LISTA_H
