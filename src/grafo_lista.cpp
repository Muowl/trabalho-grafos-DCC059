#include "../include/grafo_lista.h"
#include "../include/leitura.h"
#include <iostream>

bool grafo_lista::carrega_grafo(const std::string &filename) {
    // Cria objeto de leitura para o arquivo
    leitura l(filename);
    ordem = l.get_num_nos();
    direcionado = l.get_direcionado();
    verticesPonderados = l.get_ponderado_vertices();
    arestasPonderadas = l.get_ponderado_arestas();
    
    int numNos = l.get_num_nos();
    // Atribuição já foi feita acima: ordem = numNos;
    
    bool ponderadoVertices = l.get_ponderado_vertices();
    
    // Cria nós com id de 0 a numNos-1 (formato 0-based)
    for (int i = 0; i < numNos; i++) {
        no novoNo(i, (ponderadoVertices ? 0 : 0));
        vertices.push_back(novoNo);
    }

    int totalArestas = l.get_total_lin();
    float **matriz = l.get_matriz_info();
    this->total_lin = totalArestas;
    this->matriz_info = matriz;

    for (int i = 0; i < totalArestas; i++) {
        // Os IDs no arquivo já são 0-based, não precisa subtrair 1
        int origem = static_cast<int>(matriz[i][0]);
        int destino = static_cast<int>(matriz[i][1]);
        float peso = matriz[i][2];
        
        // Verificar se os IDs estão dentro do intervalo válido
        if (origem >= 0 && origem < numNos && destino >= 0 && destino < numNos) {
            aresta novaAresta(origem, destino, peso);
            arestas.push_back(novaAresta);
        }
    }
    
    return true;
}

int grafo_lista::n_conexo() const {
    int n = vertices.get_size();
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

    for (auto it = arestas.begin(); it != arestas.end(); ++it) {
        // Os IDs já são 0-based, não precisa ajustar
        int u = it->origem;
        int v = it->destino;

        // Verifica se u e v são índices válidos
        if (u < 0 || u >= n || v < 0 || v >= n) {
            // Pular esta aresta inválida
            continue;
        }

        int pu = find(u);
        int pv = find(v);
        if (pu != pv) {
            parent[pv] = pu;
        }
    }

    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (find(i) == i)
            componentes++;
    }
    
    delete[] parent;
    return componentes;
}

int grafo_lista::get_grau() const {
    int maxDegree = 0;
    // Iterar sobre cada vértice
    for (auto vit = vertices.begin(); vit != vertices.end(); ++vit) {
        int degree = 0;
        // Iterar sobre cada aresta
        for (auto ait = arestas.begin(); ait != arestas.end(); ++ait) {
            if (!direcionado) {
                // Para grafo não direcionado, contar se o vértice está em qualquer das extremidades
                if (ait->origem == vit->id || ait->destino == vit->id)
                    degree++;
            } else {
                // Para digrafo, somar in e out degree
                if (ait->origem == vit->id)
                    degree++;
                if (ait->destino == vit->id)
                    degree++;
            }
        }
        if (degree > maxDegree)
            maxDegree = degree;
    }
    return maxDegree;
}

// Parte 2

void grafo_lista::novo_no(float peso) {
    no novoNo(ordem, peso);
    vertices.push_back(novoNo);
    ordem++;
}

void grafo_lista::nova_aresta(int origem, int destino, float peso) {
    if (origem >= 0 && origem < ordem && destino >= 0 && destino < ordem) {
        aresta a(origem, destino, peso);
        arestas.push_back(a);
        // Se o grafo não for direcionado, adiciona a aresta reversa
        if (!direcionado) {
            aresta reversa(destino, origem, peso);
            arestas.push_back(reversa);
        }
    }
}

void grafo_lista::deleta_no(int id) {
    // Remove o vértice e suas arestas
    vertices.remove_if([&](const no &n) { return n.id == id; });
    arestas.remove_if([&](const aresta &a) { return a.origem == id || a.destino == id; });

    // Reindexa os vértices restantes
    int novo_id = 0;
    for (auto& no : vertices) {
        no.id = novo_id++;
    }

    // Atualiza as arestas com os novos IDs
    for (auto& aresta : arestas) {
        if (aresta.origem > id) aresta.origem--;
        if (aresta.destino > id) aresta.destino--;
    }

    ordem--;
}

void grafo_lista::deleta_aresta(int origem, int destino) {
    bool removed = false;
    arestas.remove_if([&](const aresta &a) {
        if (!removed && a.origem == origem && a.destino == destino) {
            removed = true;
            return true;
        }
        return false;
    });
    // Se o grafo não for direcionado, removemos a aresta reversa
    if (!direcionado) {
        removed = false;
        arestas.remove_if([&](const aresta &a) {
            if (!removed && a.origem == destino && a.destino == origem) {
                removed = true;
                return true;
            }
            return false;
        });
    }
}

grafo_lista::MenorMaior grafo_lista::menor_maior_distancia() const {
    const float INF = 1e9f; // Valor para representar distâncias infinitas
    int n = ordem; // Número de vértices

    // Aloca e inicializa matriz de distâncias
    float** dist = new float*[n];
    for (int i = 0; i < n; i++) {
        dist[i] = new float[n];
        for (int j = 0; j < n; j++) {
            dist[i][j] = (i == j) ? 0.0f : INF; // Distância de um nó para ele mesmo é 0
        }
    }

    // Inicializa as distâncias diretas com base nas arestas
    for (auto it = arestas.begin(); it != arestas.end(); ++it) {
        int u = it->origem; // IDs já são 0-based
        int v = it->destino;
        float peso = it->peso;
        if (peso < dist[u][v]) { // Se houver múltiplas arestas, escolhe a de menor peso
            dist[u][v] = peso;
        }
    }

    // Algoritmo de Floyd-Warshall para calcular as distâncias mínimas
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                float novo = dist[i][k] + dist[k][j];
                if (novo < dist[i][j]) {
                    dist[i][j] = novo; // Atualiza a distância mínima
                }
            }
        }
    }

    // Encontra o par de nós com a maior distância mínima
    int no1 = -1, no2 = -1;
    float maxDist = -1.0f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] < INF && dist[i][j] > maxDist) {
                maxDist = dist[i][j];
                no1 = i;
                no2 = j;
            }
        }
    }

    // Libera a memória da matriz de distâncias
    for (int i = 0; i < n; i++) {
        delete[] dist[i];
    }
    delete[] dist;

    // Retorna o resultado mantendo IDs 0-based
    MenorMaior ret;
    ret.no1 = no1;  // Sem converter para 1-based
    ret.no2 = no2;
    ret.distancia = maxDist;
    return ret;
}