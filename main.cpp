#include "leitura.h"
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "no.h"
#include "aresta.h"
#include <iostream>

using namespace std;

int main() {
    // Teste utilizando a classe leitura (já existente)
    leitura l("../entradas/grafo.txt");
    cout << "Num nos : " << l.get_num_nos() << endl;
    cout << "Direcionado : " << l.get_direcionado() << endl;
    cout << "Ponderado nos : " << l.get_ponderado_vertices() << endl;
    cout << "Ponderado arestas : " << l.get_ponderado_arestas() << endl;
    l.imprimir_matriz_info();

    // Teste para verificar se grafo_lista está sendo criado corretamente
    grafo_lista gl;
    if (gl.carrega_grafo("../entradas/grafo.txt")) {
        cout << "\ngrafo_lista carregado com sucesso." << endl;

        // Imprimindo os vértices
        cout << "Vertices: ";
        const auto &vertices = gl.get_vertices();
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            cout << it->id << " ";
        }
        cout << endl;

        // Imprimindo as arestas
        cout << "Arestas: ";
        const auto &arestas = gl.get_arestas();
        for (auto it = arestas.begin(); it != arestas.end(); ++it) {
            cout << "(" << it->origem << ", " << it->destino << ", " << it->peso << ") ";
        }
        cout << endl;

        // Teste das novas funções: get_grau e n_conexo
        cout << "Grau do grafo: " << gl.get_grau() << endl;
        cout << "Ordem do grafo: " << gl.get_ordem() << endl;
        cout << "Quantidade de componentes conexas: " << gl.n_conexo() << endl;
    } else {
        cout << "Erro ao carregar grafo_lista." << endl;
    }

    // Teste para verificar se grafo_matriz está sendo criado corretamente
    grafo_matriz gm;
    if (gm.carrega_grafo("../entradas/grafo.txt")) {
        cout << "\ngrafo_matriz carregado com sucesso." << endl;

        // Imprimindo os vértices
        cout << "Vertices: ";
        for (int i = 0; i < gm.get_ordem(); i++) {
            no* n = gm.get_no(i);
            if (n != nullptr) {
                cout << n->id << " ";
            }
        }
        cout << endl;

        // Imprimindo as arestas
        cout << "Arestas: ";
        for (int i = 0; i < gm.get_ordem(); i++) {
            for (int j = 0; j < gm.get_ordem(); j++) {
                aresta* a = gm.get_aresta(i, j);
                if (a != nullptr) {
                    cout << "(" << a->origem << ", " << a->destino << ", " << a->peso << ") ";
                    delete a; // Liberar a memória alocada para a aresta
                }
            }
        }
        cout << endl;
    } else {
        cout << "Erro ao carregar grafo_matriz." << endl;
    }
    
    return 0;
}