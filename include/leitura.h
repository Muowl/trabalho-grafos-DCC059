#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "vetor.h"

// classe responsável por ler o arquivo de entrada e criar o grafo
class leitura
{
private:
    int num_nos;          // número de nós no grafo
    int **matriz_adj;     // matriz de adjacências
    Vetor<std::pair<int, int>> arestas; // lista de arestas (origem, destino)
     
public:
    leitura(const std::string &filename);
    ~leitura();

    // Função para ler arquivo no formato com-friendster.txt ou similar
    // Ignora as 4 primeiras linhas e monta uma estrutura de matriz
    bool ler_arquivo_grafo(const std::string &filename);
    
    void imprimir_matriz_adj();          // função para imprimir matriz de adjacências

    // Getters
    int get_num_nos() const { return num_nos; }
    int** get_matriz_adj() const { return matriz_adj; }
    const Vetor<std::pair<int, int>>& get_arestas() const { return arestas; }
};