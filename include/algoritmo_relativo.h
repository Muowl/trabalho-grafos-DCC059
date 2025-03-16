#pragma once

#include "comunidade.h"
#include "algoritmo_guloso.h"
#include "algoritmo_randomizado.h"

// Algoritmo que refina comunidades a partir de uma solução inicial
class AlgoritmoRelativo : public DetectorComunidades {
public:
    enum TipoInicial { GULOSO, RANDOMIZADO };
    
private:
    TipoInicial tipoAlgoritmoInicial;
    float limiarMelhoria;  // Limiar para aceitar movimentação de nós
    int maxIteracoes;      // Número máximo de iterações
    const leitura* leitor; // Referência para o leitor de dados (necessário para o algoritmo randomizado)
    
public:
    AlgoritmoRelativo(const Grafo* g, TipoInicial tipo = GULOSO, 
                     float limiarMelhoria = 0.01f, int maxIteracoes = 10,
                     const leitura* leitor = nullptr);
    
    // Implementação do algoritmo relativo
    void detectarComunidades() override;
    
private:
    // Inicializa com o algoritmo escolhido (guloso ou randomizado)
    void inicializarComAlgoritmo();
    
    // Refina comunidades
    void refinarComunidades();
    
    // Calcula ganho de modularidade ao mover vértice v para comunidade c
    float calcularGanhoModularidade(int vertice, int comunidadeOriginal, int comunidadeDestino) const;
    
    // Obtém comunidades vizinhas de um vértice
    void obterComunidadesVizinhas(int vertice, Vetor<int>& comunidadesVizinhas, int limite);
};
