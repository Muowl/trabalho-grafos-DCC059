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

leitura::leitura(const std::string &filename) : 
    num_nos(0), 
    matriz_adj(nullptr), 
    direcionado(false),
    ponderado_vertices(false),
    ponderado_arestas(false),
    total_lin(0),
    matriz_info(nullptr)
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

    if (matriz_info != nullptr) {
        for (int i = 0; i < total_lin; i++) {
            delete[] matriz_info[i];
        }
        delete[] matriz_info;
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
    
    // Primeira passagem: contar linhas e encontrar o maior ID de nó
    int max_no_id = -1;
    total_lin = 0;
    
    // Ignorar linhas de comentário (começando com #)
    // E extrair informações importantes como número de nós
    while (std::getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') {
            // Tenta extrair número de nós da linha de comentário
            if (linha.find("Nodes:") != std::string::npos) {
                size_t pos = linha.find("Nodes:") + 7;
                while (pos < linha.size() && std::isspace(linha[pos])) pos++;
                num_nos = std::atoi(&linha[pos]);
            }
            continue;
        }
        
        // Processamento de linha válida de aresta
        size_t espaco_pos = linha.find_first_of(" \t"); // Encontra o primeiro espaço ou tabulação
        if (espaco_pos != std::string::npos) {
            std::string from_str = linha.substr(0, espaco_pos); // Extrai a substring até o separador
            std::string to_str = linha.substr(espaco_pos + 1);  // Extrai a substring após o separador
            int from = std::atoi(from_str.c_str());             // Converte para inteiro
            int to = std::atoi(to_str.c_str());                 // Converte para inteiro
            if (from > max_no_id) max_no_id = from;
            if (to > max_no_id) max_no_id = to;
            total_lin++;
        }
    }
    
    // Se o número de nós não foi extraído do comentário, use o maior ID + 1
    if (num_nos == 0) {
        num_nos = max_no_id + 1;
    }
    
    // Por padrão, assumimos que o grafo é direcionado (conforme o arquivo de exemplo)
    direcionado = true;
    
    // Alocar matriz_info para armazenar as arestas
    matriz_info = new float*[total_lin];
    for (int i = 0; i < total_lin; i++) {
        matriz_info[i] = new float[3]; // [origem, destino, peso]
    }

    // Segunda passagem: preencher matriz_info
    arquivo.clear();                 // Limpar flags
    arquivo.seekg(0, std::ios::beg); // Voltar ao início do arquivo
    
    int linha_index = 0;
    while (std::getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') continue;
        
        size_t espaco_pos = linha.find_first_of(" \t"); // Encontra o primeiro espaço ou tabulação
        if (espaco_pos != std::string::npos) {
            std::string from_str = linha.substr(0, espaco_pos); // Extrai a substring até o separador
            std::string to_str = linha.substr(espaco_pos + 1);  // Extrai a substring após o separador
            int from = std::atoi(from_str.c_str());             // Converte para inteiro
            int to = std::atoi(to_str.c_str());                 // Converte para inteiro
            matriz_info[linha_index][0] = static_cast<float>(from);
            matriz_info[linha_index][1] = static_cast<float>(to);
            matriz_info[linha_index][2] = 1.0f; // Peso padrão
            linha_index++;
        }
    }
    
    // Alocar e inicializar a matriz de adjacências
    matriz_adj = new int*[num_nos];
    for (int i = 0; i < num_nos; i++) {
        matriz_adj[i] = new int[num_nos];
        for (int j = 0; j < num_nos; j++) {
            matriz_adj[i][j] = 0;
        }
    }

    // Preencher a matriz de adjacências
    for (int i = 0; i < total_lin; i++) {
        int from = static_cast<int>(matriz_info[i][0]);
        int to = static_cast<int>(matriz_info[i][1]);
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