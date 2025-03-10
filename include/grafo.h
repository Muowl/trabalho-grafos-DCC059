#pragma once
#include <string>
#include <cmath> // para std::abs
#include "leitura.h"

class grafo {
protected:
    int ordem; // número de vértices (ordem)
    bool direcionado;
    bool verticesPonderados;
    bool arestasPonderadas;
    float **matriz_info;
    int total_lin;
public:
    // Inicializa os atributos com valores padrão
    grafo() : ordem(0), direcionado(false), verticesPonderados(false), arestasPonderadas(false),
              matriz_info(nullptr), total_lin(0) {}
    virtual ~grafo() {
        // Não liberamos matriz_info aqui, pois é gerenciada pela classe leitura
    }

    // Nova função para calcular o peso da aresta como abs(from - to)
    int calcular_peso_aresta(int from, int to) const {
        return std::abs(from - to);
    }

    // Funções implementadas na classe abstrata:
    virtual int n_conexo() const {
        // implementação básica (a ser refinada conforme a estrutura)
        return 1;
    }
    
    virtual int get_grau() const {
        // A implementação concreta precisa utilizar os métodos de acesso específicos
        return 0;
    }
    
    int get_ordem() const { return ordem; }
    bool eh_direcionado() const { return direcionado; }
    bool vertice_ponderado() const { return verticesPonderados; }
    bool aresta_ponderada() const { return arestasPonderadas; }

    // Implementação de get_grau_no utilizando a matriz_info
    int get_grau_no(int id) const {
        if (matriz_info == nullptr || total_lin == 0)
            return 0;
            
        int degree = 0;
        // Para cada linha (aresta) da matriz
        for (int i = 0; i < total_lin; i++) {
            int origem = static_cast<int>(matriz_info[i][0]);
            int destino = static_cast<int>(matriz_info[i][1]);
            if (direcionado) {
                if (origem == id) degree++; // saída
                if (destino == id) degree++; // entrada
            } else {
                if (origem == id || destino == id)
                    degree++;
            }
        }
        return degree;
    }

    // Assumindo digrafo simples e grafo simples
    virtual bool eh_completo() const {
        if(get_ordem() <= 1)
            return true;
        for (int i = 0; i < get_ordem(); i++) {
            if (get_grau() - 1 != get_grau_no(i))
                return false;
        }
        return true;
    }

    virtual bool carrega_grafo(const std::string &filename) {
        // Implementação base que deve ser sobrescrita pelas classes derivadas
        return true;
    }
    
    // Função auxiliar para calcular o peso de todas as arestas e preencher uma matriz de pesos
    void calcular_pesos_arestas(float **matriz_pesos, int num_arestas) {
        if (matriz_info == nullptr)
            return;
            
        for (int i = 0; i < num_arestas; i++) {
            int from = static_cast<int>(matriz_info[i][0]);
            int to = static_cast<int>(matriz_info[i][1]);
            // Verificar se from e to estão dentro dos limites válidos
            if (from >= 0 && from < ordem && to >= 0 && to < ordem) {
                matriz_pesos[from][to] = static_cast<float>(calcular_peso_aresta(from, to));
            }
        }
    }
};
