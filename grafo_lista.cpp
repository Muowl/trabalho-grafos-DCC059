#include "grafo_lista.h"

bool GrafoLista::eh_bipartido() const {
    // Fazer
    return false;
}

int GrafoLista::n_conexo() const {
    // Fazer
    return 0;
}

int GrafoLista::get_grau(int vertice) const {
    if (vertice < 1 || vertice > (int)vertices.size()) return -1;
    return vertices[vertice - 1].adjacentes.get_size();
}

int GrafoLista::get_ordem() const {
    return vertices.size();
}

bool GrafoLista::eh_direcionado() const {
    return direcionado;
}

bool GrafoLista::vertice_ponderado() const {
    return vertices_ponderados;
}

bool GrafoLista::aresta_ponderada() const {
    return arestas_ponderadas;
}

bool GrafoLista::eh_completo() const {
    // Fazer
    return false;
}

bool GrafoLista::eh_arvore() const {
    // Fazer
    return false;
}

bool GrafoLista::possui_articulacao() const {
    // Fazer
    return false;
}

bool GrafoLista::possui_ponte() const {
    // Fazer
    return false;
}

void GrafoLista::carrega_grafo(const std::string& arquivo) {
    std::ifstream in(arquivo);
    if (!in) {
        throw std::runtime_error("Erro ao abrir o arquivo");
    }

    int num_vertices, dir, vp, ap;
    in >> num_vertices >> dir >> vp >> ap;

    direcionado = dir;
    vertices_ponderados = vp;
    arestas_ponderadas = ap;

    vertices.clear();
    for (int i = 1; i <= num_vertices; ++i) {
        vertices.emplace_back(i);
    }

    int origem, destino, peso;
    while (in >> origem >> destino >> peso) {
        vertices[origem - 1].adjacentes.push_back(Aresta(destino, peso));
        if (!direcionado) {
            vertices[destino - 1].adjacentes.push_back(Aresta(origem, peso));
        }
    }
}

void GrafoLista::novo_grafo(const std::string& descricao, const std::string& saida) {
    // Fazer
}