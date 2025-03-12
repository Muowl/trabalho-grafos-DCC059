#include "../include/grafo_lista.h"
#include "../include/leitura.h"
#include <iostream>
#include <iomanip>

GrafoLista::GrafoLista(int n, bool dir, bool pond, const std::string& nome)
    : Grafo(n, dir, pond, nome) {
    adjacencias = new ListaEncadeada<Aresta>[n];
}

GrafoLista::~GrafoLista() {
    delete[] adjacencias;
}

void GrafoLista::adicionarAresta(int v1, int v2, float peso) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    Aresta aresta(v1, v2, peso);
    adjacencias[v1].push_back(aresta);

    if (!direcionado && v1 != v2) {
        Aresta aresta_inv(v2, v1, peso);
        adjacencias[v2].push_back(aresta_inv);
    }
}

void GrafoLista::removerAresta(int v1, int v2) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return;

    adjacencias[v1].remove_if([v2](const Aresta& a) { return a.getDestino() == v2; });

    if (!direcionado) {
        adjacencias[v2].remove_if([v1](const Aresta& a) { return a.getDestino() == v1; });
    }
}

bool GrafoLista::existeAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return false;

    for (const auto& aresta : adjacencias[v1]) {
        if (aresta.getDestino() == v2)
            return true;
    }
    return false;
}

float GrafoLista::getPesoAresta(int v1, int v2) const {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        return -1.0f;

    for (const auto& aresta : adjacencias[v1]) {
        if (aresta.getDestino() == v2)
            return aresta.getPeso();
    }
    return -1.0f; // Aresta não existe
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

