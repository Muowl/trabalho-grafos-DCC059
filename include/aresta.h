#pragma once

class Aresta {
private:
    int origem;
    int destino;
    float peso;

public:
    Aresta(int orig, int dest, float p = 1.0)
        : origem(orig), destino(dest), peso(p) {}

    int getOrigem() const { return origem; }
    int getDestino() const { return destino; }
    float getPeso() const { return peso; }
    
    void setPeso(float p) { peso = p; }
};
