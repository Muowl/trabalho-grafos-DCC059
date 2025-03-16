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
    
    // Verificar se o grafo está vazio
    if (!grafo || grafo->getNumVertices() <= 0) {
        std::cout << "Algoritmo Relativo: Grafo vazio ou inválido." << std::endl;
        return;
    }
    
    // Para grafos muito grandes, usar abordagem simplificada
    if (grafo->getNumVertices() > 10000) {
        std::cout << "Algoritmo Relativo: Grafo muito grande, usando abordagem simplificada." << std::endl;
        
        // Usar o algoritmo inicial para estabelecer comunidades base
        inicializarComAlgoritmo();
        
        // Para grafos muito grandes, limitamos o refinamento
        int limiteVertices = std::min(1000, grafo->getNumVertices());
        int maxIter = std::min(3, maxIteracoes); // Reduzir número de iterações
        bool melhorou = true;
        int iteracao = 0;
        
        // Refinamento limitado
        while (melhorou && iteracao < maxIter) {
            melhorou = false;
            iteracao++;
            
            // Amostrar vértices distribuídos pelo grafo
            for (int vinc = 0; vinc < limiteVertices; vinc += 5) { // Aumentar passo para amostrar menos
                int v = (vinc * 83) % grafo->getNumVertices(); // Distribuição com primo
                
                // Se o vértice não está atribuído, continuar
                if (v >= atribuicaoVertices.size() || atribuicaoVertices[v] == -1) {
                    continue;
                }
                
                int comunidadeAtual = atribuicaoVertices[v];
                float melhorGanho = 0.0f;
                int melhorComunidade = comunidadeAtual;
                
                // Verificar apenas algumas comunidades (vizinhas)
                Vetor<int> comunidadesVizinhas;
                obterComunidadesVizinhas(v, comunidadesVizinhas, 5);
                
                // Avaliar ganho para cada comunidade vizinha
                for (int c = 0; c < comunidadesVizinhas.size(); c++) {
                    int comunidadeDestino = comunidadesVizinhas[c];
                    if (comunidadeDestino == comunidadeAtual) continue;
                    
                    float ganho = calcularGanhoModularidade(v, comunidadeAtual, comunidadeDestino);
                    
                    if (ganho > melhorGanho) {
                        melhorGanho = ganho;
                        melhorComunidade = comunidadeDestino;
                    }
                }
                
                // Se encontrou uma movimentação que melhora
                if (melhorGanho > limiarMelhoria && melhorComunidade != comunidadeAtual) {
                    // Mover vértice para nova comunidade
                    atribuicaoVertices[v] = melhorComunidade;
                    comunidades[melhorComunidade].adicionarVertice(v);
                    melhorou = true;
                }
            }
            
            std::cout << "Algoritmo Relativo: Iteração " << iteracao << " concluída." << std::endl;
        }
        
        std::cout << "Algoritmo Relativo: Refinamento simplificado concluído após " << iteracao << " iterações." << std::endl;
    }
    else {
        // Para grafos menores, usar a abordagem completa
        inicializarComAlgoritmo();
        refinarComunidades();
    }
    
    std::cout << "Algoritmo Relativo: Detectou " << comunidades.size() << " comunidades." << std::endl;
}

// Método auxiliar para obter comunidades vizinhas de um vértice
void AlgoritmoRelativo::obterComunidadesVizinhas(int vertice, Vetor<int>& comunidadesVizinhas, int limite) {
    comunidadesVizinhas.clear();
    int comunidadeAtual = atribuicaoVertices[vertice];
    
    // Adicionar a comunidade atual
    comunidadesVizinhas.push_back(comunidadeAtual);
    
    // ALTERAÇÃO: Contar conexões para cada comunidade para priorizar as mais conectadas
    Vetor<std::pair<int, int>> comunidadesConexoes; // <id_comunidade, num_conexoes>
    
    // Verificar comunidades dos vizinhos
    int vizinhosVerificados = 0;
    for (int i = 0; i < grafo->getNumVertices() && vizinhosVerificados < 100; i++) {
        if (grafo->existeAresta(vertice, i)) {
            vizinhosVerificados++;
            int comunidadeVizinho = atribuicaoVertices[i];
            
            if (comunidadeVizinho == -1 || comunidadeVizinho == comunidadeAtual) {
                continue;
            }
            
            // Verificar se já contamos essa comunidade
            bool encontrado = false;
            for (int j = 0; j < comunidadesConexoes.size(); j++) {
                if (comunidadesConexoes[j].first == comunidadeVizinho) {
                    comunidadesConexoes[j].second++;
                    encontrado = true;
                    break;
                }
            }
            
            // Se não, adicionar a nova comunidade
            if (!encontrado) {
                comunidadesConexoes.push_back(std::make_pair(comunidadeVizinho, 1));
            }
        }
    }
    
    // Ordenar comunidades por número de conexões (ordenação simples)
    for (int i = 0; i < comunidadesConexoes.size(); i++) {
        for (int j = i + 1; j < comunidadesConexoes.size(); j++) {
            if (comunidadesConexoes[i].second < comunidadesConexoes[j].second) {
                auto temp = comunidadesConexoes[i];
                comunidadesConexoes[i] = comunidadesConexoes[j];
                comunidadesConexoes[j] = temp;
            }
        }
    }
    
    // Adicionar comunidades mais conectadas primeiro (até o limite)
    for (int i = 0; i < comunidadesConexoes.size() && comunidadesVizinhas.size() < limite; i++) {
        comunidadesVizinhas.push_back(comunidadesConexoes[i].first);
    }
    
    // Se não tiver comunidades suficientes, adicionar algumas aleatórias
    int tentativas = 0;
    while (comunidadesVizinhas.size() < limite && tentativas < 10 && comunidades.size() > 1) {
        tentativas++;
        int comAleatoria = std::rand() % comunidades.size();
        
        // Verificar se já está na lista
        bool jaExiste = false;
        for (int j = 0; j < comunidadesVizinhas.size(); j++) {
            if (comunidadesVizinhas[j] == comAleatoria) {
                jaExiste = true;
                break;
            }
        }
        
        // Adicionar se não existir
        if (!jaExiste) {
            comunidadesVizinhas.push_back(comAleatoria);
        }
    }
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
    float modularidadeInicial = avaliarQualidade();
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
                // NOVO: Remover vértice da comunidade original
                // Encontramos o índice do vértice na comunidade original para removê-lo
                Comunidade& comOriginal = comunidades[comunidadeAtual];
                for (int i = 0; i < comOriginal.getTamanho(); i++) {
                    if (comOriginal.getVertice(i) == v) {
                        // Simulamos a remoção trocando com o último e redimensionando
                        comOriginal.removerVertice(i);
                        break;
                    }
                }
                
                // Adiciona vértice à nova comunidade
                comunidades[melhorComunidade].adicionarVertice(v);
                atribuicaoVertices[v] = melhorComunidade;
                
                melhorou = true;
            }
        }
        
        // Calcular nova modularidade sem output
        float novaModularidade = grafo->getNumVertices() > 10000 ? 0.5f : avaliarQualidade();
        std::cout << "Iteração " << iteracao << ": Avaliação = " << novaModularidade 
                  << " (melhoria: " << (novaModularidade - modularidadeInicial) << ")" << std::endl;
        
        // NOVO: Remover comunidades vazias
        Vetor<Comunidade> comunidadesNaoVazias;
        // Fix: Initialize the vector and then resize it with default values
        Vetor<int> mapeamento;
        mapeamento.resize(comunidades.size(), -1); // Mapeia IDs antigos para novos
        
        for (int i = 0; i < comunidades.size(); i++) {
            if (comunidades[i].getTamanho() > 0) {
                mapeamento[i] = comunidadesNaoVazias.size();
                comunidadesNaoVazias.push_back(comunidades[i]);
            }
        }
        
        // Atualizar IDs de comunidades no vetor de atribuição
        for (int i = 0; i < atribuicaoVertices.size(); i++) {
            if (atribuicaoVertices[i] != -1) {
                atribuicaoVertices[i] = mapeamento[atribuicaoVertices[i]];
            }
        }
        
        // Substituir o vetor de comunidades
        comunidades = comunidadesNaoVazias;
        
        // Atualiza modularidade atual
        modularidadeInicial = novaModularidade; // Use the variable to track progress between iterations
    }
    
    std::cout << "Algoritmo Relativo: Refinamento concluído após " << iteracao 
              << " iterações. Comunidades finais: " << comunidades.size() << std::endl;
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
