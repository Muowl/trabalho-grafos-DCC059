#pragma once

#include "comunidade.h"

class AlgoritmoGuloso : public DetectorComunidades {
private:
    float limiarDensidade; // Limiar para considerar quando um vértice deve ser adicionado à comunidade
    int limiteNos;         // Limite para número de nós a considerar (para desempenho)

public:
    // Reduzir ainda mais o limiar para grafos pequenos
    AlgoritmoGuloso(const Grafo* g, float limiarDensidade = 0.05f, int limiteNos = 1000);
    
    // Implementação do algoritmo guloso
    void detectarComunidades() override;
    
private:
    // Expande uma comunidade a partir de um vértice inicial
    void expandirComunidade(int idComunidade, int verticeInicial, int nodesLimit);
    
    // Verifica se um vértice deve ser adicionado a uma comunidade
    bool deveAdicionarVertice(int idComunidade, int vertice) const;
};
