#ifndef AUXILIAR_GRAFO_H
#define AUXILIAR_GRAFO_H

class no_grafo {
public:
    int id;
    float peso;

    no_grafo(int id, float peso) : id(id), peso(peso) {}
};

class aresta_grafo {
public:
    int origem;
    int destino;
    float peso;

    aresta_grafo(int origem, int destino, float peso) : origem(origem), destino(destino), peso(peso) {}
};

#endif // AUXILIAR_GRAFO_H