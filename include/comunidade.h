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
    
    void adicionarVertice(int vertice);
    bool contemVertice(int vertice) const;
    int getTamanho() const;
    int getId() const;
    const Vetor<int>& getVertices() const;
    
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
    
    // Avalia a qualidade da detecção
    float avaliarQualidade() const;
    
    // Exibe resultados da detecção
    void imprimirResultados() const;
};
