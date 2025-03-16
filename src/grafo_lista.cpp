#include "../include/grafo_lista.h"
#include "../include/leitura.h"
#include <iostream>
#include <iomanip>

GrafoLista::GrafoLista(int n, bool dir, bool pond, const std::string& nome)
    : Grafo(n, dir, pond, nome) {
    adjacencias = new ListaEncadeada<Aresta>[n];
    inicializarCache();
}

void GrafoLista::inicializarCache() {
    // Inicializa a matriz de cache para consulta rápida O(1)
    matriz_cache = new float*[num_vertices];
    existencia_cache = new bool*[num_vertices];
    
    for (int i = 0; i < num_vertices; i++) {
        matriz_cache[i] = new float[num_vertices];
        existencia_cache[i] = new bool[num_vertices];
        
        // Inicializa com valores padrão
        for (int j = 0; j < num_vertices; j++) {
            matriz_cache[i][j] = -1.0f; // -1 indica que não há aresta
            existencia_cache[i][j] = false;
        }
    }
}

void GrafoLista::limparCache() {
    if (matriz_cache) {
        for (int i = 0; i < num_vertices; i++) {
            delete[] matriz_cache[i];
            delete[] existencia_cache[i];
        }
        delete[] matriz_cache;
        delete[] existencia_cache;
    }
}

GrafoLista::~GrafoLista() {
    limparCache();
    delete[] adjacencias;
}

void GrafoLista::adicionarAresta(int v1, int v2, float peso) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    // Verifica se a aresta já existe usando o cache
    if (existencia_cache[v1][v2]) {
        // Atualiza o peso se a aresta já existe
        for (auto& aresta : adjacencias[v1]) {
            if (aresta.getDestino() == v2) {
                aresta.setPeso(peso);
                matriz_cache[v1][v2] = peso;
                
                if (!direcionado && v1 != v2) {
                    for (auto& aresta_inv : adjacencias[v2]) {
                        if (aresta_inv.getDestino() == v1) {
                            aresta_inv.setPeso(peso);
                            matriz_cache[v2][v1] = peso;
                            break;
                        }
                    }
                }
                return;
            }
        }
    }

    // Adiciona a nova aresta
    Aresta aresta(v1, v2, peso);
    adjacencias[v1].push_back(aresta);
    
    // Atualiza o cache
    matriz_cache[v1][v2] = peso;
    existencia_cache[v1][v2] = true;

    if (!direcionado && v1 != v2) {
        Aresta aresta_inv(v2, v1, peso);
        adjacencias[v2].push_back(aresta_inv);
        
        // Atualiza o cache para a aresta inversa
        matriz_cache[v2][v1] = peso;
        existencia_cache[v2][v1] = true;
    }
}

void GrafoLista::removerAresta(int v1, int v2) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    // Atualiza o cache primeiro
    matriz_cache[v1][v2] = -1.0f;
    existencia_cache[v1][v2] = false;

    adjacencias[v1].remove_if([v2](const Aresta& a) { return a.getDestino() == v2; });

    if (!direcionado) {
        matriz_cache[v2][v1] = -1.0f;
        existencia_cache[v2][v1] = false;
        adjacencias[v2].remove_if([v1](const Aresta& a) { return a.getDestino() == v1; });
    }
}

bool GrafoLista::existeAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return false;

    // Consulta O(1) usando o cache
    return existencia_cache[v1][v2];
}

float GrafoLista::getPesoAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return -1.0f;

    // Consulta O(1) usando o cache
    return matriz_cache[v1][v2];
}

void GrafoLista::imprimirGrafo() const {
    std::cout << "Grafo: " << nome << std::endl;
    std::cout << "Número de vértices: " << num_vertices << std::endl;
    std::cout << "Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl;
    std::cout << "Ponderado: " << (ponderado ? "Sim" : "Não") << std::endl;
    std::cout << "Lista de adjacências:" << std::endl;

    for (int i = 0; i < num_vertices; i++) {
        if (adjacencias[i].empty()) continue;
        
        std::cout << i << " -> ";
        for (const auto& aresta : adjacencias[i]) {
            std::cout << aresta.getDestino();
            if (ponderado)
                std::cout << "(" << std::fixed << std::setprecision(1) << aresta.getPeso() << ")";
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void GrafoLista::carregarDoArquivo(const std::string& arquivo) {
    leitura leitor(arquivo);
    
    // Usar arestas com peso para o grafo AGMG
    const Vetor<EdgeData>& arestas = leitor.get_arestas_com_peso();
    
    // Adicionar cada aresta ao grafo com seu peso
    for (int i = 0; i < arestas.size(); i++) {
        adicionarAresta(arestas[i].origem, arestas[i].destino, arestas[i].peso);
    }
}