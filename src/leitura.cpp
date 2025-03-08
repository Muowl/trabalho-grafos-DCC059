#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "../include/leitura.h"

struct Pair {
    int first;
    int second;
    
    Pair(int f, int s) : first(f), second(s) {}
    Pair() : first(0), second(0) {}
};

leitura::leitura(const std::string &filename) : num_nos(0), matriz_adj(nullptr)
{
    ler_arquivo_grafo(filename);
}

leitura::~leitura()
{
    if (matriz_adj != nullptr) {
        for (int i = 0; i < num_nos; i++) {
            delete[] matriz_adj[i];
        }
        delete[] matriz_adj;
    }
}

bool leitura::ler_arquivo_grafo(const std::string &filename)
{
    std::ifstream arquivo(filename);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string linha;
    int linhas_ignoradas = 0;

    // Ignorar as 4 primeiras linhas
    while (linhas_ignoradas < 4 && std::getline(arquivo, linha)) {
        linhas_ignoradas++;
    }

    // Determinar o número máximo de nós enquanto lê as arestas
    int max_no_id = -1;
    int from, to;
    
    // Primeira passagem: encontrar o maior ID de nó
    while (arquivo >> from >> to) {
        if (from > max_no_id) max_no_id = from;
        if (to > max_no_id) max_no_id = to;
        
        Pair aresta(from, to);
        arestas.push_back(std::pair<int, int>(from, to));
    }
    
    // O número de nós será o maior ID + 1 (assumindo que os IDs começam em 0)
    num_nos = max_no_id + 1;
    
    // Alocar a matriz de adjacências
    matriz_adj = new int*[num_nos];
    for (int i = 0; i < num_nos; i++) {
        matriz_adj[i] = new int[num_nos];
        // Inicializar com zeros
        for (int j = 0; j < num_nos; j++) {
            matriz_adj[i][j] = 0;
        }
    }

    // Segunda passagem: preencher a matriz de adjacências
    arquivo.clear();                 // limpar flags
    arquivo.seekg(0, std::ios::beg); // voltar ao início do arquivo
    
    // Ignorar as 4 primeiras linhas novamente
    linhas_ignoradas = 0;
    while (linhas_ignoradas < 4 && std::getline(arquivo, linha)) {
        linhas_ignoradas++;
    }

    // Ler as arestas e preencher a matriz
    while (arquivo >> from >> to) {
        matriz_adj[from][to] = 1; // Marcar como adjacente
    }

    arquivo.close();
    return true;
}

void leitura::imprimir_matriz_adj()
{
    if (matriz_adj == nullptr) {
        std::cout << "Matriz não inicializada" << std::endl;
        return;
    }

    std::cout << "Matriz de Adjacências (" << num_nos << "x" << num_nos << "):" << std::endl;
    for (int i = 0; i < num_nos; i++) {
        for (int j = 0; j < num_nos; j++) {
            std::cout << matriz_adj[i][j] << " ";
        }
        std::cout << std::endl;
    }
}