#include "../include/grafo_lista.h"
#include "grafo_lista.h"
#include "../include/leitura.h"
#include <iostream>

bool grafo_lista::carrega_grafo(const std::string &filename) {
    // Cria objeto de leitura para o arquivo
    leitura l(filename);

    int numNos = l.get_num_nos();
    bool ponderadoVertices = l.get_ponderado_vertices();
    
    // Cria nós com id de 0 a numNos-1.
    // como a classe leitura não expõe os pesos, usa-se 0 como peso padrão
    for (int i = 0; i < numNos; i++) {
        no novoNo(i, (ponderadoVertices ? 0 : 0));
        vertices.push_back(novoNo);
    }

    int totalArestas = l.get_total_lin();
    float **matriz = l.get_matriz_info();

    // Para cada linha da matriz_info (formato: [origem, destino, peso])
    for (int i = 0; i < totalArestas; i++) {
        int origem  = static_cast<int>(matriz[i][0]);
        int destino = static_cast<int>(matriz[i][1]);
        float peso  = matriz[i][2];
        aresta novaAresta(origem, destino, peso);
        arestas.push_back(novaAresta);
    }
    // Retorna true se o grafo foi carregado com sucesso
    return true;
}

// Implementação de n_conexo usando brute force
int grafo_lista::n_conexo() const {
    // Obter quantidade de vértices
    int n = vertices.get_size();
    if (n == 0)
        return 0;

    // Cria array para os pais
    int *parent = new int[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    // Função inline para encontrar o representante (com iteração)
    auto find = [&parent](int x) -> int {
        while (parent[x] != x)
            x = parent[x];
        return x;
    };

    // Iterar sobre a lista de arestas e unir os conjuntos
    for (auto it = arestas.begin(); it != arestas.end(); ++it) {
        int u = it->origem;
        int v = it->destino;
        int pu = find(u);
        int pv = find(v);
        if (pu != pv) {
            parent[pv] = pu;
        }
    }

    // Contar raízes (componentes)
    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (find(i) == i)
            componentes++;
    }
    
    delete[] parent;
    return componentes;
}

// Adicionado: implementação de get_grau para calcular o grau do grafo
int grafo_lista::get_grau() const {
    int maxDegree = 0;
    // Iterar sobre cada vértice
    for (auto vit = vertices.begin(); vit != vertices.end(); ++vit) {
        int degree = 0;
        // Iterar sobre cada aresta
        for (auto ait = arestas.begin(); ait != arestas.end(); ++ait) {
            if (!direcionado) {
                // Para grafo não direcionado, contar se o vértice está em qualquer das extremidades
                if (ait->origem == vit->id || ait->destino == vit->id)
                    degree++;
            } else {
                // Para digrafo, somar in e out degree
                if (ait->origem == vit->id)
                    degree++;
                if (ait->destino == vit->id)
                    degree++;
            }
        }
        if (degree > maxDegree)
            maxDegree = degree;
    }
    return maxDegree;
}