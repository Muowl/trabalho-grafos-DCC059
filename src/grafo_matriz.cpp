#include "../include/grafo_matriz.h"
#include "../include/leitura.h"
#include <iostream>
#include <iomanip>

GrafoMatriz::GrafoMatriz(int n, bool dir, bool pond, const std::string& nome)
    : Grafo(n, dir, pond, nome) {
    num_vertices = n;
    capacidade = n > 0 ? n : 10;
    
    // Alocar apenas o suficiente para começar
    // Para grafos grandes que serão carregados de arquivo, é melhor começar pequeno
    capacidade = std::min(capacidade, 100); // Limitar tamanho inicial
    
    nos = new No[capacidade];
    for (int i = 0; i < std::min(n, capacidade); i++) {
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
    
    // Use nos_presentes instead of creating our own ids_unicos vector
    const Vetor<int>& nos_presentes = l.get_nos_presentes();
    
    // Atualizar o número de vértices para o número real de nós
    num_vertices = nos_presentes.size();
    std::cout << "Grafo compactado: " << num_vertices << " nós efetivos (de " 
              << l.get_num_nos() << " possíveis IDs)" << std::endl;
    
    // Redimensionar a matriz para o tamanho compacto
    if (capacidade < num_vertices) {
        redimensionar_matriz(num_vertices);
    }
    
    // Inicializar os nós
    for (int i = 0; i < num_vertices; i++) {
        nos[i] = No(i, std::to_string(nos_presentes[i])); // guarda o ID original como string
    }
    
    // Zerar a matriz de adjacência
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            matriz_adj[i][j] = 0;
        }
    }
    
    // Função auxiliar de busca binária
    auto buscar_indice = [&nos_presentes](int id) -> int {
        int esquerda = 0;
        int direita = nos_presentes.size() - 1;
        
        while (esquerda <= direita) {
            int meio = esquerda + (direita - direita) / 2;
            if (nos_presentes[meio] == id) {
                return meio;
            }
            if (nos_presentes[meio] < id) {
                esquerda = meio + 1;
            } else {
                direita = meio - 1;
            }
        }
        return -1; // não encontrado
    };
    
    // Preencher a matriz usando o mapeamento
    const Vetor<EdgeData>& arestas = l.get_arestas_com_peso();
    for (int i = 0; i < arestas.size(); i++) {
        int origem_idx = buscar_indice(arestas[i].origem);
        int destino_idx = buscar_indice(arestas[i].destino);
        
        if (origem_idx >= 0 && destino_idx >= 0) {
            matriz_adj[origem_idx][destino_idx] = arestas[i].peso;
            if (!direcionado && origem_idx != destino_idx) {
                matriz_adj[destino_idx][origem_idx] = arestas[i].peso;
            }
        }
    }
    
    return true;
}