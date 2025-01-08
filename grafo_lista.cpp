#include "grafo_lista.h"

static bool dfs_color(const GrafoLista &g, int v, int corAtual, int *corArray)
{
    corArray[v] = corAtual;
    // Percorre a lista de adjacências
    for (auto it = g.vertices[v].adjacentes.begin(); it != g.vertices[v].adjacentes.end(); ++it)
    {
        int w = it->destino - 1;
        // Se w não foi colorido, colore com cor oposta
        if (corArray[w] == 0)
        {
            if (!dfs_color(g, w, -corAtual, corArray))
                return false;
        }
        // Se w tem a mesma cor, não é bipartido
        else if (corArray[w] == corArray[v])
        {
            return false;
        }
    }
    return true;
}

bool GrafoLista::eh_bipartido() const
{
    // Implementação de força bruta via DFS em cada componente
    if (vertices.empty())
        return true;

    // Aloca array de cores (1 ou -1). 0 significa não colorido.
    int *corArray = new int[vertices.size()];
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        corArray[i] = 0;
    }

    // Tenta colorir cada componente
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        if (corArray[i] == 0)
        {
            if (!dfs_color(*this, i, 1, corArray))
            {
                delete[] corArray;
                return false;
            }
        }
    }

    delete[] corArray;
    return true;
}

static void dfs_componente(const GrafoLista &g, int v, bool *visitado)
{
    visitado[v] = true;
    // Percorre a lista de adjacências
    for (auto it = g.vertices[v].adjacentes.begin(); it != g.vertices[v].adjacentes.end(); ++it)
    {
        int w = it->destino - 1;
        if (!visitado[w])
        {
            dfs_componente(g, w, visitado);
        }
    }
}

int GrafoLista::n_conexo() const
{
    // Conta quantas componentes conexas há no grafo usando DFS manual
    if (vertices.empty())
        return 0;

    bool *visitado = new bool[vertices.size()];
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        visitado[i] = false;
    }

    int componentes = 0;
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        if (!visitado[i])
        {
            dfs_componente(*this, i, visitado);
            componentes++;
        }
    }

    delete[] visitado;
    return componentes;
}

int GrafoLista::get_grau(int vertice) const
{
    if (vertice < 1 || vertice > (int)vertices.size())
        return -1;
    return vertices[vertice - 1].adjacentes.get_size();
}

int GrafoLista::get_ordem() const
{
    return vertices.size();
}

bool GrafoLista::eh_direcionado() const
{
    return direcionado;
}

bool GrafoLista::vertice_ponderado() const
{
    return vertices_ponderados;
}

bool GrafoLista::aresta_ponderada() const
{
    return arestas_ponderadas;
}

bool GrafoLista::eh_completo() const
{
    int n = vertices.size();
    for (int i = 0; i < n; ++i)
    {
        if (vertices[i].adjacentes.get_size() != n - 1)
        {
            return false;
        }
    }
    return true;
}

bool GrafoLista::eh_arvore() const
{
    if (vertices.empty())
        return false;
    if (n_conexo() != 1)
        return false;
    int num_arestas = 0;
    for (const auto &vertice : vertices)
    {
        num_arestas += vertice.adjacentes.get_size();
    }
    if (!direcionado)
        num_arestas /= 2;
    return num_arestas == vertices.size() - 1;
}

static void dfs_articulacao(const GrafoLista &g, int v, bool *visitado, int *disc, int *low, int *parent, bool *ap, int &time)
{
    int children = 0;
    visitado[v] = true;
    disc[v] = low[v] = ++time;

    for (auto it = g.vertices[v].adjacentes.begin(); it != g.vertices[v].adjacentes.end(); ++it)
    {
        int w = it->destino - 1;
        if (!visitado[w])
        {
            children++;
            parent[w] = v;
            dfs_articulacao(g, w, visitado, disc, low, parent, ap, time);
            low[v] = std::min(low[v], low[w]);
            if (parent[v] == -1 && children > 1)
                ap[v] = true;
            if (parent[v] != -1 && low[w] >= disc[v])
                ap[v] = true;
        }
        else if (w != parent[v])
        {
            low[v] = std::min(low[v], disc[w]);
        }
    }
}

bool GrafoLista::possui_articulacao() const
{
    int n = vertices.size();
    bool *visitado = new bool[n];
    int *disc = new int[n];
    int *low = new int[n];
    int *parent = new int[n];
    bool *ap = new bool[n];

    for (int i = 0; i < n; ++i)
    {
        visitado[i] = false;
        parent[i] = -1;
        ap[i] = false;
    }

    int time = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            dfs_articulacao(*this, i, visitado, disc, low, parent, ap, time);
        }
    }

    bool has_ap = false;
    for (int i = 0; i < n; ++i)
    {
        if (ap[i])
        {
            has_ap = true;
            break;
        }
    }

    delete[] visitado;
    delete[] disc;
    delete[] low;
    delete[] parent;
    delete[] ap;

    return has_ap;
}

static void dfs_ponte(const GrafoLista &g, int v, bool *visitado, int *disc, int *low, int *parent, bool &has_bridge, int &time)
{
    visitado[v] = true;
    disc[v] = low[v] = ++time;

    for (auto it = g.vertices[v].adjacentes.begin(); it != g.vertices[v].adjacentes.end(); ++it)
    {
        int w = it->destino - 1;
        if (!visitado[w])
        {
            parent[w] = v;
            dfs_ponte(g, w, visitado, disc, low, parent, has_bridge, time);
            low[v] = std::min(low[v], low[w]);
            if (low[w] > disc[v])
            {
                has_bridge = true;
                return;
            }
        }
        else if (w != parent[v])
        {
            low[v] = std::min(low[v], disc[w]);
        }
    }
}

bool GrafoLista::possui_ponte() const
{
    int n = vertices.size();
    bool *visitado = new bool[n];
    int *disc = new int[n];
    int *low = new int[n];
    int *parent = new int[n];

    for (int i = 0; i < n; ++i)
    {
        visitado[i] = false;
        parent[i] = -1;
    }

    bool has_bridge = false;
    int time = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!visitado[i])
        {
            dfs_ponte(*this, i, visitado, disc, low, parent, has_bridge, time);
            if (has_bridge)
                break;
        }
    }

    delete[] visitado;
    delete[] disc;
    delete[] low;
    delete[] parent;

    return has_bridge;
}

void GrafoLista::carrega_grafo(const std::string &arquivo)
{
    std::ifstream in(arquivo);
    if (!in)
    {
        throw std::runtime_error("Erro ao abrir o arquivo");
    }

    int num_vertices, dir, vp, ap;
    in >> num_vertices >> dir >> vp >> ap;

    direcionado = dir;
    vertices_ponderados = vp;
    arestas_ponderadas = ap;

    vertices.clear();
    for (int i = 1; i <= num_vertices; ++i)
    {
        vertices.emplace_back(i);
    }

    int origem, destino, peso;
    while (in >> origem >> destino >> peso)
    {
        vertices[origem - 1].adjacentes.push_back(Aresta(destino, peso));
        if (!direcionado)
        {
            vertices[destino - 1].adjacentes.push_back(Aresta(origem, peso));
        }
    }
}

void GrafoLista::novo_grafo(const std::string &descricao, const std::string &saida)
{
    std::ifstream in(descricao);
    if (!in)
    {
        throw std::runtime_error("Erro ao abrir o arquivo de descrição");
    }

    int num_vertices, dirigido, vertices_ponderados, arestas_ponderadas, completo, bipartido, arvore, aresta_ponte, vertice_articulacao;
    in >> num_vertices >> dirigido >> vertices_ponderados >> arestas_ponderadas >> completo >> bipartido >> arvore >> aresta_ponte >> vertice_articulacao;

    direcionado = dirigido;
    this->vertices_ponderados = vertices_ponderados;
    this->arestas_ponderadas = arestas_ponderadas;

    vertices.clear();
    for (int i = 1; i <= num_vertices; ++i)
    {
        vertices.emplace_back(i);
    }

    int num_arestas = completo ? (num_vertices * (num_vertices - 1)) / (direcionado ? 1 : 2) : num_vertices - 1;

    std::ofstream out(saida);
    if (!out)
    {
        throw std::runtime_error("Erro ao criar o arquivo de saída");
    }

    out << num_vertices << " " << dirigido << " " << vertices_ponderados << " " << arestas_ponderadas << "\n";

    for (int i = 0; i < num_arestas; ++i)
    {
        int origem = rand() % num_vertices + 1;
        int destino = rand() % num_vertices + 1;
        while (destino == origem)
        {
            destino = rand() % num_vertices + 1;
        }

        int peso = arestas_ponderadas ? (rand() % 20 - 10) : 1; // Pesos entre -10 e 10 para arestas ponderadas

        vertices[origem - 1].adjacentes.push_back(Aresta(destino, peso));
        if (!direcionado)
        {
            vertices[destino - 1].adjacentes.push_back(Aresta(origem, peso));
        }

        out << origem << " " << destino;
        if (arestas_ponderadas)
        {
            out << " " << peso;
        }
        out << "\n";
    }

    in.close();
    out.close();
}