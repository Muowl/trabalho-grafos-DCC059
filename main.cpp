#include "leitura.h"
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "no.h"
#include "aresta.h"
#include "algoritmo_guloso.h"
#include "algoritmo_randomizado.h"
#include "algoritmo_relativo.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

void testarDeteccaoComunidades(GrafoLista& grafo, const leitura& leitor);
void testarDeteccaoComunidadesMatriz(GrafoMatriz& grafo, const leitura& leitor);
void imprimirResultadosComIdsOriginais(const DetectorComunidades& detector, const Vetor<int>& nosPresentes);

int main(int argc, char** argv) {
    string arquivo = "../entradas/grafo.txt";
    
    cout << "Inicializando grafo a partir do arquivo: " << arquivo << endl;
    
    // Criar grafo usando listas de adjacência
    leitura leitor(arquivo);
    
    // Exibir informações sobre nós presentes
    leitor.exibir_nos_presentes();
    
    // Verificar a presença de alguns nós específicos
    cout << "\nVerificação de nós específicos:" << endl;
    cout << "Nó 0 está presente? " << (leitor.no_esta_presente(0) ? "Sim" : "Não") << endl;
    cout << "Nó 1 está presente? " << (leitor.no_esta_presente(1) ? "Sim" : "Não") << endl;
    cout << "Nó 2 está presente? " << (leitor.no_esta_presente(2) ? "Sim" : "Não") << endl;
    cout << "Nó 3 está presente? " << (leitor.no_esta_presente(3) ? "Sim" : "Não") << endl;
    cout << "Nó 11342 está presente? " << (leitor.no_esta_presente(11342) ? "Sim" : "Não") << endl;
    
    // Restante do código para construir grafo e testar algoritmos...
    
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
    GrafoLista grafo(numNos, false, true, "Grafo AGMG"); // Direcionado e ponderado
    
    // Adicionar arestas relacionadas aos nós 0, 1 e 11342 para demonstrar o cálculo de grau
    int arestasAdicionadas = 0;
    for (int i = 0; i < arestas_com_peso.size(); i++) {
        if (arestas_com_peso[i].origem == 0 || 
            arestas_com_peso[i].origem == 1 || 
            arestas_com_peso[i].destino == 0 ||
            arestas_com_peso[i].destino == 1 ||
            arestas_com_peso[i].origem == 11342 ||    // Adicionado: incluir arestas com origem em 11342
            arestas_com_peso[i].destino == 11342) {   // Adicionado: incluir arestas com destino em 11342
            
            grafo.adicionarAresta(
                arestas_com_peso[i].origem, 
                arestas_com_peso[i].destino, 
                arestas_com_peso[i].peso
            );
            arestasAdicionadas++;
        }
    }
    
    cout << "Arestas adicionadas ao grafo: " << arestasAdicionadas << endl;
    
    // Calcular e exibir o grau do nó 0
    int grauNo0 = grafo.calcularGrau(0);
    cout << "\nGrau do nó 0: " << grauNo0 << endl;
    
    // Calcular e exibir o grau do nó 1
    int grauNo1 = grafo.calcularGrau(1);
    cout << "Grau do nó 1: " << grauNo1 << endl;

    // Calcular e exibir o grau do nó 11342
    int grauNo11342 = grafo.calcularGrau(11342);
    cout << "Grau do nó 11342: " << grauNo11342 << endl;
    
    // Mostrar as conexões específicas do nó 11342 para verificação
    cout << "\nConexões do nó 11342:" << endl;
    cout << "11342 -> ";
    temAdjacente = false;
    
    for (int j = 0; j < numNos; j++) {
        if (grafo.existeAresta(11342, j)) {
            cout << j << " ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente de saída)";
    }
    cout << endl;
    
    // Mostrar conexões de entrada para 11342
    cout << "-> 11342: ";
    temAdjacente = false;
    
    for (int j = 0; j < numNos; j++) {
        if (j != 11342 && grafo.existeAresta(j, 11342)) {
            cout << j << " ";
            temAdjacente = true;
        }
    }
    
    if (!temAdjacente) {
        cout << "(nenhum adjacente de entrada)";
    }
    cout << endl;
    
    // Encontrar e exibir o nó com maior grau entre os adicionados
    cout << "\nBuscando nó com maior grau (entre nós 0 e 1):" << endl;
    No noMaiorGrau = grafo.getNoComMaiorGrauLimitado(2); // Limitar a busca para evitar problemas de performance
    if (noMaiorGrau.getId() != -1) {
        cout << "Nó com maior grau: " << noMaiorGrau.getId() 
             << " (Grau: " << noMaiorGrau.getGrau() << ")" << endl;
    } else {
        cout << "Nenhum nó com conexões encontrado." << endl;
    }
    
    // Modificação no teste com matriz de adjacências
    cout << "\nTeste usando a implementação do grafo ponderado com matriz de adjacências:" << endl;

    // Criar grafo com tamanho inicial pequeno e carregar do arquivo (compactado)
    GrafoMatriz grafoMatriz(10, false, true, "Grafo AGMG - Matriz"); // Tamanho inicial pequeno
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
    
    testarDeteccaoComunidades(grafo, leitor);
    
    // Teste usando GrafoLista
    cout << "\n===== TESTES COM GRAFO LISTA =====" << endl;
    GrafoLista grafoLista(leitor.get_num_nos(), false, true, "Grafo Lista");
    grafoLista.carregarDoArquivo(arquivo);
    testarDeteccaoComunidades(grafoLista, leitor);
    
    // Teste usando GrafoMatriz - Mudei o nome da variável para evitar redeclaração
    cout << "\n===== TESTES COM GRAFO MATRIZ =====" << endl;
    GrafoMatriz grafoMatrizTestes(leitor.get_num_nos(), false, true, "Grafo Matriz");
    grafoMatrizTestes.carregarDoArquivo(arquivo);
    testarDeteccaoComunidadesMatriz(grafoMatrizTestes, leitor);
    
    return 0;
}

// Implementar testes para algoritmos de detecção de comunidades
void testarDeteccaoComunidades(GrafoLista& grafo, const leitura& leitor) {
    cout << "\n=====================================================" << endl;
    cout << "TESTES DE ALGORITMOS DE DETECÇÃO DE COMUNIDADES" << endl;
    cout << "=====================================================" << endl;
    
    // Verificar se o grafo é muito grande para análise completa
    if (grafo.getNumVertices() > 100000) {
        cout << "AVISO: Grafo muito grande (" << grafo.getNumVertices() 
             << " vértices), os algoritmos usarão abordagens ultrasseguras." << endl;
    }
    
    try {
        // Teste do algoritmo guloso com abordagem ultrassegura
        cout << "\n----- Algoritmo Guloso -----" << endl;
        auto inicioGuloso = high_resolution_clock::now();
        
        // Limite extremamente reduzido para 5 nós apenas
        AlgoritmoGuloso guloso(&grafo, 0.3f, 5);  
        guloso.detectarComunidades();
        guloso.imprimirResultados();
        
        auto fimGuloso = high_resolution_clock::now();
        auto duracaoGuloso = duration_cast<milliseconds>(fimGuloso - inicioGuloso);
        cout << "Tempo de execução: " << duracaoGuloso.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo guloso: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo guloso" << endl;
    }
    
    try {
        // Teste do algoritmo randomizado - passando o leitor
        cout << "\n----- Algoritmo Randomizado -----" << endl;
        auto inicioRandom = high_resolution_clock::now();
        AlgoritmoRandomizado randomizado(&grafo, &leitor, 0.5f, 3, time(0));
        randomizado.detectarComunidades();
        randomizado.imprimirResultados();
        auto fimRandom = high_resolution_clock::now();
        auto duracaoRandom = duration_cast<milliseconds>(fimRandom - inicioRandom);
        cout << "Tempo de execução: " << duracaoRandom.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo randomizado: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo randomizado" << endl;
    }
    
    try {
        // Teste do algoritmo relativo - passando o leitor
        cout << "\n----- Algoritmo Relativo -----" << endl;
        auto inicioRelativo = high_resolution_clock::now();
        AlgoritmoRelativo relativo(&grafo, AlgoritmoRelativo::GULOSO, 0.01f, 5, &leitor);
        relativo.detectarComunidades();
        relativo.imprimirResultados();
        auto fimRelativo = high_resolution_clock::now();
        auto duracaoRelativo = duration_cast<milliseconds>(fimRelativo - inicioRelativo);
        cout << "Tempo de execução: " << duracaoRelativo.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo relativo: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo relativo" << endl;
    }
}

// Implementar testes para algoritmos de detecção de comunidades usando GrafoMatriz
void testarDeteccaoComunidadesMatriz(GrafoMatriz& grafo, const leitura& leitor) {
    try {
        // Teste do algoritmo guloso
        cout << "\n----- Algoritmo Guloso (Matriz) -----" << endl;
        auto inicioGuloso = high_resolution_clock::now();
        AlgoritmoGuloso guloso(&grafo, 0.3f, 5);  
        guloso.detectarComunidades();
        // Usar a função customizada para exibir os resultados com IDs originais
        imprimirResultadosComIdsOriginais(guloso, leitor.get_nos_presentes());
        
        auto fimGuloso = high_resolution_clock::now();
        auto duracaoGuloso = duration_cast<milliseconds>(fimGuloso - inicioGuloso);
        cout << "Tempo de execução: " << duracaoGuloso.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo guloso: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo guloso" << endl;
    }
    
    try {
        // Teste do algoritmo randomizado - passando o leitor
        cout << "\n----- Algoritmo Randomizado (Matriz) -----" << endl;
        auto inicioRandom = high_resolution_clock::now();
        AlgoritmoRandomizado randomizado(&grafo, &leitor, 0.5f, 3, time(0));
        randomizado.detectarComunidades();
        // Usar a função customizada para exibir os resultados com IDs originais
        imprimirResultadosComIdsOriginais(randomizado, leitor.get_nos_presentes());
        
        auto fimRandom = high_resolution_clock::now();
        auto duracaoRandom = duration_cast<milliseconds>(fimRandom - inicioRandom);
        cout << "Tempo de execução: " << duracaoRandom.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo randomizado: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo randomizado" << endl;
    }
    
    try {
        // Teste do algoritmo relativo - passando o leitor
        cout << "\n----- Algoritmo Relativo (Matriz) -----" << endl;
        auto inicioRelativo = high_resolution_clock::now();
        AlgoritmoRelativo relativo(&grafo, AlgoritmoRelativo::GULOSO, 0.01f, 5, &leitor);
        relativo.detectarComunidades();
        // Usar a função customizada para exibir os resultados com IDs originais
        imprimirResultadosComIdsOriginais(relativo, leitor.get_nos_presentes());
        
        auto fimRelativo = high_resolution_clock::now();
        auto duracaoRelativo = duration_cast<milliseconds>(fimRelativo - inicioRelativo);
        cout << "Tempo de execução: " << duracaoRelativo.count() << " ms" << endl;
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo relativo: " << e.what() << endl;
    } catch (...) {
        cerr << "Erro desconhecido no algoritmo relativo" << endl;
    }
}

// Função para imprimir resultados convertendo IDs compactos para IDs originais
void imprimirResultadosComIdsOriginais(const DetectorComunidades& detector, const Vetor<int>& nosPresentes) {
    std::cout << "Resultado da Detecção de Comunidades:" << std::endl;
    std::cout << "Número de comunidades: " << detector.getNumComunidades() << std::endl;
    
    // Mostrar métrica de qualidade
    if (detector.getNumComunidades() > 0) {
        std::cout << "Modularidade (Q): " << detector.avaliarQualidade() << std::endl;
    } else {
        std::cout << "Modularidade (Q): N/A (sem comunidades)" << std::endl;
    }
    
    // Mostrar tamanho de cada comunidade
    std::cout << "Tamanho das comunidades: ";
    for (int i = 0; i < detector.getNumComunidades(); i++) {
        std::cout << "C" << i << "=" << detector.getComunidades()[i].getTamanho() << " ";
    }
    std::cout << std::endl;
    
    // Mostrar vértices de cada comunidade com IDs originais
    std::cout << "Vértices por comunidade (limitado a 5 vértices):" << std::endl;
    for (int i = 0; i < detector.getNumComunidades(); i++) {
        const Comunidade& c = detector.getComunidades()[i];
        std::cout << "C" << i << ": ";
        
        int count = 0;
        for (int j = 0; j < c.getVertices().size() && count < 5; j++, count++) {
            int verticeCompacto = c.getVertices()[j];
            
            // Converter do índice compacto para o ID original
            int idOriginal = (verticeCompacto >= 0 && verticeCompacto < nosPresentes.size()) 
                ? nosPresentes[verticeCompacto] 
                : verticeCompacto;
            
            std::cout << idOriginal << " ";
        }
        
        if (c.getTamanho() > 5) {
            std::cout << "... (+" << (c.getTamanho() - 5) << " vértices)";
        }
        
        std::cout << std::endl;
    }
}