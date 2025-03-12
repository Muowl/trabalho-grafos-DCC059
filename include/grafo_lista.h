#pragma once
#include "grafo.h"
#include "lista_encadeada.h"
#include "no.h"
#include "aresta.h"
#include <string>

class GrafoLista : public Grafo {
private:
    ListaEncadeada<Aresta>* adjacencias;

public:
    GrafoLista(int n, bool dir = false, bool pond = false, const std::string& nome = "");
    ~GrafoLista();

    void adicionarAresta(int v1, int v2, float peso = 1.0) override;
    void removerAresta(int v1, int v2) override;
    bool existeAresta(int v1, int v2) const override;
    float getPesoAresta(int v1, int v2) const override;
    void imprimirGrafo() const;
    void carregarDoArquivo(const std::string& arquivo);
};

