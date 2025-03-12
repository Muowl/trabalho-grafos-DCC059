#include "leitura.h"
#include "grafo_lista.h"
#include "no.h"
#include "aresta.h"
#include <iostream>
#include <string>

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
    
    // Exibir as conexões do nó 0 diretamente das arestas lidas
    cout << "\nConexões do nó 0 (do arquivo):" << endl;
    cout << "0 -> ";
    bool temAdjacente = false;
    
    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].first == 0) {
            cout << arestas[i].second << " ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente)";
    }
    cout << endl;
    
    // Teste usando o grafo de lista de adjacências
    cout << "\nTeste usando a implementação do grafo com lista de adjacências:" << endl;
    
    // Criando grafo com todos os nós
    GrafoLista grafo(numNos, true); // Grafo direcionado
    
    // Adicionar apenas as arestas que saem do nó 0 para economizar memória
    int arestasAdicionadas = 0;
    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].first == 0) {
            grafo.adicionarAresta(arestas[i].first, arestas[i].second);
            arestasAdicionadas++;
        }
    }
    
    cout << "Arestas adicionadas ao grafo (partindo do nó 0): " << arestasAdicionadas << endl;
    
    // Verificar e mostrar adjacências do nó 0 usando o método existeAresta
    cout << "Nó 0 -> ";
    temAdjacente = false;
    
    for (int j = 0; j < numNos; j++) {
        if (grafo.existeAresta(0, j)) {
            cout << j << " ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente)";
    }
    cout << endl;
    
    return 0;
}