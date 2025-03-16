#pragma once

#include "grafo.h"
#include "vetor.h"
#include <string>

// Representa uma comunidade: conjunto de vértices fortemente conectados
class Comunidade {
private:
    Vetor<int> vertices;
    int id;
    
public:
    Comunidade(int id = -1);
    Comunidade(const Comunidade& other);
    Comunidade& operator=(const Comunidade& other);
    
    void adicionarVertice(int vertice);
    bool contemVertice(int vertice) const;
    int getTamanho() const;
    int getId() const;
    
    Vetor<int> getVertices() const;
    int getVertice(int index) const;
    
    // Método otimizado para remover vértice sem vazamentos de memória
    void removerVertice(int indice) {
        if (indice >= 0 && indice < vertices.size()) {
            // Move o último elemento para a posição do removido
            if (indice < vertices.size() - 1) {
                vertices[indice] = vertices[vertices.size() - 1];
            }
            // Reduz o tamanho sem alocar nova memória
            vertices.resize(vertices.size() - 1);
        }
    }
    
    // Calcular densidade interna da comunidade
    float calcularDensidade(const Grafo* grafo) const;
};

// Classe base para algoritmos de detecção de comunidades
class DetectorComunidades {
protected:
    const Grafo* grafo;
    Vetor<Comunidade> comunidades;
    Vetor<int> atribuicaoVertices;  // Mapeia cada vértice para sua comunidade
    
    // Função para calcular modularidade da partição atual
    float calcularModularidade() const;
    
public:
    DetectorComunidades(const Grafo* g);
    virtual ~DetectorComunidades() = default;
    
    // Método que deve ser implementado pelos algoritmos específicos
    virtual void detectarComunidades() = 0;
    
    // Métodos comuns a todos os algoritmos
    int getNumComunidades() const;
    const Vetor<Comunidade>& getComunidades() const;
    int getComunidadeDoVertice(int vertice) const;
    
    // Método para acessar o grafo (adicionado para permitir análise)
    const Grafo* getGrafo() const { return grafo; }
    
    // Avalia a qualidade da detecção
    float avaliarQualidade() const;
    
    // Exibe resultados da detecção
    void imprimirResultados() const;
};
