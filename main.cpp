#include "leitura.h"
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "no.h"
#include "aresta.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    // Verifica argumentos: espera-se "main.out -d -m|-l grafo.txt"
    if (argc != 4) {
        cout << "Uso: " << argv[0] << " -d -m|-l grafo.txt" << endl;
        return 1;
    }
    
    string flagDirecionado = argv[1];
    string flagTipo = argv[2];
    string filename = argv[3];

    // Verificação do flag -d (pode ser usado para futuras opções)
    if (flagDirecionado != "-d") {
        cout << "Opção inválida. Use -d para indicar execução." << endl;
        return 1;
    }

    // Variável base para a interface comum do grafo
    grafo* g = nullptr;
    
    if (flagTipo == "-m") {
        // Carregamento via matriz de adjacência
        g = new grafo_matriz();
    } else if (flagTipo == "-l") {
        // Carregamento via lista encadeada
        g = new grafo_lista();
    } else {
        cout << "Opção inválida: use -m (matriz) ou -l (lista)" << endl;
        return 1;
    }
    
    // Carrega o grafo a partir do arquivo
    if (!g->carrega_grafo(filename)) {
        cout << "Erro ao carregar o arquivo " << filename << endl;
        delete g;
        return 1;
    }
    
    // Imprime as informações do grafo
    cout << filename << endl;
    cout << "Grau: " << g->get_grau() << endl;
    cout << "Ordem: " << g->get_ordem() << endl;
    cout << "Direcionado: " << (g->eh_direcionado() ? "Sim" : "Não") << endl;
    cout << "Vertices ponderados: " << (g->vertice_ponderado() ? "Sim" : "Não") << endl;
    cout << "Arestas ponderadas: " << (g->aresta_ponderada() ? "Sim" : "Não") << endl;
    cout << "Completo: " << (g->eh_completo() ? "Sim" : "Não") << endl;
    
    delete g;
    return 0;
}