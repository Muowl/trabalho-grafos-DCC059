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
    Vetor<std::pair<int, int>> arestas; // lista de arestas (origem, destino)
     
public:
    leitura(const std::string &filename);
    ~leitura();

    // Função para ler arquivo no formato com-friendster.txt ou similar
    // Ignora as 4 primeiras linhas e monta uma estrutura de arestas
    bool ler_arquivo_grafo(const std::string &filename);

    // Getters
    int get_num_nos() const { return num_nos; }
    const Vetor<std::pair<int, int>>& get_arestas() const { return arestas; }
};