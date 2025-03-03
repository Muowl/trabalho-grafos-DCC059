#pragma once

class no {
public:
    int id;
    float peso;

    no() : id(-1), peso(0.0f) {}  // Construtor padrão para grafo_matriz
    no(int id, float peso) : id(id), peso(peso) {}
};
