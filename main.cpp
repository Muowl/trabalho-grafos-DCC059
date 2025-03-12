#include "leitura.h"
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "no.h"
#include "aresta.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main(int argc, char** argv) {
    string arquivo = "../entradas/grafo.txt";
    
    cout << "Inicializando grafo a partir do arquivo: " << arquivo << endl;
    
    // Criar grafo usando listas de adjacência
    leitura leitor(arquivo);
    
    int numNos = leitor.get_num_nos();
    cout << "Número de nós total: " << numNos << endl;
    
    // Obter arestas lidas do arquivo
    const Vetor<std::pair<int, int>>& arestas = leitor.get_arestas();
    cout << "Número de arestas total: " << arestas.size() << endl;
    
    // Exibir as conexões do nó 0 com pesos sintéticos
    cout << "\nConexões do nó 0 com pesos sintéticos (do arquivo):" << endl;
    cout << "0 -> ";
    bool temAdjacente = false;
    
    // Obter arestas com pesos
    const Vetor<EdgeData>& arestas_com_peso = leitor.get_arestas_com_peso();
    
    for (int i = 0; i < arestas_com_peso.size(); i++) {
        if (arestas_com_peso[i].origem == 0) {
            cout << arestas_com_peso[i].destino << "(peso:" 
                 << fixed << setprecision(1) << arestas_com_peso[i].peso << ") ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente)";
    }
    cout << endl;
    
    // Teste usando o grafo de lista de adjacências ponderado
    cout << "\nTeste usando a implementação do grafo ponderado com lista de adjacências:" << endl;
    
    // Criando grafo com todos os nós - agora ponderado
    GrafoLista grafo(numNos, true, true, "Grafo AGMG - Lista"); // Direcionado e ponderado
    
    // Adicionar apenas as arestas que saem do nó 0 para economizar memória, com seus pesos
    int arestasAdicionadas = 0;
    for (int i = 0; i < arestas_com_peso.size(); i++) {
        if (arestas_com_peso[i].origem == 0) {
            grafo.adicionarAresta(
                arestas_com_peso[i].origem, 
                arestas_com_peso[i].destino, 
                arestas_com_peso[i].peso
            );
            arestasAdicionadas++;
        }
    }
    
    cout << "Arestas adicionadas ao grafo (partindo do nó 0): " << arestasAdicionadas << endl;
    
    // Verificar e mostrar adjacências do nó 0 usando o método existeAresta e getPesoAresta
    cout << "Nó 0 -> ";
    temAdjacente = false;
    
    for (int j = 0; j < numNos; j++) {
        if (grafo.existeAresta(0, j)) {
            float peso = grafo.getPesoAresta(0, j);
            cout << j << "(peso:" << fixed << setprecision(1) << peso << ") ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente)";
    }
    cout << endl;
    
        // Modificação no teste com matriz de adjacências
    cout << "\nTeste usando a implementação do grafo ponderado com matriz de adjacências:" << endl;

    // Criar grafo com tamanho inicial pequeno e carregar do arquivo (compactado)
    GrafoMatriz grafoMatriz(10, true, true, "Grafo AGMG - Matriz"); // Tamanho inicial pequeno
    if (grafoMatriz.carregarDoArquivo(arquivo)) {
        cout << "Grafo carregado com sucesso usando representação compacta" << endl;
        
        // Verificar e mostrar adjacências do nó 0 
        cout << "Nó 0 -> ";
        temAdjacente = false;
        
        for (int j = 0; j < grafoMatriz.getNumVertices(); j++) {
            // Note: Aqui estamos trabalhando com o índice compacto, não com o ID original
            if (grafoMatriz.existeAresta(0, j)) {
                float peso = grafoMatriz.getPesoAresta(0, j);
                cout << j << "(peso:" << fixed << setprecision(1) << peso << ") ";
                temAdjacente = true;
            }
        }
        
        if (!temAdjacente) {
            cout << "(nenhum adjacente)";
        }
        cout << endl;
    } else {
        cout << "Erro ao carregar o grafo do arquivo." << endl;
    }
    return 0;
}