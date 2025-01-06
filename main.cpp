#include <iostream>
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "grafo.h"

void exibir_descricao(const Grafo& grafo) {
    std::cout << "Ordem: " << grafo.get_ordem() << "\n";
    std::cout << "Direcionado: " << (grafo.eh_direcionado() ? "Sim" : "Não") << "\n";
    std::cout << "Vertices ponderados: " << (grafo.vertice_ponderado() ? "Sim" : "Não") << "\n";
    std::cout << "Arestas ponderadas: " << (grafo.aresta_ponderada() ? "Sim" : "Não") << "\n";
    std::cout << "Completo: " << (grafo.eh_completo() ? "Sim" : "Não") << "\n";
    std::cout << "Bipartido: " << (grafo.eh_bipartido() ? "Sim" : "Não") << "\n";
    std::cout << "Árvore: " << (grafo.eh_arvore() ? "Sim" : "Não") << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " [-d|-c] [-m|-l] <arquivo> [descricao]" << std::endl;
        return 1;
    }

    std::string operacao = argv[1];
    std::string estrutura = argv[2];
    std::string arquivo = argv[3];

    if (operacao == "-d") {
        if (estrutura == "-m") {
            GrafoMatriz grafo;
            grafo.carrega_grafo(arquivo);
            exibir_descricao(grafo);
        } else if (estrutura == "-l") {
            GrafoLista grafo;
            grafo.carrega_grafo(arquivo);
            exibir_descricao(grafo);
        } else {
            std::cerr << "Estrutura desconhecida: " << estrutura << std::endl;
            return 1;
        }
    } else if (operacao == "-c" && argc == 5) {
        std::string descricao = argv[4];
        if (estrutura == "-m") {
            GrafoMatriz grafo;
            grafo.novo_grafo(descricao, arquivo);
        } else if (estrutura == "-l") {
            GrafoLista grafo;
            grafo.novo_grafo(descricao, arquivo);
        } else {
            std::cerr << "Estrutura desconhecida: " << estrutura << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Operacao desconhecida ou parametros insuficientes" << std::endl;
        return 1;
    }

    return 0;
}
