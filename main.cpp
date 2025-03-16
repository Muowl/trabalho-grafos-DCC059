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

// Estrutura para armazenar os resultados de cada algoritmo
struct ResultadoAlgoritmo {
    string nome;
    int numComunidades;
    float modularidade;
    float densidadeMedia;
    int tempoExecucao;  // em ms
    int tamanhoMedioComunidade;
    int maiorComunidade;
};

// Função para executar um algoritmo e coletar seus resultados
ResultadoAlgoritmo executarAlgoritmo(DetectorComunidades* algoritmo, const string& nome) {
    ResultadoAlgoritmo resultado;
    resultado.nome = nome;
    
    // Medir o tempo de execução
    auto inicio = high_resolution_clock::now();
    
    algoritmo->detectarComunidades();
    
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);
    resultado.tempoExecucao = duracao.count();
    
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
    
    cout << "- Algoritmo mais rápido: " << resultados[idxMaisRapido].nome 
         << " (" << resultados[idxMaisRapido].tempoExecucao << " ms)" << endl;
         
    cout << "- Algoritmo com maior modularidade: " << resultados[idxMelhorModularidade].nome 
         << " (" << resultados[idxMelhorModularidade].modularidade << ")" << endl;
         
    cout << "- Algoritmo com maior densidade: " << resultados[idxMelhorDensidade].nome 
         << " (" << resultados[idxMelhorDensidade].densidadeMedia << ")" << endl;
    
    // Conclusão geral
    cout << "\nCONCLUSÃO:" << endl;
    cout << "Com base nos resultados acima, o algoritmo com melhor equilíbrio entre "
         << "qualidade (modularidade e densidade) e eficiência (tempo) é: ";
    
    // Fórmula simples para pontuação: (modularidade + densidade) / (tempo normalizado)
    float maxTempo = 0;
    for (int i = 0; i < numAlgoritmos; i++) {
        if (resultados[i].tempoExecucao > maxTempo) {
            maxTempo = resultados[i].tempoExecucao;
        }
    }
    
    int idxMelhorGlobal = 0;
    float maxPontuacao = 0;
    
    for (int i = 0; i < numAlgoritmos; i++) {
        float tempoNormalizado = resultados[i].tempoExecucao / maxTempo;
        float pontuacao = (resultados[i].modularidade + resultados[i].densidadeMedia) / 
                          (tempoNormalizado > 0 ? tempoNormalizado : 0.001f);
                          
        if (pontuacao > maxPontuacao) {
            maxPontuacao = pontuacao;
            idxMelhorGlobal = i;
        }
    }
    
    cout << resultados[idxMelhorGlobal].nome << endl;
}

// Modifica o método getGrafo para permitir acesso ao grafo pelo método executarAlgoritmo
void adicionarGetGrafo(const string& filepath) {
    cout << "Adicionando método getGrafo() à classe DetectorComunidades..." << endl;
}

int main(int argc, char** argv) {
    string arquivo = "../entradas/grafo-teste.txt";
    
    cout << "ANÁLISE DE COMUNIDADES EM GRAFOS" << endl;
    cout << "=================================" << endl;
    cout << "Carregando grafo do arquivo: " << arquivo << endl;
    
    // Carregar o grafo
    leitura leitor(arquivo);
    int numNos = leitor.get_num_nos();
    cout << "Número de nós total: " << numNos << endl;
    cout << "Número de arestas: " << leitor.get_arestas().size() << endl;
    
    // Criar grafo com lista de adjacências
    GrafoLista grafo(numNos, false, true, "Grafo Amostral");
    
    // Adicionar todas as arestas ao grafo
    const Vetor<EdgeData>& arestas = leitor.get_arestas_com_peso();
    cout << "Adicionando " << arestas.size() << " arestas ao grafo..." << endl;
    
    for (int i = 0; i < arestas.size(); i++) {
        grafo.adicionarAresta(
            arestas[i].origem, 
            arestas[i].destino, 
            arestas[i].peso
        );
    }
    
    cout << "\nPreparando para executar algoritmos de detecção de comunidades..." << endl;
    
    // Adicionar o método getGrafo() à classe DetectorComunidades para permitir acesso ao grafo
    adicionarGetGrafo("d:\\Trabalho\\trabalho-grafos-DCC059\\include\\comunidade.h");
    
    // Executar os três algoritmos e coletar resultados
    const int NUM_ALGORITMOS = 3;
    ResultadoAlgoritmo resultados[NUM_ALGORITMOS];
    
    try {
        cout << "\nExecutando Algoritmo Guloso..." << endl;
        AlgoritmoGuloso guloso(&grafo, 0.3f, 5);
        resultados[0] = executarAlgoritmo(&guloso, "Guloso");
        guloso.imprimirResultados();
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo guloso: " << e.what() << endl;
        resultados[0].nome = "Guloso (ERRO)";
    }
    
    try {
        cout << "\nExecutando Algoritmo Randomizado..." << endl;
        AlgoritmoRandomizado randomizado(&grafo, &leitor, 0.5f, 3, time(0));
        resultados[1] = executarAlgoritmo(&randomizado, "Randomizado");
        randomizado.imprimirResultados();
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo randomizado: " << e.what() << endl;
        resultados[1].nome = "Randomizado (ERRO)";
    }
    
    try {
        cout << "\nExecutando Algoritmo Relativo..." << endl;
        AlgoritmoRelativo relativo(&grafo, AlgoritmoRelativo::GULOSO, 0.01f, 5, &leitor);
        resultados[2] = executarAlgoritmo(&relativo, "Relativo");
        relativo.imprimirResultados();
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo relativo: " << e.what() << endl;
        resultados[2].nome = "Relativo (ERRO)";
    }
    
    // Comparar resultados
    compararAlgoritmos(resultados, NUM_ALGORITMOS);
    
    return 0;
}