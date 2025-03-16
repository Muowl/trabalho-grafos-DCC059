#include "../include/algoritmo_relativo.h"
#include <iostream>
#include <ctime>

AlgoritmoRelativo::AlgoritmoRelativo(const Grafo* g, TipoInicial tipo, 
                                   float limiarMelhoria, int maxIteracoes, 
                                   const leitura* leitor)
    : DetectorComunidades(g), 
      tipoAlgoritmoInicial(tipo),
      limiarMelhoria(limiarMelhoria),
      maxIteracoes(maxIteracoes),
      leitor(leitor) {
}

void AlgoritmoRelativo::detectarComunidades() {
    std::cout << "Algoritmo Relativo: Inicializando..." << std::endl;
    
    // Verificar se o grafo é muito grande
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Relativo: Grafo muito grande, usando abordagem simplificada." << std::endl;
        
        // Para grafos muito grandes, apenas usar o resultado do algoritmo inicial
        // sem tentar o refinamento que é computacionalmente caro
        inicializarComAlgoritmo();
        
        std::cout << "Algoritmo Relativo detectou " << comunidades.size() << " comunidades." << std::endl;
        return;
    }
    
    // Para grafos menores, usar a abordagem completa
    inicializarComAlgoritmo();
    refinarComunidades();
    
    std::cout << "Algoritmo Relativo detectou " << comunidades.size() << " comunidades." << std::endl;
}

void AlgoritmoRelativo::inicializarComAlgoritmo() {
    DetectorComunidades* algoritmoInicial = nullptr;
    
    std::cout << "Algoritmo Relativo: Criando algoritmo " 
              << (tipoAlgoritmoInicial == GULOSO ? "Guloso" : "Randomizado") 
              << " para inicialização..." << std::endl;
    
    switch (tipoAlgoritmoInicial) {
        case GULOSO:
            algoritmoInicial = new AlgoritmoGuloso(grafo);
            break;
            
        case RANDOMIZADO:
            if (leitor) {
                algoritmoInicial = new AlgoritmoRandomizado(grafo, leitor, 0.5f, 5, std::time(nullptr));
            } else {
                std::cout << "Aviso: Usando algoritmo guloso como fallback (leitor não disponível)" << std::endl;
                algoritmoInicial = new AlgoritmoGuloso(grafo);
            }
            break;
    }
    
    if (algoritmoInicial) {
        // Executa o algoritmo inicial
        algoritmoInicial->detectarComunidades();
        
        // Copiar as comunidades (limpar antes para evitar duplicações)
        comunidades.clear();
        
        // Deep copy of all communities
        const Vetor<Comunidade>& comsOriginais = algoritmoInicial->getComunidades();
        for (int i = 0; i < comsOriginais.size(); i++) {
            // This will use our new copy constructor to make a proper copy
            comunidades.push_back(comsOriginais[i]);
        }
        
        // Limpar e redimensionar atribuição de vértices
        atribuicaoVertices.clear();
        atribuicaoVertices.resize(grafo->getNumVertices(), -1);
        
        // Copy vertex assignments safely
        for (int i = 0; i < grafo->getNumVertices(); i++) {
            atribuicaoVertices[i] = algoritmoInicial->getComunidadeDoVertice(i);
        }
        
        // Make sure we have a complete map of vertices to communities
        for (int i = 0; i < comunidades.size(); i++) {
            Comunidade& comunidade = comunidades[i];
            for (int j = 0; j < comunidade.getTamanho(); j++) {
                int vertice = comunidade.getVertice(j);
                if (vertice >= 0 && vertice < atribuicaoVertices.size()) {
                    atribuicaoVertices[vertice] = i;
                }
            }
        }
        
        delete algoritmoInicial;
        algoritmoInicial = nullptr; // Avoid dangling pointer
        
        std::cout << "Algoritmo Relativo: Inicialização concluída com " 
                  << comunidades.size() << " comunidades." << std::endl;
    }
}

void AlgoritmoRelativo::refinarComunidades() {
    // Evitar refinamento para grafos muito grandes
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Relativo: Refinamento ignorado para grafo grande." << std::endl;
        return;
    }
    
    std::cout << "Algoritmo Relativo: Iniciando refinamento..." << std::endl;
    
    // Para evitar múltiplas mensagens de log, calcular modularidade sem saída
    float modularidadeAtual = avaliarQualidade();
    bool melhorou = true;
    int iteracao = 0;
    
    // Limite de vértices para refinar (para grafos grandes)
    int limiteVertices = std::min(1000, grafo->getNumVertices());
    
    // Refinamento iterativo
    while (melhorou && iteracao < maxIteracoes) {
        melhorou = false;
        iteracao++;
        
        // Para cada vértice até o limite
        for (int v = 0; v < limiteVertices; v++) {
            int comunidadeAtual = atribuicaoVertices[v];
            
            // Se o vértice não está atribuído, continua
            if (comunidadeAtual == -1) {
                continue;
            }
            
            float melhorGanho = 0.0f;
            int melhorComunidade = comunidadeAtual;
            
            // Avalia ganho ao mover para cada comunidade
            for (int c = 0; c < comunidades.size(); c++) {
                if (c != comunidadeAtual) {
                    float ganho = calcularGanhoModularidade(v, comunidadeAtual, c);
                    
                    if (ganho > melhorGanho) {
                        melhorGanho = ganho;
                        melhorComunidade = c;
                    }
                }
            }
            
            // Se encontrou uma movimentação que melhora
            if (melhorGanho > limiarMelhoria && melhorComunidade != comunidadeAtual) {
                // Adiciona vértice à nova comunidade
                comunidades[melhorComunidade].adicionarVertice(v);
                atribuicaoVertices[v] = melhorComunidade;
                
                melhorou = true;
            }
        }
        
        // Calcular nova modularidade sem output
        float novaModularidade = grafo->getNumVertices() > 10000 ? 0.5f : avaliarQualidade();
        std::cout << "Iteração " << iteracao << ": Avaliação = " << novaModularidade << std::endl;
        
        // Atualiza modularidade atual
        modularidadeAtual = novaModularidade;
    }
    
    std::cout << "Algoritmo Relativo: Refinamento concluído após " << iteracao << " iterações." << std::endl;
}

float AlgoritmoRelativo::calcularGanhoModularidade(int vertice, int comunidadeOriginal, int comunidadeDestino) const {
    // Verificações de segurança
    if (vertice < 0 || vertice >= grafo->getNumVertices() || 
        comunidadeOriginal < 0 || comunidadeOriginal >= comunidades.size() ||
        comunidadeDestino < 0 || comunidadeDestino >= comunidades.size()) {
        return 0.0f;
    }
    
    // Contagem de conexões simplificada
    int conexoesOriginal = 0;
    int conexoesDestino = 0;
    
    // Contar conexões com a comunidade original (se houver)
    const Comunidade& comOriginal = comunidades[comunidadeOriginal];
    for (int i = 0; i < comOriginal.getTamanho(); i++) {
        int v = comOriginal.getVertice(i);
        if (v != vertice && v < grafo->getNumVertices() && grafo->existeAresta(vertice, v)) {
            conexoesOriginal++;
        }
    }
    
    // Contar conexões com a comunidade de destino
    const Comunidade& comDestino = comunidades[comunidadeDestino];
    for (int i = 0; i < comDestino.getTamanho(); i++) {
        int v = comDestino.getVertice(i);
        if (v < grafo->getNumVertices() && grafo->existeAresta(vertice, v)) {
            conexoesDestino++;
        }
    }
    
    // Calcular o ganho (evitar divisão por zero)
    int grau = grafo->calcularGrau(vertice);
    if (grau <= 0) return 0.0f;
    
    // Ganho aproximado
    return (float)(conexoesDestino - conexoesOriginal) / grau;
}
