#pragma once
#include "grafo.h"
#include "lista_encadeada.h"
#include "no.h"
#include "aresta.h"
#include <string>

// Classe derivada que utiliza lista encadeada para armazenar vértices e arestas
class grafo_lista : public grafo {
private:
    ListaEncadeada<no> vertices;   // cada vértice possui id e peso
    ListaEncadeada<aresta> arestas; // cada aresta armazena origem, destino e peso
public:
    struct MenorMaior {
        int no1;
        int no2;
        float distancia;
    };

    grafo_lista() : grafo() {}
    virtual ~grafo_lista() {}

    // Carrega o grafo a partir do arquivo de entrada utilizando a classe leitura
    bool carrega_grafo(const std::string &filename);
    
    // Adicionado: retorna a quantidade de componentes conexas
    virtual int n_conexo() const override;

    // Retorna o grau do grafo
    virtual int get_grau() const override;

    // Getters para testes
    const ListaEncadeada<no>& get_vertices() const { return vertices; }
    const ListaEncadeada<aresta>& get_arestas() const { return arestas; }

    // Métodos parte 2
    void novo_no(float peso);
    void nova_aresta(int origem, int destino, float peso);
    void deleta_no(int id);
    void deleta_aresta(int origem, int destino);

    MenorMaior menor_maior_distancia() const;
};