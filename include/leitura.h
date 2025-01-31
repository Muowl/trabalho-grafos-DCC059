#pragma once
#include <iostream>
#include <fstream>

// classe responsável por ler o arquivo de entrada e criar o grafo
// lembrar que a forma que vamos salvar a relação entre os grafos vai ser de responsabilidade da estrutura (lista ou matriz)
class leitura
{
private:
    int num_nos;
    bool direcionado;
    bool ponderado_vertices;
    bool ponderado_arestas; // fim da primeira linha do arquivo txt
    int *pesos_nos;         // armazena pesos dos nós (se ponderado_vertices == true)
    int total_lin;
    float **matriz_info;      
     
public:
    int *peso_nos(int num_nos); // segunda linha do arquivo txt, apenas se ponderado_vertices for true

    leitura(const std::string &filename);
    ~leitura();

    void imprimir_matriz_info(); // função para imprimir matriz de nós

    int get_num_nos() { return num_nos; }
    bool get_direcionado() { return direcionado; }
    bool get_ponderado_vertices() { return ponderado_vertices; }
    bool get_ponderado_arestas() { return ponderado_arestas; }
};