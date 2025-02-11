#include "../include/grafo_lista.h"
#include "grafo_lista.h"
#include "../include/leitura.h"
#include <iostream>

bool grafo_lista::carrega_grafo(const std::string &filename) {
    // Cria objeto de leitura para o arquivo
    leitura l(filename);
    ordem = l.get_num_nos();
    direcionado = l.get_direcionado();
    verticesPonderados = l.get_ponderado_vertices();
    arestasPonderadas = l.get_ponderado_arestas();

    int numNos = l.get_num_nos();
    // Adicionado: atualiza a ordem do grafo com o número de nós lidos
    ordem = numNos;
    
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

int grafo_lista::n_conexo() const {
    int n = vertices.get_size();
    if (n == 0)
        return 0;

    int *parent = new int[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    auto find = [&parent](int x) -> int {
        while (parent[x] != x)
            x = parent[x];
        return x;
    };

    for (auto it = arestas.begin(); it != arestas.end(); ++it) {
        // Ajuste para índices 0-based (se necessário)
        int u = it->origem - 1; // Subtrai 1 se os vértices são 1-based
        int v = it->destino - 1;

        // Verifica se u e v são índices válidos
        if (u < 0 || u >= n || v < 0 || v >= n) {
            // Tratar erro: vértice inválido na aresta
            delete[] parent;
            throw std::runtime_error("Vértice inválido na aresta");
        }

        int pu = find(u);
        int pv = find(v);
        if (pu != pv) {
            parent[pv] = pu;
        }
    }

    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (find(i) == i)
            componentes++;
    }
    
    delete[] parent;
    return componentes;
}

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

// Parte 2

void grafo_lista::novo_no(float peso) {
    no novoNo(ordem, peso);
    vertices.push_back(novoNo);
    ordem++;
}

void grafo_lista::nova_aresta(int origem, int destino, float peso) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        aresta a(origem, destino, peso);
        arestas.push_back(a);
        // Se o grafo não for direcionado, adiciona a aresta reversa
        if (!direcionado) {
            aresta reversa(destino, origem, peso);
            arestas.push_back(reversa);
        }
    }
}

void grafo_lista::deleta_no(int id) {
    // Remove arestas ligadas ao vértice
    arestas.remove_if([&](const aresta &a) {
        return (a.origem == id || a.destino == id);
    });
    // Remove o vértice
    vertices.remove_if([&](const no &n) {
        return (n.id == id);
    });
    // Atualiza a ordem do grafo
    ordem--;
}

void grafo_lista::deleta_aresta(int origem, int destino) {
    bool removed = false;
    arestas.remove_if([&](const aresta &a) {
        if (!removed && a.origem == origem && a.destino == destino) {
            removed = true;
            return true;
        }
        return false;
    });
    // Se o grafo não for direcionado, removemos a aresta reversa
    if (!direcionado) {
        removed = false;
        arestas.remove_if([&](const aresta &a) {
            if (!removed && a.origem == destino && a.destino == origem) {
                removed = true;
                return true;
            }
            return false;
        });
    }
}