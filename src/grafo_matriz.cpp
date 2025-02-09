#include "../include/grafo_matriz.h"
#include "../include/leitura.h"

no* grafo_matriz::get_no(int id) {
    if (id >= 0 && id < ordem) {
        return &nos[id];
    }
    return nullptr;
}

aresta* grafo_matriz::get_aresta(int origem, int destino) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        if (matriz_adj[origem][destino] != 0) {
            return new aresta(origem, destino, matriz_adj[origem][destino]);
        }
    }
    return nullptr;
}

void grafo_matriz::add_no(float peso) {
    if (ordem < MAX_VERTICES) {
        nos[ordem] = no(ordem, peso);
        ordem++;
    }
}

void grafo_matriz::add_aresta(int origem, int destino, float peso) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        matriz_adj[origem][destino] = peso;
        if (!direcionado) {
            matriz_adj[destino][origem] = peso;
        }
    }
}

aresta** grafo_matriz::get_vizinhos(int id) {
    if (id >= 0 && id < ordem) {
        aresta** vizinhos = new aresta*[ordem];
        int count = 0;
        for (int i = 0; i < ordem; i++) {
            if (matriz_adj[id][i] != 0) {
                vizinhos[count++] = new aresta(id, i, matriz_adj[id][i]);
            }
        }
        return vizinhos;
    }
    return nullptr;
}

void grafo_matriz::remove_no(int id) {
    if (id >= 0 && id < ordem) {
        for (int i = 0; i < ordem; i++) {
            matriz_adj[id][i] = 0;
            matriz_adj[i][id] = 0;
        }
    }
}

int grafo_matriz::n_conexo() const {
    bool visitado[MAX_VERTICES] = {false};
    for (int i = 0; i < ordem; i++) { 
        for (int j = 0; j < ordem; j++) {
            if (i != j) {
                bool encontrado = false;
                for (int k = 0; k < ordem; k++) { // verifica se existe um caminho entre i e j
                    if (matriz_adj[i][k] != 0 && matriz_adj[k][j] != 0) {
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) { // se não existe caminho, o grafo não é conexo
                    return 0;
                }
            }
        }
    }
    return 1;
}

int grafo_matriz::get_grau() const {
    int grau = 0;
    for (int i = 0; i < ordem; i++) {
        int grauNo = 0;
        for (int j = 0; j < ordem; j++) { // verifica para cada nó o número de arestas incidentes
            if (matriz_adj[i][j] != 0) {
                grauNo++;
            }
        }
        if (grauNo > grau) { // atualiza o grau máximo
            grau = grauNo;
        }
    }
    return grau;
}


bool grafo_matriz::carrega_grafo(const std::string &filename) {
    // Cria objeto de leitura para o arquivo
    leitura l(filename);

    // Inicializa os atributos do grafo
    this->ordem = l.get_num_nos();
    this->direcionado = l.get_direcionado();
    this->verticesPonderados = l.get_ponderado_vertices();
    this->arestasPonderadas = l.get_ponderado_arestas();

    // Inicializa os nós
    for (int i = 0; i < ordem; i++) {
        float peso = (verticesPonderados) ? 0 : 0; // Ajustar para obter o peso correto se necessário
        nos[i] = no(i, peso);
    }

    // Inicializa a matriz de adjacência
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            matriz_adj[i][j] = 0;
        }
    }

    // Preenche a matriz de adjacência com as arestas
    int totalArestas = l.get_total_lin();
    float **matriz = l.get_matriz_info();

    for (int i = 0; i < totalArestas; i++) {
        int origem = static_cast<int>(matriz[i][0]);
        int destino = static_cast<int>(matriz[i][1]);
        float peso = matriz[i][2];
        add_aresta(origem, destino, peso);
    }

    // Retorna true se o grafo foi carregado com sucesso
    return true;
}