/* #ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include "no.h"
#include "aresta.h"
#include <iostream>
#include <string>

class grafo_matriz: public grafo {
private:
    float** matriz_adj;
    no* nos;
    int capacidade;

    void redimensionar_matriz(int nova_capacidade) {
        float** nova_matriz = new float*[nova_capacidade];
        for (int i = 0; i < nova_capacidade; ++i) {
            nova_matriz[i] = new float[nova_capacidade]();
        }

        for (int i = 0; i < ordem; ++i) {
            for (int j = 0; j < ordem; ++j) {
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
    // Estrutura auxiliar para retornar os nós com a maior distância
    struct MenorMaior {
        int no1;
        int no2;
        float distancia;
    };

    grafo_matriz() {
        ordem = 0;
        capacidade = 10;
        direcionado = false;
        
        nos = new no[capacidade];
        
        matriz_adj = new float*[capacidade];
        for (int i = 0; i < capacidade; i++) {
            matriz_adj[i] = new float[capacidade]();
        }
    }

    ~grafo_matriz() {
        if (matriz_adj != nullptr) {
            for (int i = 0; i < capacidade; i++) {
                if (matriz_adj[i] != nullptr) {
                    delete[] matriz_adj[i];
                }
            }
            delete[] matriz_adj;
        }
        if (nos != nullptr) {
            delete[] nos;
        }
    }

    no* get_no(int id);
    aresta* get_aresta(int origem, int destino);

    void add_no(float peso);
    void add_aresta(int origem, int destino, float peso);

    aresta** get_vizinhos(int id);

    void remove_no(int id);
    
    int n_conexo() const;
    int get_grau() const override;

    bool carrega_grafo(const std::string &filename);

    // Métodos parte 2
    void novo_no(float peso);
    void nova_aresta(int origem, int destino, float peso);
    void deleta_no(int id);
    void deleta_aresta(int origem, int destino);

    MenorMaior menor_maior_distancia() const;
};

#endif // GRAFO_MATRIZ_H */