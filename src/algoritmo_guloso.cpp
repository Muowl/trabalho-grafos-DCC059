#include "../include/algoritmo_guloso.h"
#include <iostream>

// Reduzir ainda mais o limiar para grafos pequenos/esparsos
AlgoritmoGuloso::AlgoritmoGuloso(const Grafo* g, float limiarDensidade, int limiteNos) 
    : DetectorComunidades(g), limiarDensidade(limiarDensidade), limiteNos(limiteNos) {
}

void AlgoritmoGuloso::detectarComunidades() {
    // Limpa comunidades existentes
    comunidades.clear();
    
    // Reinicializa vetor de atribuição
    atribuicaoVertices.clear();
    
    // Verificação de segurança - não prosseguir se o grafo estiver vazio
    if (!grafo || grafo->getNumVertices() <= 0) {
        std::cout << "Algoritmo Guloso: Grafo vazio ou inválido." << std::endl;
        return;
    }

    // Definir tamanho do vetor de atribuição
    atribuicaoVertices.resize(grafo->getNumVertices(), -1);
    
    // Para grafos muito grandes, usar abordagem simplificada
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Guloso: Grafo grande (" << grafo->getNumVertices() 
                  << " vértices), usando abordagem simplificada." << std::endl;
        
        // Número de amostras para análise
        int amostras = std::min(1000, grafo->getNumVertices());
        int passo = grafo->getNumVertices() / amostras;
        if (passo < 1) passo = 1;
        
        // Contador para número de comunidades
        int idComunidade = 0;
        
        // Amostrar vértices distribuídos pelo grafo
        for (int i = 0; i < grafo->getNumVertices() && idComunidade < 50; i += passo) {
            // Se o vértice já está atribuído, pular
            if (i < atribuicaoVertices.size() && atribuicaoVertices[i] != -1) {
                continue;
            }
            
            // Criar uma nova comunidade
            Comunidade novaComunidade(idComunidade);
            novaComunidade.adicionarVertice(i);
            atribuicaoVertices[i] = idComunidade;
            
            // Expandir a comunidade a partir deste vértice (limitado)
            expandirComunidade(idComunidade, i, 100);
            
            // Adicionar à lista de comunidades
            comunidades.push_back(novaComunidade);
            idComunidade++;
        }
        
        // Se não criamos comunidades suficientes, garantir que temos pelo menos uma
        if (comunidades.size() == 0) {
            Comunidade comunidadeDemo(0);
            comunidadeDemo.adicionarVertice(0);
            comunidades.push_back(comunidadeDemo);
            atribuicaoVertices[0] = 0;
            std::cout << "Algoritmo Guloso: Criada uma comunidade de demonstração com " 
                      << comunidades[0].getTamanho() << " vértices." << std::endl;
        }
    }
    else {
        // Abordagem completa para grafos menores
        int nodesLimit = std::min(limiteNos, grafo->getNumVertices());
        
        // Contador para comunidades
        int idComunidade = 0;
        
        // MODIFICAÇÃO: Usar os nós de maior grau como sementes para comunidades
        Vetor<No> nosOrdenadosPorGrau = grafo->getNodesComGrau();
        
        // MODIFICAÇÃO: Algoritmo de ordenação mais seguro para `nosOrdenadosPorGrau`
        // Criar um vetor e copiar os elementos ordenados de volta para o vetor original
        if (grafo->getNumVertices() <= 10000) {
            Vetor<No> nosOrdenadosPorGrau = grafo->getNodesComGrau();
            
            // CORRIGIDO: Algoritmo de ordenação mais seguro que evita problemas de uso-após-liberação
            for (int i = 0; i < nosOrdenadosPorGrau.size(); i++) {
                for (int j = i + 1; j < nosOrdenadosPorGrau.size(); j++) {
                    if (nosOrdenadosPorGrau[i].getGrau() < nosOrdenadosPorGrau[j].getGrau()) {
                        // Criar uma cópia temporária e usar atribuição adequada
                        No temp = nosOrdenadosPorGrau[i]; // Construtor de cópia
                        nosOrdenadosPorGrau[i] = nosOrdenadosPorGrau[j]; // Operador de atribuição
                        nosOrdenadosPorGrau[j] = temp; // Operador de atribuição
                    }
                }
            }
            
            // Continuar com o resto do algoritmo usando o vetor ordenado
            int maxComunidadesIniciais = std::min(grafo->getNumVertices() / 4, 20);
            if (maxComunidadesIniciais < 2) maxComunidadesIniciais = 2;
            
            // Criar comunidades a partir dos vértices de maior grau
            for (int i = 0; i < nosOrdenadosPorGrau.size() && idComunidade < maxComunidadesIniciais; i++) {
                int vertice = nosOrdenadosPorGrau[i].getId();
                
                // Se o vértice já está atribuído, pular
                if (atribuicaoVertices[vertice] != -1) {
                    continue;
                }
                
                // Criar uma nova comunidade
                Comunidade novaComunidade(idComunidade);
                novaComunidade.adicionarVertice(vertice);
                atribuicaoVertices[vertice] = idComunidade;
                
                // Expandir a comunidade a partir do vértice inicial
                expandirComunidade(idComunidade, vertice, nodesLimit);
                
                // Adicionar a comunidade ao resultado somente se não estiver vazia
                if (novaComunidade.getTamanho() > 0) {
                    comunidades.push_back(novaComunidade);
                    idComunidade++;
                }
            }
        }
        
        // NOVO: Segunda fase para atribuir vértices restantes
        bool mudou = true;
        while (mudou) {
            mudou = false;
            
            for (int v = 0; v < grafo->getNumVertices(); v++) {
                // Se o vértice já está atribuído, pular
                if (atribuicaoVertices[v] != -1) {
                    continue;
                }
                
                // Encontrar a melhor comunidade para este vértice
                int melhorComunidade = -1;
                int maxConexoes = -1;
                
                for (int c = 0; c < comunidades.size(); c++) {
                    int conexoes = 0;
                    for (int i = 0; i < comunidades[c].getTamanho(); i++) {
                        int u = comunidades[c].getVertice(i);
                        if (grafo->existeAresta(v, u)) {
                            conexoes++;
                        }
                    }
                    
                    if (conexoes > maxConexoes) {
                        maxConexoes = conexoes;
                        melhorComunidade = c;
                    }
                }
                
                // Se encontrou alguma comunidade com conexões
                if (melhorComunidade != -1 && maxConexoes > 0) {
                    comunidades[melhorComunidade].adicionarVertice(v);
                    atribuicaoVertices[v] = melhorComunidade;
                    mudou = true;
                }
            }
        }
        
        // NOVO: Se ainda há vértices não atribuídos, criar comunidades para eles
        for (int v = 0; v < grafo->getNumVertices(); v++) {
            if (atribuicaoVertices[v] == -1) {
                Comunidade novaComunidade(idComunidade);
                novaComunidade.adicionarVertice(v);
                atribuicaoVertices[v] = idComunidade;
                comunidades.push_back(novaComunidade);
                idComunidade++;
            }
        }
    }
}

void AlgoritmoGuloso::expandirComunidade(int idComunidade, int verticeInicial, int nodesLimit) {
    // Validações básicas
    if (idComunidade < 0 || verticeInicial < 0 || verticeInicial >= grafo->getNumVertices()) {
        return;
    }
    
    // Inicializar com vértice inicial - fila para BFS
    Vetor<int> fila;
    fila.push_back(verticeInicial);
    
    // Contador de nós processados
    int nosProcessados = 0;
    
    // BFS limitado para encontrar comunidade
    while (fila.size() > 0 && nosProcessados < nodesLimit) {
        int verticeAtual = fila[0];
        
        // Simular pop_front (removendo o primeiro elemento)
        Vetor<int> novaFila;
        for (int i = 1; i < fila.size(); i++) {
            novaFila.push_back(fila[i]);
        }
        fila = novaFila;
        
        nosProcessados++;
        
        // MODIFICAÇÃO: Verificar apenas vizinhos diretos para melhor eficiência
        for (int i = 0; i < grafo->getNumVertices(); i++) {
            // Se é vizinho e não está atribuído a nenhuma comunidade
            if (grafo->existeAresta(verticeAtual, i) && atribuicaoVertices[i] == -1) {
                // MODIFICAÇÃO: Usar um critério mais relaxado para adicionar à comunidade
                if (deveAdicionarVertice(idComunidade, i)) {
                    comunidades[idComunidade].adicionarVertice(i);
                    atribuicaoVertices[i] = idComunidade;
                    fila.push_back(i);
                }
            }
        }
    }
}

bool AlgoritmoGuloso::deveAdicionarVertice(int idComunidade, int vertice) const {
    // Validações básicas
    if (idComunidade < 0 || idComunidade >= comunidades.size() || 
        vertice < 0 || vertice >= grafo->getNumVertices()) {
        return false;
    }
    
    // Contar conexões com a comunidade
    int conexoes = 0;
    int totalVerticesNaComunidade = comunidades[idComunidade].getTamanho();
    
    // MODIFICAÇÃO: Para grafos pequenos, usar critério muito mais flexível
    if (totalVerticesNaComunidade <= 5 || grafo->getNumVertices() < 100) {
        // Para comunidades pequenas ou grafos pequenos, basta ter uma única conexão
        for (int i = 0; i < totalVerticesNaComunidade; i++) {
            if (grafo->existeAresta(vertice, comunidades[idComunidade].getVertice(i))) {
                return true; // Uma única conexão é suficiente
            }
        }
    }
    
    // Limitar análise para performance
    int limiteAnalise = std::min(100, totalVerticesNaComunidade);
    
    // Verificar conexões com os vértices da comunidade
    for (int i = 0; i < limiteAnalise; i++) {
        int v = comunidades[idComunidade].getVertice(i);
        if (grafo->existeAresta(vertice, v)) {
            conexoes++;
        }
    }
    
    // MODIFICAÇÃO: Calcular limiar dinamicamente baseado no tamanho do grafo
    float limiarAjustado = grafo->getNumVertices() < 100 ? 0.01f : 
                          (grafo->getNumVertices() < 500 ? 0.05f : limiarDensidade);
    
    // Calcular densidade local - proporção de conexões com a comunidade
    float densidadeLocal = totalVerticesNaComunidade > 0 ? 
                          (float)conexoes / totalVerticesNaComunidade : 0;
    
    // Retornar true se densidade for maior que o limiar ajustado
    return densidadeLocal >= limiarAjustado;
}
