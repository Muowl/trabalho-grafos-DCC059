#include "../include/leitura.h"

leitura::leitura(const std::string &filename) {
    std::ifstream grafotxt(filename);
    if (!grafotxt.is_open()) {
    }
    // ler primeira linha: numero de nos, se é direcionado, se ponderado nos e arestas
    grafotxt >> num_nos >> direcionado >> ponderado_vertices >> ponderado_arestas;

    // se ponderado_vertices for true, ler a linha de pesos dos nós
    if (ponderado_vertices) {
        pesos_nos = new int[num_nos];
        for(int i = 0; i < num_nos; i++){
            grafotxt >> pesos_nos[i];
        }
    } else {
        pesos_nos = NULL;
    }

    // alocar matriz_nos
    matriz_nos = new float*[num_nos];
    for(int i = 0; i < num_nos; i++){
        matriz_nos[i] = new float[num_nos];
        for(int j = 0; j < num_nos; j++){
            matriz_nos[i][j] = 0.0f; 
        }
    }

    // ler as linhas de aresta (origem, destino, peso)
    // lendo a partir da terceira linha
    while(!grafotxt.eof()) {
        int origem, destino;
        if (ponderado_arestas) {
            float peso;
            grafotxt >> origem >> destino >> peso;
            if(!grafotxt.fail()) {
                matriz_nos[origem-1][destino-1] = peso;
                // se não for direcionado, espelhar na outra direção
                if(!direcionado) {
                    matriz_nos[destino-1][origem-1] = peso;
                }
            }
        } else {
            grafotxt >> origem >> destino;
            if(!grafotxt.fail()) {
                matriz_nos[origem-1][destino-1] = 1.0f;
                // se não for direcionado, espelhar na outra direção
                if(!direcionado) {
                    matriz_nos[destino-1][origem-1] = 1.0f;
                }
            }
        }
    }
    grafotxt.close();
}

// exibir a matriz de nós
void leitura::imprimir_matriz_nos() {
    for(int i = 0; i < num_nos; i++){
        for(int j = 0; j < num_nos; j++){
            std::cout << matriz_nos[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
