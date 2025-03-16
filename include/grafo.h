#pragma once
#include <string>
#include <cmath>
#include "leitura.h"
#include "no.h"
#include "vetor.h"

class Grafo {
protected:
    int num_vertices;
    bool direcionado;
    bool ponderado;
    std::string nome;

public:
    Grafo(int n, bool dir = false, bool pond = false, const std::string& nome = "");
    virtual ~Grafo() = default;

    virtual void adicionarAresta(int v1, int v2, float peso = 1.0) = 0;
    virtual void removerAresta(int v1, int v2) = 0;
    virtual bool existeAresta(int v1, int v2) const = 0;
    virtual float getPesoAresta(int v1, int v2) const = 0;
    
    int calcularGrau(int v) const {
        if (v < 0 || v >= num_vertices)
            return -1;
            
        int grau = 0;
        for (int i = 0; i < num_vertices; i++) {
            if (existeAresta(v, i)) {
                grau++;
            }
        }
        
        return grau;
    }

    Vetor<No> getNodesComGrau() const {
        // CORRIGIDO: Inicializar e gerenciar objetos No corretamente
        Vetor<No> nos;
        nos.resize(num_vertices); // Pré-alocar espaço para todos os vértices
        
        for (int i = 0; i < num_vertices; i++) {
            // Criar um objeto No adequado
            No no(i);
            int grau = calcularGrau(i);
            no.setGrau(grau);
            
            // Atribuir ao espaço pré-alocado
            nos[i] = no;
        }
        
        return nos;
    }

    // Implementação para buscar o nó com maior grau, sem limite, mais custoso em tempo de execução
    No getNoComMaiorGrau() const {
        int maxGrau = -1;
        int idNoMaiorGrau = -1;
        
        for (int i = 0; i < num_vertices; i++) {
            int grau = calcularGrau(i);
            if (grau > maxGrau) {
                maxGrau = grau;
                idNoMaiorGrau = i;
            }
        }
        
        if (idNoMaiorGrau == -1) {
            return No(-1);
        }
        
        No resultado(idNoMaiorGrau);
        resultado.setGrau(maxGrau);
        return resultado;
    }
    
    // Observação: para implementação final vamos usar o limitado para evitar problemas de desempenho
    No getNoComMaiorGrauLimitado(int limite) const {
        int maxGrau = -1;
        int idNoMaiorGrau = -1;
        
        int max = (limite < num_vertices) ? limite : num_vertices;
        for (int i = 0; i < max; i++) {
            int grau = calcularGrau(i);
            if (grau > maxGrau) {
                maxGrau = grau;
                idNoMaiorGrau = i;
            }
        }
        
        if (idNoMaiorGrau == -1) {
            return No(-1);
        }
        
        No resultado(idNoMaiorGrau);
        resultado.setGrau(maxGrau);
        return resultado;
    }

    int getNumVertices() const { return num_vertices; }
    bool isDirecionado() const { return direcionado; }
    bool isPonderado() const { return ponderado; }
    std::string getNome() const { return nome; }
};

