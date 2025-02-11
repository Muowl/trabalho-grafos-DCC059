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
    if (ordem >= capacidade) {
        redimensionar_matriz(capacidade * 2);
    }
    nos[ordem] = no(ordem, peso);
    ordem++;
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
        for (int i = id; i < ordem - 1; i++) {
            nos[i] = nos[i + 1];
            for (int j = 0; j < ordem; j++) {
                matriz_adj[i][j] = matriz_adj[i + 1][j];
                matriz_adj[j][i] = matriz_adj[j][i + 1];
            }
        }
        ordem--;
    }
}

void grafo_matriz::novo_no(float peso) {
    add_no(peso);
}

void grafo_matriz::nova_aresta(int origem, int destino, float peso) {
    add_aresta(origem, destino, peso);
}

void grafo_matriz::deleta_no(int id) {
    if (id >= 0 && id < ordem) {
        // Remover todas as arestas associadas ao nó
        for (int i = 0; i < ordem; i++) {
            matriz_adj[id][i] = 0;
            matriz_adj[i][id] = 0;
        }

        // Remover o nó e ajustar a matriz de adjacência
        for (int i = id; i < ordem - 1; i++) {
            nos[i] = nos[i + 1];
            for (int j = 0; j < ordem; j++) {
                matriz_adj[i][j] = matriz_adj[i + 1][j];
                matriz_adj[j][i] = matriz_adj[j][i + 1];
            }
        }

        // Atualizar a ordem do grafo
        ordem--;

        // Recalcular IDs dos nós restantes
        for (int i = 0; i < ordem; i++) {
            nos[i].id = i;
        }

        // Redimensionar a matriz de adjacência para refletir a nova ordem
        float** nova_matriz = new float*[ordem];
        for (int i = 0; i < ordem; ++i) {
            nova_matriz[i] = new float[ordem]();
            for (int j = 0; j < ordem; ++j) {
                nova_matriz[i][j] = matriz_adj[i][j];
            }
        }

        for (int i = 0; i < ordem + 1; ++i) {
            delete[] matriz_adj[i];
        }
        delete[] matriz_adj;

        matriz_adj = nova_matriz;
    }
}

void grafo_matriz::deleta_aresta(int origem, int destino) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        matriz_adj[origem][destino] = 0;
        if (!direcionado) {
            matriz_adj[destino][origem] = 0;
        }
    }
}

int grafo_matriz::n_conexo() const {
    int n = ordem;
    if (n == 0)
        return 0;

    int *parent = new int[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    auto find = [&parent](int x) -> int {
        while (parent[x] != x)
            x = parent[x];
        return x;
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz_adj[i][j] != 0) {
                int pu = find(i);
                int pv = find(j);
                if (pu != pv) {
                    parent[pv] = pu;
                }
            }
        }
    }

    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (find(i) == i)
            componentes++;
    }

    delete[] parent;
    return componentes == 1 ? 1 : 0;
}

int grafo_matriz::get_grau() const {
    int maxDegree = 0;
    for (int i = 0; i < ordem; i++) {
        int degree = 0;
        for (int j = 0; j < ordem; j++) {
            if (matriz_adj[i][j] != 0) {
                degree++;
            }
        }
        if (degree > maxDegree) {
            maxDegree = degree;
        }
    }
    return maxDegree;
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