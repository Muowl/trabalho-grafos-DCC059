#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "../include/leitura.h"

leitura::leitura(const std::string &filename) : num_nos(0)
{
    ler_arquivo_grafo(filename);
    identificar_nos_presentes();
    gerar_pesos_sinteticos();
}

leitura::~leitura()
{
    // Nothing to clean up
}

bool leitura::ler_arquivo_grafo(const std::string &filename)
{
    std::ifstream arquivo(filename);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string linha;
    int linhas_ignoradas = 0;

    // Ignorar as 4 primeiras linhas
    while (linhas_ignoradas < 4 && std::getline(arquivo, linha)) {
        linhas_ignoradas++;
    }

    // Determinar o número máximo de nós enquanto lê as arestas
    int max_no_id = -1;
    int from, to;
    
    // Lê as arestas e encontra o maior ID de nó
    while (arquivo >> from >> to) {
        if (from > max_no_id) max_no_id = from;
        if (to > max_no_id) max_no_id = to;
        
        arestas.push_back(std::pair<int, int>(from, to));
    }
    
    // O número de nós será o maior ID + 1 (assumindo que os IDs começam em 0)
    num_nos = max_no_id + 1;

    arquivo.close();
    return true;
}

void leitura::identificar_nos_presentes() {
    // Conjunto temporário para armazenar IDs únicos
    Vetor<bool> presente;
    presente.resize(num_nos, false);
    
    // Marcar todos os nós que aparecem nas arestas
    for (int i = 0; i < arestas.size(); i++) {
        presente[arestas[i].first] = true;
        presente[arestas[i].second] = true;
    }
    
    // Contar e popular o vetor com os IDs presentes
    nos_presentes.clear();
    for (int i = 0; i < num_nos; i++) {
        if (presente[i]) {
            nos_presentes.push_back(i);
        }
    }
    
    // Ordenar os IDs (já estão ordenados neste caso, mas para generalizar)
    // Não podemos usar std::sort diretamente com o nosso Vetor, então vamos usar uma abordagem manual
    for (int i = 0; i < nos_presentes.size(); i++) {
        for (int j = i + 1; j < nos_presentes.size(); j++) {
            if (nos_presentes[i] > nos_presentes[j]) {
                int temp = nos_presentes[i];
                nos_presentes[i] = nos_presentes[j];
                nos_presentes[j] = temp;
            }
        }
    }
}

void leitura::exibir_nos_presentes() const {
    std::cout << "Nós presentes no grafo (" << nos_presentes.size() << " nós):" << std::endl;
    
    int contador = 0;
    for (int i = 0; i < nos_presentes.size(); i++) {
        std::cout << nos_presentes[i] << " ";
        contador++;
        
        // Quebra de linha a cada 10 nós para melhorar a visualização
        if (contador % 10 == 0) {
            std::cout << std::endl;
        }
    }
    
    if (contador % 10 != 0) {
        std::cout << std::endl;
    }
    
    // Exibir gaps (IDs ausentes) para diagnóstico
    if (nos_presentes.size() > 1) {
        std::cout << "Gaps na sequência de IDs:" << std::endl;
        int gaps = 0;
        
        for (int i = 1; i < nos_presentes.size(); i++) {
            int gap = nos_presentes[i] - nos_presentes[i-1] - 1;
            if (gap > 0) {
                std::cout << "Entre " << nos_presentes[i-1] << " e " << nos_presentes[i] 
                          << ": " << gap << " ID(s) ausente(s)" << std::endl;
                gaps++;
                
                // Limitar a exibição para não sobrecarregar
                if (gaps >= 10) {
                    std::cout << "... (mais gaps existentes)" << std::endl;
                    break;
                }
            }
        }
    }
}

bool leitura::no_esta_presente(int id) const {
    // Busca binária para verificar rapidamente se um nó está presente
    int esquerda = 0;
    int direita = nos_presentes.size() - 1;
    
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        
        if (nos_presentes[meio] == id) {
            return true;
        }
        
        if (nos_presentes[meio] < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    return false;
}

void leitura::gerar_pesos_sinteticos() {
    // Gerar pesos como o valor absoluto da diferença entre os IDs dos nós
    for (int i = 0; i < arestas.size(); i++) {
        int origem = arestas[i].first;
        int destino = arestas[i].second;
        float peso = std::abs(destino - origem);
        
        // Para evitar peso zero em self-loops
        if (peso == 0) peso = 1.0f;
        
        arestas_com_peso.push_back(EdgeData(origem, destino, peso));
    }
    
    std::cout << "Pesos sintéticos gerados para " << arestas_com_peso.size() << " arestas." << std::endl;
}