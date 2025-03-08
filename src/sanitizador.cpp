#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>  // para qsort
#include "../include/vetor.h"  // Seu vetor personalizado

// Definindo um par simples para representar arestas
struct Par {
    int primeiro;
    int segundo;
    
    Par(int p = 0, int s = 0) : primeiro(p), segundo(s) {}
    
    bool operator==(const Par& outro) const {
        return primeiro == outro.primeiro && segundo == outro.segundo;
    }
    
    bool operator!=(const Par& outro) const {
        return !(*this == outro);
    }
};

// Função de comparação para qsort
int compararArestas(const void* a, const void* b) {
    const Par* arestaA = (const Par*)a;
    const Par* arestaB = (const Par*)b;
    if (arestaA->primeiro != arestaB->primeiro) {
        return arestaA->primeiro - arestaB->primeiro;
    }
    return arestaA->segundo - arestaB->segundo;
}

int main() {
    // Nomes dos arquivos
    std::string arquivo_entrada = "../entradas/grafo.txt";
    std::string arquivo_saida = arquivo_entrada + "-sanitizado.txt";

    // Abrir arquivo de entrada
    std::ifstream entrada(arquivo_entrada.c_str());
    if (!entrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << arquivo_entrada << std::endl;
        return 1;
    }

    // Abrir arquivo de saída
    std::ofstream saida(arquivo_saida.c_str());
    if (!saida.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de saída: " << arquivo_saida << std::endl;
        entrada.close();
        return 1;
    }

    // Copiar o cabeçalho (primeiras 4 linhas)
    std::string linha;
    for (int i = 0; i < 4; ++i) {
        if (std::getline(entrada, linha)) {
            saida << linha << std::endl;
        } else {
            std::cerr << "O arquivo de entrada tem menos de 4 linhas." << std::endl;
            entrada.close();
            saida.close();
            return 1;
        }
    }

    // Vetor para armazenar arestas
    Vetor<Par> arestas;

    // Ler todas as arestas
    int origem, destino;
    while (entrada >> origem >> destino) {
        // Criar um par ordenado (menor ID primeiro)
        Par aresta;
        if (origem < destino) {
            aresta = Par(origem, destino);
        } else {
            aresta = Par(destino, origem);
        }
        arestas.push_back(aresta);
    }

    // Ordenar o vetor de arestas
    if (arestas.size() > 0) {
        qsort(&arestas[0], arestas.size(), sizeof(Par), compararArestas);
    }

    // Escrever arestas únicas no arquivo de saída
    if (arestas.size() > 0) {
        Par ultima = arestas[0];
        saida << ultima.primeiro << "\t" << ultima.segundo << std::endl;
        for (int i = 1; i < arestas.size(); ++i) {
            if (arestas[i] != ultima) {
                ultima = arestas[i];
                saida << ultima.primeiro << "\t" << ultima.segundo << std::endl;
            }
        }
    }

    // Fechar os arquivos
    entrada.close();
    saida.close();

    std::cout << "Arquivo sanitizado salvo como: " << arquivo_saida << std::endl;
    return 0;
}