#pragma once
#include <iostream>
#include <fstream>

// Classe responsável por ler o arquivo de entrada e criar o grafo
// Lembrar que a forma que vamos salvar a relação entre os grafos vai ser de responsabilidade da estrutura (lista ou matriz)
class leitura
{
private:
    int num_nos;
    bool direcionado;
    bool ponderado_vertices;
    bool ponderado_arestas; // fim da primeira linha do arquivo txt

    int *peso_nos(int num_nos); // segunda linha do arquivo txt, apenas se ponderado_vertices for true

public:
    leitura(const std::string &filename)
    {
        std::ifstream grafotxt(filename);
        if (!grafotxt.is_open())
        {
            std::cout << "Erro ao abrir o arquivo\n";
            exit(1);
        }
        while (!grafotxt.eof())
        {
            std::string linha;
            std::getline(grafotxt, linha);
            std::cout << linha << std::endl;
        }
    }
    ~leitura(){};

    void leitura_arquivo();
};