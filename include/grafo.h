#pragma once
#include <string>
#include "leitura.h"

class grafo {
protected:
    int ordem; // número de vértices (ordem)
    bool direcionado;
    bool verticesPonderados;
    bool arestasPonderadas;
public:
    grafo() : ordem(0), direcionado(false), verticesPonderados(false), arestasPonderadas(false) {}
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
    virtual int get_grau_no(int id) const { return 0; }

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
        // Usa a classe leitura para carregar os dados do arquivo e inicializar os atributos
        leitura l(filename);
        // A partir daqui, a classe derivada deve processar os dados lidos para preencher as estruturas internas
        return true;
    }
};
