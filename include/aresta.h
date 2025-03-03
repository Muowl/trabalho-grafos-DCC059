#pragma once

class aresta {
public:
    int origem;
    int destino;
    float peso;

    aresta(int origem, int destino, float peso) : origem(origem), destino(destino), peso(peso) {}
};
