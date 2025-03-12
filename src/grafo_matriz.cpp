#include "../include/grafo_matriz.h"
#include "../include/leitura.h"
#include <iostream>
#include <iomanip>

GrafoMatriz::GrafoMatriz(int n, bool dir, bool pond, const std::string& nome)
    : Grafo(n, dir, pond, nome) {
    num_vertices = n;
    capacidade = n > 0 ? n : 10;
    
    nos = new No[capacidade];
    for (int i = 0; i < n; i++) {
        nos[i] = No(i, std::to_string(0.0f));
    }
    
    matriz_adj = new float*[capacidade];
    for (int i = 0; i < capacidade; i++) {
        matriz_adj[i] = new float[capacidade]();
    }
}

GrafoMatriz::~GrafoMatriz() {
    if (matriz_adj != nullptr) {
        for (int i = 0; i < capacidade; i++) {
            delete[] matriz_adj[i];
        }
        delete[] matriz_adj;
    }
    delete[] nos;
}

void GrafoMatriz::adicionarAresta(int v1, int v2, float peso) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    matriz_adj[v1][v2] = peso;
    if (!direcionado && v1 != v2) {
        matriz_adj[v2][v1] = peso;
    }
}

void GrafoMatriz::removerAresta(int v1, int v2) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    matriz_adj[v1][v2] = 0;
    if (!direcionado) {
        matriz_adj[v2][v1] = 0;
    }
}

bool GrafoMatriz::existeAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return false;
    return matriz_adj[v1][v2] != 0;
}

float GrafoMatriz::getPesoAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return 0;
    return matriz_adj[v1][v2];
}

void GrafoMatriz::imprimirGrafo() const {
    std::cout << "Grafo: " << nome << std::endl;
    std::cout << "Número de vértices: " << num_vertices << std::endl;
    std::cout << "Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl;
    std::cout << "Ponderado: " << (ponderado ? "Sim" : "Não") << std::endl;
    std::cout << "Matriz de adjacência:" << std::endl;
    
    std::cout << "   ";
    for (int i = 0; i < num_vertices; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < num_vertices; i++) {
        std::cout << std::setw(3) << i;
        for (int j = 0; j < num_vertices; j++) {
            if (matriz_adj[i][j] != 0) {
                std::cout << std::setw(4) << std::fixed << std::setprecision(1) << matriz_adj[i][j];
            } else {
                std::cout << std::setw(4) << "0";
            }
        }
        std::cout << std::endl;
    }
}

bool GrafoMatriz::carregarDoArquivo(const std::string& arquivo) {
    leitura l(arquivo);
    if (!l.ler_arquivo_grafo(arquivo))
        return false;
    
    // Atualiza o número de vértices a partir da leitura
    num_vertices = l.get_num_nos();
    // Se a capacidade for menor, redimensionar
    if (capacidade < num_vertices) {
        redimensionar_matriz(num_vertices);
    }
    
    // Inicializa nós (para este exemplo, atribuímos peso 0)
    for (int i = 0; i < num_vertices; i++) {
        nos[i] = No(i, std::to_string(0.0f));
    }
    
    // Zera a matriz de adjacência
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            matriz_adj[i][j] = 0;
        }
    }
    
    // Preenche as arestas usando os pesos sintéticos obtidos
    const auto& edges = l.get_arestas_com_peso();
    for (size_t i = 0; i < edges.size(); i++) {
        // Ajusta para 0-based, se os IDs no arquivo estiverem em 1-based
        int origem = edges[i].origem - 1;
        int destino = edges[i].destino - 1;
        float peso = edges[i].peso;
        if (origem >= 0 && origem < num_vertices && destino >= 0 && destino < num_vertices)
            adicionarAresta(origem, destino, peso);
    }
    
    return true;
}