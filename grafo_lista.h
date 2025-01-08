#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include "grafo.h"
#include "lista_encadeada.h"
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
        ListaEncadeada<Aresta> adjacentes;
        Vertice(int i) : id(i) {}
    };

    std::vector<Vertice> vertices;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;

    // Funções auxiliares como "friend" para acessarem 'vertices'
    friend bool dfs_color(const GrafoLista& g, int v, int corAtual, int* corArray);
    friend void dfs_componente(const GrafoLista& g, int v, bool* visitado);
    friend void dfs_articulacao(const GrafoLista& g, int v, bool* visitado, int* disc, int* low, int* parent, bool* ap, int& time);
    friend void dfs_ponte(const GrafoLista& g, int v, bool* visitado, int* disc, int* low, int* parent, bool& has_bridge, int& time);
    


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