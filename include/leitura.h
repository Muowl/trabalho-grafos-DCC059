#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "vetor.h"

// Estrutura para armazenar uma aresta com peso
struct EdgeData {
    int origem;
    int destino;
    float peso;
    
    EdgeData(int src, int dest, float weight) : 
        origem(src), destino(dest), peso(weight) {}
};

// classe responsável por ler o arquivo de entrada e criar o grafo
class leitura
{
private:
    int num_nos;          // número de nós no grafo
    Vetor<std::pair<int, int>> arestas; // lista de arestas (origem, destino)
    Vetor<EdgeData> arestas_com_peso;   // lista de arestas com peso
     
public:
    leitura(const std::string &filename);
    ~leitura();

    // Função para ler arquivo no formato com-friendster.txt ou similar
    // Ignora as 4 primeiras linhas e monta uma estrutura de arestas
    bool ler_arquivo_grafo(const std::string &filename);

    // Método para gerar pesos sintéticos baseados na diferença absoluta entre os IDs dos nós
    void gerar_pesos_sinteticos();

    // Getters
    int get_num_nos() const { return num_nos; }
    const Vetor<std::pair<int, int>>& get_arestas() const { return arestas; }
    const Vetor<EdgeData>& get_arestas_com_peso() const { return arestas_com_peso; }
};