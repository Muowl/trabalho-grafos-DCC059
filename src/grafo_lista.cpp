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