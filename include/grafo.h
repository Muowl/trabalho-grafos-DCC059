#pragma once
#include <string>
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
    grafo() : ordem(0), direcionado(false), verticesPonderados(false), arestasPonderadas(false),
              matriz_info(nullptr), total_lin(0) {}
    virtual ~grafo() {}

    // Funções implementadas na classe abstrata:
    virtual int n_conexo() const {
        // implementação básica (a ser refinada conforme a estrutura)
        // Exemplo: retornar 1 se o grafo for conexo
        return 1;
    }
    virtual int get_grau() const {
        // A implementação concreta precisa utilizar os métodos de acesso, que são definidos nas classes filhas
        return 0;
    }
    int get_ordem() const { return ordem; }
    bool eh_direcionado() const { return direcionado; }
    bool vertice_ponderado() const { return verticesPonderados; }
    bool aresta_ponderada() const { return arestasPonderadas; }

    // Implementação de get_grau_no utilizando a matriz carregada em leitura.h
    int get_grau_no(int id) const {
        int degree = 0;
        // Para cada linha (aresta) da matriz
        for (int i = 0; i < total_lin; i++) {
            int origem = static_cast<int>(matriz_info[i][0]) - 1; // converter para índice 0-based
            int destino = static_cast<int>(matriz_info[i][1]) - 1;
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
            std::cout << "Grau : " << get_grau() << std::endl;
            if (get_grau() - 1 != get_grau_no(i))
                std::cout << "Grau i : " << get_grau_no(i) << std::endl;
                return false;
        }
        return true;
    }

    virtual bool carrega_grafo(const std::string &filename) {
        // Usa a classe leitura para carregar os dados do arquivo e inicializar os atributos
        leitura l(filename);
        // A partir daqui, a classe derivada deve processar os dados lidos para preencher as estruturas internas
        return true;
    }
};
