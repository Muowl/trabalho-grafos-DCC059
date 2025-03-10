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
    bool direcionado;     // indica se o grafo é direcionado
    bool ponderado_vertices; // indica se os vértices têm peso
    bool ponderado_arestas;  // indica se as arestas têm peso
    int total_lin;        // total de linhas (arestas) no arquivo
    float **matriz_info;  // matriz com informações (origem, destino, peso)
     
public:
    leitura(const std::string &filename);
    ~leitura();

    // Função para ler arquivo no formato com-friendster.txt ou similar
    // Ignora as linhas de comentário (começando com #) e monta a estrutura
    bool ler_arquivo_grafo(const std::string &filename);
    
    void imprimir_matriz_adj();          // função para imprimir matriz de adjacências

    // Getters
    int get_num_nos() const { return num_nos; }
    int** get_matriz_adj() const { return matriz_adj; }
    const Vetor<std::pair<int, int>>& get_arestas() const { return arestas; }
    bool get_direcionado() const { return direcionado; }
    bool get_ponderado_vertices() const { return ponderado_vertices; }
    bool get_ponderado_arestas() const { return ponderado_arestas; }
    int get_total_lin() const { return total_lin; }
    float** get_matriz_info() const { return matriz_info; }
};