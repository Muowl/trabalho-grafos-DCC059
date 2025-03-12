#include "../include/grafo_matriz.h"
#include "../include/leitura.h"
#include <iostream>
#include <iomanip>

GrafoMatriz::GrafoMatriz(int n, bool dir, bool pond, const std::string& nome)
    : Grafo(n, dir, pond, nome) {
    num_vertices = n;
    capacidade = n > 0 ? n : 10;
    
    nos = new no[capacidade];
    for (int i = 0; i < n; i++) {
        nos[i] = no(i, 0.0f);
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
    
    // Cabeçalho
    std::cout << "   ";
    for (int i = 0; i < num_vertices; i++) {
        std::cout << std::setw(4) << i;
    }
    std::cout << std::endl;
    
    // Matriz
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
    
    num_vertices = l.get_num_nos();
    direcionado = l.get_direcionado();
    ponderado = l.get_ponderado_arestas();
    
    // Garantir capacidade suficiente
    if (capacidade < num_vertices) {
        redimensionar_matriz(num_vertices);
    }

    // Inicializa os nós
    for (int i = 0; i < num_vertices; i++) {
        float peso = l.get_ponderado_vertices() ? l.get_pesos_nos()[i] : 0;
        nos[i] = no(i, peso);
    }

    // Inicializa a matriz de adjacência
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            matriz_adj[i][j] = 0;
        }
    }

    // Preenche a matriz de adjacência com as arestas
    int totalArestas = l.get_total_lin();
    float** matriz = l.get_matriz_info();

    for (int i = 0; i < totalArestas; i++) {
        int origem = static_cast<int>(matriz[i][0]) - 1;  // Converter para 0-based
        int destino = static_cast<int>(matriz[i][1]) - 1; // Converter para 0-based
        float peso = l.get_ponderado_arestas() ? matriz[i][2] : 1.0f;
        
        adicionarAresta(origem, destino, peso);
    }

    return true;
}