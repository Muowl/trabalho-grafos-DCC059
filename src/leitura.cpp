#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include "../include/leitura.h"

leitura::leitura(const std::string &filename) : num_nos(0)
{
    ler_arquivo_grafo(filename);
    gerar_pesos_sinteticos();
}

leitura::~leitura()
{
    // Nothing to clean up
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

void leitura::gerar_pesos_sinteticos() {
    // Gerar pesos como o valor absoluto da diferença entre os IDs dos nós
    for (int i = 0; i < arestas.size(); i++) {
        int origem = arestas[i].first;
        int destino = arestas[i].second;
        float peso = std::abs(destino - origem);
        
        // Para evitar peso zero em self-loops
        if (peso == 0) peso = 1.0f;
        
        arestas_com_peso.push_back(EdgeData(origem, destino, peso));
    }
    
    std::cout << "Pesos sintéticos gerados para " << arestas_com_peso.size() << " arestas." << std::endl;
}