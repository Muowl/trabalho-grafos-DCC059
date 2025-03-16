#include "leitura.h"
#include "grafo_lista.h"
#include "algoritmo_guloso.h"
#include "algoritmo_randomizado.h"
#include "algoritmo_relativo.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

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
    
    // Coletar estatísticas básicas
    resultado.numComunidades = algoritmo->getNumComunidades();
    resultado.modularidade = algoritmo->avaliarQualidade();
    
    // Calcular densidade média e tamanho médio
    float densidade = 0.0f;
    int totalVertices = 0;
    int maxTamanho = 0;
    
    const Vetor<Comunidade>& comunidades = algoritmo->getComunidades();
    for (int i = 0; i < comunidades.size(); i++) {
        const Comunidade& c = comunidades[i];
        densidade += c.calcularDensidade(algoritmo->getGrafo());
        totalVertices += c.getTamanho();
        if (c.getTamanho() > maxTamanho) {
            maxTamanho = c.getTamanho();
        }
    }
    
    resultado.densidadeMedia = comunidades.size() > 0 ? densidade / comunidades.size() : 0.0f;
    resultado.tamanhoMedioComunidade = comunidades.size() > 0 ? totalVertices / comunidades.size() : 0;
    resultado.maiorComunidade = maxTamanho;
    
    return resultado;
}

// Função para comparar os resultados dos algoritmos
void compararAlgoritmos(const ResultadoAlgoritmo resultados[], int numAlgoritmos) {
    cout << "\n==========================================================" << endl;
    cout << "COMPARAÇÃO DE ALGORITMOS DE DETECÇÃO DE COMUNIDADES" << endl;
    cout << "==========================================================" << endl;
    
    // Tabela de resultados
    cout << left << setw(15) << "Algoritmo" << " | ";
    cout << setw(10) << "Comunidades" << " | ";
    cout << setw(12) << "Modularidade" << " | ";
    cout << setw(16) << "Densidade Média" << " | ";
    cout << setw(8) << "Tempo(ms)" << " | ";
    cout << setw(13) << "Tamanho Médio" << " | ";
    cout << setw(10) << "Maior Com." << endl;
    
    cout << string(90, '-') << endl;
    
    for (int i = 0; i < numAlgoritmos; i++) {
        cout << left << setw(15) << resultados[i].nome << " | ";
        cout << setw(10) << resultados[i].numComunidades << " | ";
        cout << fixed << setprecision(4) << setw(12) << resultados[i].modularidade << " | ";
        cout << fixed << setprecision(4) << setw(16) << resultados[i].densidadeMedia << " | ";
        cout << setw(8) << resultados[i].tempoExecucao << " | ";
        cout << setw(13) << resultados[i].tamanhoMedioComunidade << " | ";
        cout << setw(10) << resultados[i].maiorComunidade << endl;
    }
    
    // Análise dos resultados
    cout << "\nANÁLISE COMPARATIVA:" << endl;
    
    // Encontrar o algoritmo mais rápido
    int idxMaisRapido = 0;
    for (int i = 1; i < numAlgoritmos; i++) {
        if (resultados[i].tempoExecucao < resultados[idxMaisRapido].tempoExecucao) {
            idxMaisRapido = i;
        }
    }
    
    // Encontrar o algoritmo com maior modularidade
    int idxMelhorModularidade = 0;
    for (int i = 1; i < numAlgoritmos; i++) {
        if (resultados[i].modularidade > resultados[idxMelhorModularidade].modularidade) {
            idxMelhorModularidade = i;
        }
    }
    
    // Encontrar o algoritmo com maior densidade
    int idxMelhorDensidade = 0;
    for (int i = 1; i < numAlgoritmos; i++) {
        if (resultados[i].densidadeMedia > resultados[idxMelhorDensidade].densidadeMedia) {
            idxMelhorDensidade = i;
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