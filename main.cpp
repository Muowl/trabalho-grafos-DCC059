#include "leitura.h"
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "algoritmo_guloso.h"
#include "algoritmo_randomizado.h"
#include "algoritmo_relativo.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

// Structure to hold algorithm results
struct ResultadoAlgoritmo {
    string nome;
    int numComunidades;
    float modularidade;
    float densidadeMedia;
    int tamanhoMedioComunidade;
    int maiorComunidade;
};

void imprimirResultadosComIdsOriginais(const DetectorComunidades& detector, const Vetor<int>& nosPresentes);
ResultadoAlgoritmo executarAlgoritmoGuloso(Grafo* grafo, const leitura* leitor, bool exibirDetalhes = true);
ResultadoAlgoritmo executarAlgoritmoRandomizado(Grafo* grafo, const leitura* leitor, bool exibirDetalhes = true);
ResultadoAlgoritmo executarAlgoritmoRelativo(Grafo* grafo, const leitura* leitor, bool exibirDetalhes = true);
void compararAlgoritmos(const ResultadoAlgoritmo resultados[], int numAlgoritmos);
void exibirAjuda();

int main(int argc, char** argv) {
    // Valor padrão para o arquivo
    string arquivo_base = "grafo-teste.txt";
    string diretorio_entradas = "../entradas/";
    string caminho_arquivo;
    bool usarMatriz = false;
    bool resolverProblema = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            resolverProblema = true;
        } else if (strcmp(argv[i], "-m") == 0) {
            usarMatriz = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            usarMatriz = false;
        } else if (argv[i][0] != '-') {
            // Se for um nome de arquivo, apenas armazena o nome base
            arquivo_base = argv[i];
        } else {
            exibirAjuda();
            return 1;
        }
    }
    
    // Montar o caminho completo do arquivo
    caminho_arquivo = diretorio_entradas + arquivo_base;
    
    if (!resolverProblema) {
        cout << "Erro: Parâmetro -p obrigatório." << endl;
        exibirAjuda();
        return 1;
    }
    
    cout << "ANÁLISE DE COMUNIDADES EM GRAFOS" << endl;
    cout << "=================================" << endl;
    cout << "Arquivo de entrada: " << arquivo_base << endl;
    cout << "Caminho completo: " << caminho_arquivo << endl;
    cout << "Usando representação: " << (usarMatriz ? "Matriz" : "Lista") << endl;
    
    // Criar grafo usando listas ou matriz de adjacência
    leitura leitor(caminho_arquivo);
    
    int numNos = leitor.get_num_nos();
    cout << "Número de nós total: " << numNos << endl;
    cout << "Número de arestas: " << leitor.get_arestas_com_peso().size() << endl;
    
    // Criar o grafo conforme especificado
    Grafo* grafo = nullptr;
    
    if (usarMatriz) {
        grafo = new GrafoMatriz(numNos, false, true, "Grafo de Comunidades");
    } else {
        grafo = new GrafoLista(numNos, false, true, "Grafo de Comunidades");
    }
    
    // Carregar arestas no grafo
    const Vetor<EdgeData>& arestas = leitor.get_arestas_com_peso();
    for (int i = 0; i < arestas.size(); i++) {
        grafo->adicionarAresta(arestas[i].origem, arestas[i].destino, arestas[i].peso);
    }
    
    cout << "\nPreparando para executar algoritmos de detecção de comunidades..." << endl;
    
    // Executar os três algoritmos e coletar resultados
    ResultadoAlgoritmo resultados[3];
    
    cout << "\nExecutando Algoritmo Guloso..." << endl;
    resultados[0] = executarAlgoritmoGuloso(grafo, &leitor);
    
    cout << "\nExecutando Algoritmo Randomizado..." << endl;
    resultados[1] = executarAlgoritmoRandomizado(grafo, &leitor);
    
    cout << "\nExecutando Algoritmo Relativo..." << endl;
    resultados[2] = executarAlgoritmoRelativo(grafo, &leitor);
    
    // Exibir comparação dos algoritmos
    compararAlgoritmos(resultados, 3);
    
    // Liberar memória
    delete grafo;
    
    return 0;
}

void exibirAjuda() {
    cout << "Uso: ./main.out -p [-m|-l] arquivo.txt" << endl;
    cout << "  -p: Executa a detecção de comunidades (obrigatório)" << endl;
    cout << "  -m: Utiliza representação de matriz de adjacência" << endl;
    cout << "  -l: Utiliza representação de lista de adjacência (padrão)" << endl;
    cout << "  arquivo.txt: Arquivo com dados do grafo (procurado na pasta entradas/)" << endl;
}

// Função para executar algoritmo guloso e coletar métricas
ResultadoAlgoritmo executarAlgoritmoGuloso(Grafo* grafo, const leitura* leitor, bool exibirDetalhes) {
    ResultadoAlgoritmo resultado;
    resultado.nome = "Guloso";
    
    try {
        AlgoritmoGuloso algoritmo(grafo, 0.3f, 5);
        algoritmo.detectarComunidades();
        
        // Exibir resultados detalhados se solicitado
        if (exibirDetalhes) {
            cout << "Algoritmo Guloso: Identificadas " << algoritmo.getNumComunidades() << " comunidades." << endl;
            imprimirResultadosComIdsOriginais(algoritmo, leitor->get_nos_presentes());
        }
        
        // Coletar métricas
        resultado.numComunidades = algoritmo.getNumComunidades();
        resultado.modularidade = algoritmo.avaliarQualidade();
        
        // Calcular densidade média e tamanho
        float densidade = 0.0f;
        int totalVertices = 0;
        int maxTamanho = 0;
        
        const Vetor<Comunidade>& comunidades = algoritmo.getComunidades();
        for (int i = 0; i < comunidades.size(); i++) {
            const Comunidade& c = comunidades[i];
            densidade += c.calcularDensidade(algoritmo.getGrafo());
            totalVertices += c.getTamanho();
            if (c.getTamanho() > maxTamanho) {
                maxTamanho = c.getTamanho();
            }
        }
        
        resultado.densidadeMedia = comunidades.size() > 0 ? densidade / comunidades.size() : 0.0f;
        resultado.tamanhoMedioComunidade = comunidades.size() > 0 ? totalVertices / comunidades.size() : 0;
        resultado.maiorComunidade = maxTamanho;
        
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo guloso: " << e.what() << endl;
        resultado.numComunidades = 0;
        resultado.modularidade = 0.0f;
        resultado.densidadeMedia = 0.0f;
        resultado.tamanhoMedioComunidade = 0;
        resultado.maiorComunidade = 0;
    }
    
    return resultado;
}

// Função para executar algoritmo randomizado e coletar métricas
ResultadoAlgoritmo executarAlgoritmoRandomizado(Grafo* grafo, const leitura* leitor, bool exibirDetalhes) {
    ResultadoAlgoritmo resultado;
    resultado.nome = "Randomizado";
    
    try {
        AlgoritmoRandomizado algoritmo(grafo, leitor, 0.5f, 3, time(0));
        algoritmo.detectarComunidades();
        
        // Exibir resultados detalhados se solicitado
        if (exibirDetalhes) {
            cout << "Algoritmo Randomizado: Criadas " << algoritmo.getNumComunidades() << " comunidades." << endl;
            imprimirResultadosComIdsOriginais(algoritmo, leitor->get_nos_presentes());
        }
        
        // Coletar métricas
        resultado.numComunidades = algoritmo.getNumComunidades();
        resultado.modularidade = algoritmo.avaliarQualidade();
        
        // Calcular densidade média e tamanho
        float densidade = 0.0f;
        int totalVertices = 0;
        int maxTamanho = 0;
        
        const Vetor<Comunidade>& comunidades = algoritmo.getComunidades();
        for (int i = 0; i < comunidades.size(); i++) {
            const Comunidade& c = comunidades[i];
            densidade += c.calcularDensidade(algoritmo.getGrafo());
            totalVertices += c.getTamanho();
            if (c.getTamanho() > maxTamanho) {
                maxTamanho = c.getTamanho();
            }
        }
        
        resultado.densidadeMedia = comunidades.size() > 0 ? densidade / comunidades.size() : 0.0f;
        resultado.tamanhoMedioComunidade = comunidades.size() > 0 ? totalVertices / comunidades.size() : 0;
        resultado.maiorComunidade = maxTamanho;
        
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo randomizado: " << e.what() << endl;
        resultado.numComunidades = 0;
        resultado.modularidade = 0.0f;
        resultado.densidadeMedia = 0.0f;
        resultado.tamanhoMedioComunidade = 0;
        resultado.maiorComunidade = 0;
    }
    
    return resultado;
}

// Função para executar algoritmo relativo e coletar métricas
ResultadoAlgoritmo executarAlgoritmoRelativo(Grafo* grafo, const leitura* leitor, bool exibirDetalhes) {
    ResultadoAlgoritmo resultado;
    resultado.nome = "Relativo";
    
    try {
        AlgoritmoRelativo algoritmo(grafo, AlgoritmoRelativo::GULOSO, 0.01f, 5, leitor);
        algoritmo.detectarComunidades();
        
        // Exibir resultados detalhados se solicitado
        if (exibirDetalhes) {
            cout << "Algoritmo Relativo: Detectou " << algoritmo.getNumComunidades() << " comunidades." << endl;
            imprimirResultadosComIdsOriginais(algoritmo, leitor->get_nos_presentes());
        }
        
        // Coletar métricas
        resultado.numComunidades = algoritmo.getNumComunidades();
        resultado.modularidade = algoritmo.avaliarQualidade();
        
        // Calcular densidade média e tamanho
        float densidade = 0.0f;
        int totalVertices = 0;
        int maxTamanho = 0;
        
        const Vetor<Comunidade>& comunidades = algoritmo.getComunidades();
        for (int i = 0; i < comunidades.size(); i++) {
            const Comunidade& c = comunidades[i];
            densidade += c.calcularDensidade(algoritmo.getGrafo());
            totalVertices += c.getTamanho();
            if (c.getTamanho() > maxTamanho) {
                maxTamanho = c.getTamanho();
            }
        }
        
        resultado.densidadeMedia = comunidades.size() > 0 ? densidade / comunidades.size() : 0.0f;
        resultado.tamanhoMedioComunidade = comunidades.size() > 0 ? totalVertices / comunidades.size() : 0;
        resultado.maiorComunidade = maxTamanho;
        
    } catch (const std::exception& e) {
        cerr << "Erro no algoritmo relativo: " << e.what() << endl;
        resultado.numComunidades = 0;
        resultado.modularidade = 0.0f;
        resultado.densidadeMedia = 0.0f;
        resultado.tamanhoMedioComunidade = 0;
        resultado.maiorComunidade = 0;
    }
    
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
    cout << setw(13) << "Tamanho Médio" << " | ";
    cout << setw(10) << "Maior Com." << endl;
    
    cout << string(80, '-') << endl;
    
    for (int i = 0; i < numAlgoritmos; i++) {
        cout << left << setw(15) << resultados[i].nome << " | ";
        cout << setw(10) << resultados[i].numComunidades << " | ";
        cout << fixed << setprecision(4) << setw(12) << resultados[i].modularidade << " | ";
        cout << fixed << setprecision(4) << setw(16) << resultados[i].densidadeMedia << " | ";
        cout << setw(13) << resultados[i].tamanhoMedioComunidade << " | ";
        cout << setw(10) << resultados[i].maiorComunidade << endl;
    }
    
    // Análise dos resultados
    cout << "\nANÁLISE COMPARATIVA:" << endl;
    
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
    
    cout << "- Algoritmo com maior modularidade: " << resultados[idxMelhorModularidade].nome 
         << " (" << fixed << setprecision(4) << resultados[idxMelhorModularidade].modularidade << ")" << endl;
    cout << "- Algoritmo com maior densidade: " << resultados[idxMelhorDensidade].nome 
         << " (" << fixed << setprecision(4) << resultados[idxMelhorDensidade].densidadeMedia << ")" << endl;
    
    // Determinar algoritmo com melhor equilíbrio - usando apenas modularidade e densidade
    int idxMelhorEquilibrio = 0;
    float pontuacaoMax = 0;
    
    for (int i = 0; i < numAlgoritmos; i++) {
        // Normalizar cada métrica entre 0 e 1 (quanto maior, melhor)
        float modNorm = resultados[i].modularidade / resultados[idxMelhorModularidade].modularidade;
        float densNorm = resultados[i].densidadeMedia / resultados[idxMelhorDensidade].densidadeMedia;
        
        // Pontuação ponderada (modularidade tem peso maior)
        float pontuacao = (0.6 * modNorm) + (0.4 * densNorm);
        
        if (pontuacao > pontuacaoMax) {
            pontuacaoMax = pontuacao;
            idxMelhorEquilibrio = i;
        }
    }
    
    cout << "\nCONCLUSÃO:" << endl;
    cout << "Com base nos resultados acima, o algoritmo com melhor equilíbrio de qualidade" << endl;
    cout << "(modularidade e densidade) é: " << resultados[idxMelhorEquilibrio].nome << endl;
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