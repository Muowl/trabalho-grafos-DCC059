#include "../include/grafo.h"

Grafo::Grafo(int n, bool dir, bool pond, const std::string& nome)
    : num_vertices(n), direcionado(dir), ponderado(pond), nome(nome) {
}
