#include <iostream>
#include <fstream>
#include "../include/leitura.h"

leitura::leitura(const std::string &filename)
{
    std::ifstream grafotxt(filename);
    if (!grafotxt.is_open())
    {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        exit(1);
    }

    // ler primeira linha: número de nós, se é direcionado, se ponderado nos e arestas
    grafotxt >> num_nos >> direcionado >> ponderado_vertices >> ponderado_arestas;

    // ler a linha de pesos dos nós (se existir)
    if (ponderado_vertices)
    {
        pesos_nos = new int[num_nos];
        for (int i = 0; i < num_nos; i++)
        {
            grafotxt >> pesos_nos[i];
        }
    }
    else
    {
        pesos_nos = nullptr;
    }

    // contar quantas linhas restam (cada uma contendo "origem destino peso")
    std::streampos currentPos = grafotxt.tellg();
    int linesCount = 0;
    {
        std::string temp;
        while (std::getline(grafotxt, temp))
        {
            if (!temp.empty()) linesCount++;
        }
    }
    total_lin = linesCount;

    // retornar ao ponto de leitura
    grafotxt.clear();
    grafotxt.seekg(currentPos);

    // alocar matriz_info para cada linha [ 3 colunas: origem, destino, peso ]
    matriz_info = new float*[total_lin];
    for (int i = 0; i < total_lin; i++)
    {
        matriz_info[i] = new float[3];
    }

    // ler as linhas de aresta e armazenar
    int row = 0;
    while (row < total_lin)
    {
        int origem, destino;
        float peso;
        grafotxt >> origem >> destino >> peso;

        matriz_info[row][0] = static_cast<float>(origem);
        matriz_info[row][1] = static_cast<float>(destino);
        matriz_info[row][2] = peso;
        row++;
    }
    grafotxt.close();
}

// destrutor
leitura::~leitura()
{
    for (int i = 0; i < total_lin; i++)
    {
        delete[] matriz_info[i];
    }
    delete[] matriz_info;
    delete[] pesos_nos;
}

// imprimir as linhas de aresta (igual ao .txt)
void leitura::imprimir_matriz_info()
{
    for (int i = 0; i < total_lin; i++)
    {
        std::cout 
            << static_cast<int>(matriz_info[i][0]) << " "
            << static_cast<int>(matriz_info[i][1]) << " "
            << matriz_info[i][2]
            << std::endl;
    }
}