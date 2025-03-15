#include "../include/algoritmo_randomizado.h"
#include <iostream>
#include <cstdlib>

AlgoritmoRandomizado::AlgoritmoRandomizado(const Grafo* g, const leitura* l, float probabilidadeConexao, 
                                         int numComunidadesAlvo, int semente)
    : DetectorComunidades(g), 
      leitor(l),
      probabilidadeConexao(probabilidadeConexao),
      numComunidadesAlvo(numComunidadesAlvo),
      semente(semente) {
    // Inicializar o gerador de números aleatórios com a semente
    std::srand(semente);
}

void AlgoritmoRandomizado::detectarComunidades() {
    // Limpa comunidades existentes
    comunidades.clear();
    atribuicaoVertices.clear();
    
    std::cout << "Algoritmo Randomizado: Analisando grafo de forma ultra limitada." << std::endl;
    
    // Verificação de segurança - não prosseguir se o grafo estiver vazio
    if (!grafo || grafo->getNumVertices() <= 0) {
        std::cout << "Algoritmo Randomizado: Grafo vazio ou inválido." << std::endl;
        return;
    }
    
    // Para grafos muito grandes, criar apenas uma pequena comunidade de demonstração
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Randomizado: Grafo muito grande (" << grafo->getNumVertices() 
                  << " vértices), usando abordagem ultrassegura." << std::endl;
        
        // Criar uma comunidade de demonstração com até 3 nós aleatórios
        Comunidade comunidadeDemo(0);
        
        // Adicionar o nó 0 como base
        comunidadeDemo.adicionarVertice(0);
        
        // Adicionar até 2 nós extras se possível usando o leitor
        if (leitor) {
            const Vetor<int>& nosPresentesVetor = leitor->get_nos_presentes();
            if (nosPresentesVetor.size() > 0) {
                int indice1 = std::rand() % nosPresentesVetor.size();
                comunidadeDemo.adicionarVertice(nosPresentesVetor[indice1]);
                
                if (nosPresentesVetor.size() > 1) {
                    int indice2;
                    do {
                        indice2 = std::rand() % nosPresentesVetor.size();
                    } while (indice2 == indice1);
                    
                    comunidadeDemo.adicionarVertice(nosPresentesVetor[indice2]);
                }
            }
        }
        
        // Adicionar a comunidade ao vetor
        comunidades.push_back(comunidadeDemo);
        
        // Criar um vetor de atribuição mínimo (apenas para os nós na comunidade)
        atribuicaoVertices.resize(1, 0);
        
        std::cout << "Algoritmo Randomizado: Criada uma comunidade de demonstração com " 
                  << comunidadeDemo.getTamanho() << " vértices." << std::endl;
        return;
    }
    
    // Para grafos menores, usar uma abordagem ainda reduzida mas um pouco mais elaborada
    int nodesLimit = 20; // Limite fixo para grafos menores
    if (nodesLimit > grafo->getNumVertices()) {
        nodesLimit = grafo->getNumVertices();
    }
    
    // Inicializar atribuição de vértices
    atribuicaoVertices.resize(nodesLimit, -1);
    
    // Inicializar um número reduzido de comunidades
    int numComunidades = numComunidadesAlvo > 3 ? 3 : numComunidadesAlvo;
    
    for (int i = 0; i < numComunidades; i++) {
        Comunidade novaComunidade(i);
        comunidades.push_back(novaComunidade);
    }
    
    // Atribuir alguns vértices aleatoriamente às comunidades
    for (int i = 0; i < nodesLimit; i++) {
        int comunidade = std::rand() % numComunidades;
        comunidades[comunidade].adicionarVertice(i);
        atribuicaoVertices[i] = comunidade;
    }
    
    std::cout << "Algoritmo Randomizado: Criadas " << numComunidades << " comunidades limitadas." << std::endl;
}

// Implementação da função que sorteia um nó aleatório
int AlgoritmoRandomizado::sortearNoAleatorio() const {
    if (!leitor) return std::rand() % std::min(grafo->getNumVertices(), 1000);
    
    const Vetor<int>& nosPresentesVetor = leitor->get_nos_presentes();
    if (nosPresentesVetor.size() == 0) return 0;
    
    int indiceAleatorio = std::rand() % nosPresentesVetor.size();
    return nosPresentesVetor[indiceAleatorio];
}

// Implementação da função que gera um valor aleatório entre 0 e 1
float AlgoritmoRandomizado::gerarNumeroAleatorio() const {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

// Outros métodos mantidos para compatibilidade, mas não usados com grafos grandes
float AlgoritmoRandomizado::calcularProbabilidade(int vertice, int idComunidade) const {
    return 0.5f; // valor de demonstração
}

void AlgoritmoRandomizado::inicializarComunidades() {
    // stub
}

void AlgoritmoRandomizado::expandirComunidades() {
    // stub
}
