#include "../include/algoritmo_randomizado.h"
#include <iostream>
#include <cstdlib>


AlgoritmoRandomizado::AlgoritmoRandomizado(const Grafo* g, const leitura* l, float probabilidadeConexao, 
                                         int numComunidadesAlvo, int semente)
    : DetectorComunidades(g), 
      probabilidadeConexao(probabilidadeConexao),
      numComunidadesAlvo(numComunidadesAlvo),
      semente(semente),
      leitor(l) {
    // Inicializar o gerador de números aleatórios com a semente
    std::srand(semente);
}

void AlgoritmoRandomizado::detectarComunidades() {
    // Limpa comunidades existentes
    comunidades.clear();
    atribuicaoVertices.clear();
    
    // Verificação de segurança - não prosseguir se o grafo estiver vazio
    if (!grafo || grafo->getNumVertices() <= 0) {
        std::cout << "Algoritmo Randomizado: Grafo vazio ou inválido." << std::endl;
        return;
    }
    
    // Redimensionar vetor de atribuição para todos os vértices
    atribuicaoVertices.resize(grafo->getNumVertices(), -1);
    
    // Para grafos muito grandes, usar abordagem simplificada
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Randomizado: Grafo grande (" << grafo->getNumVertices() 
                  << " vértices), usando abordagem simplificada." << std::endl;
        
        // Número de comunidades e tamanho de amostragem
        int comunidadesAlvo = std::min(this->numComunidadesAlvo, 10);
        int tamanhoAmostra = std::min(3000, grafo->getNumVertices());
        
        // Inicializar comunidades
        for (int i = 0; i < comunidadesAlvo; i++) {
            Comunidade novaComunidade(i);
            comunidades.push_back(novaComunidade);
        }
        
        // Distribuir vértices de amostra aleatoriamente
        int verticesAdicionados = 0;
        for (int i = 0; i < tamanhoAmostra && verticesAdicionados < 500; i++) {
            // Sortear um nó aleatório
            int vertice;
            if (i % 10 == 0) { 
                // A cada 10 iterações, tente um nó aleatório global
                vertice = sortearNoAleatorio();
            } else { 
                // Nos outros casos, use os índices sequenciais com saltos
                vertice = (i * 97) % grafo->getNumVertices(); // Multiplicar por primo para distribuição
            }
            
            // Sortear uma comunidade aleatória
            int comunidade = std::rand() % comunidadesAlvo;
            
            // Adicionar o vértice à comunidade se não estiver atribuído
            if (vertice >= 0 && vertice < grafo->getNumVertices() && 
                comunidade >= 0 && comunidade < comunidades.size() && 
                atribuicaoVertices[vertice] == -1) {
                
                comunidades[comunidade].adicionarVertice(vertice);
                atribuicaoVertices[vertice] = comunidade;
                verticesAdicionados++;
                
                // Tentar adicionar vizinhos diretos
                if (i % 3 == 0) { // A cada 3 iterações
                    adicionarVizinhosDiretos(vertice, comunidade);
                }
            }
        }
        
        // Remover comunidades vazias
        Vetor<Comunidade> comunidadesValidas;
        for (int i = 0; i < comunidades.size(); i++) {
            if (comunidades[i].getTamanho() > 0) {
                comunidadesValidas.push_back(comunidades[i]);
            }
        }
        
        // Garantir pelo menos uma comunidade
        if (comunidadesValidas.size() == 0) {
            Comunidade comunidadeDemo(0);
            comunidadeDemo.adicionarVertice(0);
            comunidadesValidas.push_back(comunidadeDemo);
        }
        
        // Atualizar comunidades
        comunidades = comunidadesValidas;
        
    }
    else {
        // Abordagem completa para grafos menores
        inicializarComunidades();
        expandirComunidades();
        
    }
}

// Adicionando método auxiliar para adicionar vizinhos diretos
void AlgoritmoRandomizado::adicionarVizinhosDiretos(int vertice, int comunidade) {
    // Adicionar até 5 vizinhos diretos à mesma comunidade
    int vizinhosAdicionados = 0;
    for (int v = 0; v < grafo->getNumVertices() && vizinhosAdicionados < 5; v++) {
        if (grafo->existeAresta(vertice, v) && atribuicaoVertices[v] == -1) {
            // Com probabilidade baseada na probabilidadeConexao
            if (gerarNumeroAleatorio() < probabilidadeConexao) {
                comunidades[comunidade].adicionarVertice(v);
                atribuicaoVertices[v] = comunidade;
                vizinhosAdicionados++;
            }
        }
    }
}

int AlgoritmoRandomizado::sortearNoAleatorio() const {
    if (!leitor) return std::rand() % std::min(grafo->getNumVertices(), 1000);
    
    const Vetor<int>& nosPresentesVetor = leitor->get_nos_presentes();
    if (nosPresentesVetor.size() == 0) return 0;
    
    int indiceAleatorio = std::rand() % nosPresentesVetor.size();
    return nosPresentesVetor[indiceAleatorio];
}

float AlgoritmoRandomizado::gerarNumeroAleatorio() const {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

void AlgoritmoRandomizado::inicializarComunidades() {
    // Criar o número alvo de comunidades
    for (int i = 0; i < numComunidadesAlvo; i++) {
        Comunidade novaComunidade(i);
        
        // Selecionar um vértice inicial aleatório para a comunidade
        int verticeInicial = sortearNoAleatorio();
        
        // Adicionar o vértice à comunidade
        novaComunidade.adicionarVertice(verticeInicial);
        if (verticeInicial < atribuicaoVertices.size()) {
            atribuicaoVertices[verticeInicial] = i;
        }
        
        // Adicionar a comunidade ao vetor
        comunidades.push_back(novaComunidade);
    }
}

void AlgoritmoRandomizado::expandirComunidades() {
    // Número máximo de iterações para evitar loops infinitos
    const int MAX_ITERACOES = 1000;
    
    // Distribuir vértices aleatoriamente para comunidades
    for (int i = 0; i < MAX_ITERACOES && i < grafo->getNumVertices() * 2; i++) {
        int vertice = std::rand() % grafo->getNumVertices();
        
        // Se o vértice já está atribuído, tentar outro
        if (vertice >= atribuicaoVertices.size() || atribuicaoVertices[vertice] != -1) {
            continue;
        }
        
        // Calcular probabilidades para cada comunidade
        float maxProb = 0.0f;
        int melhorComunidade = -1;
        
        for (int c = 0; c < comunidades.size(); c++) {
            float prob = calcularProbabilidade(vertice, c);
            if (prob > maxProb) {
                maxProb = prob;
                melhorComunidade = c;
            }
        }
        
        // Se encontrou uma comunidade adequada
        if (melhorComunidade != -1) {
            // Adicionar o vértice à comunidade
            comunidades[melhorComunidade].adicionarVertice(vertice);
            atribuicaoVertices[vertice] = melhorComunidade;
        }
        // Se não, com probabilidade baixa, criar uma nova comunidade
        else if (gerarNumeroAleatorio() < 0.05f) {
            int novaComunidadeId = comunidades.size();
            Comunidade novaComunidade(novaComunidadeId);
            novaComunidade.adicionarVertice(vertice);
            atribuicaoVertices[vertice] = novaComunidadeId;
            comunidades.push_back(novaComunidade);
        }
    }
}

float AlgoritmoRandomizado::calcularProbabilidade(int vertice, int idComunidade) const {
    // Verificações básicas
    if (idComunidade < 0 || idComunidade >= comunidades.size() || 
        vertice < 0 || vertice >= grafo->getNumVertices()) {
        return 0.0f;
    }
    
    // Contar conexões com a comunidade
    int conexoes = 0;
    int numVerticesAnalisados = 0;
    
    // Limitar análise por eficiência
    for (int i = 0; i < comunidades[idComunidade].getTamanho() && numVerticesAnalisados < 100; i++) {
        int v = comunidades[idComunidade].getVertice(i);
        numVerticesAnalisados++;
        
        if (grafo->existeAresta(vertice, v)) {
            conexoes++;
        }
    }
    
    // Calcular probabilidade baseada em conexões
    float probabilidade = probabilidadeConexao;
    if (numVerticesAnalisados > 0) {
        probabilidade *= (float)conexoes / numVerticesAnalisados;
    }
    
    return probabilidade;
}
