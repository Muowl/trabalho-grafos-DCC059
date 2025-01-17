#include "grafo_matriz.h"

bool GrafoMatriz::eh_bipartido() const
{
    int *cor = new int[ordem];
    for (int i = 0; i < ordem; i++)
    {
        cor[i] = 0;
    }
    // busca em largura
    auto bel = [&](int inicio) -> bool
    {
        int *fila = new int[ordem];
        int frente = 0, atras = 0;
        fila[atras++] = inicio;
        cor[inicio] = 1;
        while (frente != atras)
        {
            int u = fila[frente++];
            for (int v = 0; v < ordem; v++)
            {
                if (matriz[u][v] != 0)
                {
                    if (cor[v] == 0)
                    {
                        cor[v] = (cor[u] == 1) ? 2 : 1;
                        fila[atras++] = v;
                    }
                    else if (cor[v] == cor[u])
                    {
                        delete[] fila;
                        delete[] cor;
                        return false;
                    }
                }
            }
        }
        delete[] fila;
        return true;
    };
    // componentes conexas
    for (int i = 0; i < ordem; i++)
    {
        if (cor[i] == 0)
        {
            if (!bel(i))
            {
                delete[] cor;
                return false;
            }
        }
    }
    delete[] cor;
    return true;
}

int GrafoMatriz::n_conexo() const
{
    bool *visitado = new bool[ordem]();
    int componentes = 0;
    // busca em profundidade
    auto dfs = [&](int v, auto &dfs_refs) -> void
    {
        visitado[v] = true;
        for (int u = 0; u < ordem; u++)
        {
            if (matriz[v][u] != 0 && !visitado[u])
            {
                dfs_refs(u, dfs_refs);
            }
        }
    };

    for (int i = 0; i < ordem; i++)
    {
        if (!visitado[i])
        {
            ++componentes;
            dfs(i, dfs);
        }
    }
    delete[] visitado;
    return componentes;
}
int GrafoMatriz::get_grau(int vertice) const
{
    if (vertice < 0 || vertice >= ordem)
        return -1; // Verificação de índice inválido
    int grau = 0;
    for (int i = 0; i < ordem; ++i)
    {
        if (matriz[vertice][i] != 0)
            grau++;
        if (!direcionado && matriz[i][vertice] != 0)
            grau++;
    }
    return grau;
}

bool GrafoMatriz::eh_direcionado() const
{
    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            if (matriz[i][j] != matriz[j][i])
            {
                return true;
            }
        }
    }
    return false;
}

bool GrafoMatriz::vertice_ponderado() const
{
    return vertices_ponderados;
}

bool GrafoMatriz::aresta_ponderada() const
{
    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            if (matriz[i][j] != 0 && matriz[i][j] != 1)
            {
                return true; // peso != 1 ponderadas
            }
        }
    }
    return false;
}

bool GrafoMatriz::eh_completo() const
{
    for (int i = 0; i < ordem; ++i)
    {
        for (int j = 0; j < ordem; ++j)
        {
            if (i != j && matriz[i][j] == 0)
                return false;
        }
    }
    return true;
}

bool GrafoMatriz::eh_arvore() const
{
    if (n_conexo() != 1)
        return false; // Deve ser conexo
    int num_arestas = 0;
    for (int i = 0; i < ordem; ++i)
    {
        for (int j = 0; j < ordem; ++j)
        {
            if (matriz[i][j] != 0)
                num_arestas++;
        }
    }
    if (!direcionado)
        num_arestas /= 2;            // Contar arestas apenas uma vez
    return num_arestas == ordem - 1; // Árvore tem n-1 arestas
}

bool GrafoMatriz::possui_articulacao() const
{
    // verifica componentes conexas com busca em profundidade
    auto contar_componentes = [&](bool *ignorado) -> int
    {
        bool *visitado = new bool[ordem]();
        int componentes = 0;
        auto dfs = [&](int v, auto &dfs_ref) -> void
        {
            visitado[v] = true;
            for (int u = 0; u < ordem; u++)
            {
                if (!ignorado[u] && matriz[v][u] != 0 && !visitado[u])
                {
                    dfs_ref(u, dfs_ref);
                }
            }
        };
        for (int i = 0; i < ordem; i++)
        {
            if (!visitado[i] && !ignorado[i])
            {
                componentes++;
                dfs(i, dfs);
            }
        }
        delete[] visitado;
        return componentes;
    };
    // numero componentes conexas inicialmente
    bool *ignorado = new bool[ordem]();
    int componentes_iniciais = contar_componentes(ignorado);

    // verifica vertices como articulações
    for (int i = 0; i < ordem; i++)
    {
        ignorado[i] = true;
        int componentes_novas = contar_componentes(ignorado); // se aumenta ao ignorar i, entao i é articulação
        ignorado[i] = false;
        if (componentes_novas > componentes_iniciais)
        {
            delete[] ignorado;
            return true;
        }
    }
    delete[] ignorado;
    return false;
}

bool GrafoMatriz::possui_ponte() const
{
    // contar componentes conexas com busca em profundidade
    auto contar_componentes = [&]() -> int
    {
        bool *visitado = new bool[ordem]();
        int componentes = 0;
        auto dfs = [&](int v, auto &dfs_ref) -> void
        {
            visitado[v] = true;
            for (int u = 0; u < ordem; u++)
            {
                if (matriz[v][u] != 0 && !visitado[u])
                {
                    dfs_ref(u, dfs_ref);
                }
            }
        };
        for (int i = 0; i < ordem; i++)
        {
            if (!visitado[i])
            {
                componentes++;
                dfs(i, dfs);
            }
        }
        delete[] visitado;
        return componentes;
    };
    // numero inicial de componentes conexas
    int componentes_iniciais = contar_componentes();
    // testando arestas como possiveis pontes
    for (int u = 0; u < ordem; u++)
    {
        for (int v = 0; v < ordem; v++)
        {
            if (matriz[u][v] != 0)
            { // aresta entre u e v
                int peso_original = matriz[u][v];
                matriz[u][v] = 0; // remover temp a aresta u,v
                if (!direcionado)
                {
                    matriz[v][u] = 0;
                }
                int componentes_novos = contar_componentes();
                matriz[u][v] = peso_original;
                if (!direcionado)
                {
                    matriz[v][u] = peso_original;
                }
                if (componentes_novos > componentes_iniciais)
                { // se aumentar tem ponte
                    return true;
                }
            }
        }
    }
    return false;
}

void GrafoMatriz::carrega_grafo(const std::string &arquivo)
{
    std::ifstream in(arquivo);
    if (!in)
    {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    int dir, vp, ap;
    in >> ordem >> dir >> vp >> ap;
    direcionado = dir;
    vertices_ponderados = vp;
    arestas_ponderadas = ap;

    alocarMatriz(ordem);

    if (vertices_ponderados)
    {
        pesos_vertices = new int[ordem];
        for (int i = 0; i < ordem; ++i)
        {
            in >> pesos_vertices[i];
        }
    }

    int origem, destino, peso;
    while (in >> origem >> destino >> peso)
    {
        matriz[origem - 1][destino - 1] = peso;
        if (!direcionado)
            matriz[destino - 1][origem - 1] = peso;
    }
    in.close();
}

void GrafoMatriz::novo_grafo(const std::string &descricao, const std::string &saida) {
    std::ifstream in(descricao);
    if (!in) {
        throw std::runtime_error("Erro ao abrir o arquivo de descrição!");
    }

    // Ler configurações do grafo
    int num_vertices, dirigido, vertices_ponderados, arestas_ponderadas;
    in >> num_vertices >> dirigido >> vertices_ponderados >> arestas_ponderadas;

    direcionado = dirigido;
    vertices_ponderados = vertices_ponderados;
    arestas_ponderadas = arestas_ponderadas;

    alocarMatriz(num_vertices);

    srand(time(nullptr));
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            if (rand() % 2) {
                matriz[i][j] = rand() % 10 + 1;
                if (!direcionado) {
                    matriz[j][i] = matriz[i][j];
                }
            }
        }
    }
    in.close();

    // Abrir arquivo de saída com o nome correto
    std::ofstream out(saida, std::ios::out | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Erro ao criar o arquivo de saída!");
    }

    out << ordem << " " << direcionado << " " << vertices_ponderados << " " << arestas_ponderadas << "\n";
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            if (matriz[i][j] != 0) {
                out << i + 1 << " " << j + 1 << " " << matriz[i][j] << "\n";
            }
        }
    }
    out.close();
}
