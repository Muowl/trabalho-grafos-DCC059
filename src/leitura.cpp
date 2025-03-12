#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "../include/leitura.h"

leitura::leitura(const std::string &filename) : num_nos(0)
{
    ler_arquivo_grafo(filename);
}

leitura::~leitura()
{

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
    
    // Lê as arestas e encontra o maior ID de nó
    while (arquivo >> from >> to) {
        if (from > max_no_id) max_no_id = from;
        if (to > max_no_id) max_no_id = to;
        
        arestas.push_back(std::pair<int, int>(from, to));
    }
    
    // O número de nós será o maior ID + 1 (assumindo que os IDs começam em 0)
    num_nos = max_no_id + 1;

    arquivo.close();
    return true;
}