#pragma once

#include "comunidade.h"
#include "leitura.h"
#include <ctime>

class AlgoritmoRandomizado : public DetectorComunidades {
private:
    float probabilidadeConexao; // Probabilidade base para conexão
    int numComunidadesAlvo;     // Número alvo de comunidades
    int semente;                // Semente para o gerador de números aleatórios
    const leitura* leitor;      // Referência para o leitor que contém os nós presentes

public:
    AlgoritmoRandomizado(const Grafo* g, const leitura* l, float probabilidadeConexao = 0.5f, 
                         int numComunidadesAlvo = 5, int semente = 42);
    
    // Implementação do algoritmo randomizado
    void detectarComunidades() override;
    
private:
    // Inicializa comunidades com vértices aleatórios
    void inicializarComunidades();
    
    // Expande comunidades baseado em probabilidade
    void expandirComunidades();
    
    // Calcula probabilidade de um vértice pertencer a uma comunidade
    float calcularProbabilidade(int vertice, int idComunidade) const;
    
    // Função que sorteia um nó aleatório da lista de nós presentes
    int sortearNoAleatorio() const;
    
    // Função que gera um valor aleatório entre 0 e 1
    float gerarNumeroAleatorio() const;
    
    // Adiciona vizinhos diretos de um vértice à mesma comunidade
    void adicionarVizinhosDiretos(int vertice, int comunidade);
};
