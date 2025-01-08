#include "grafo_matriz.h"

bool GrafoMatriz::eh_bipartido() const{
    int* cor = new int[ordem];
    for(int i = 0; i < ordem; i++){
        cor[i] = 0;
    }
    //busca em largura
    auto bel = [&](int inicio) -> bool {
        int* fila = new int[ordem];
        int frente = 0, atras = 0;
        fila[atras++] = inicio;
        cor[inicio] = 1;
        while(frente != atras){
            int u = fila[frente++];
            for(int v = 0; v < ordem; v++){
                if(matriz[u][v] != 0){
                    if(cor[v] == 0){
                        cor[v] = (cor[u] == 1) ? 2 : 1;
                        fila[atras++] = v;
                    }else if(cor[v] == cor[u]){
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
    //componentes conexas
    for(int i = 0; i < ordem; i++){
        if(cor[i] == 0){
            if(!bel(i)){
                delete[] cor;
                return false;
            }
        }
    }
    delete[] cor;
    return true;
}

int GrafoMatriz::n_conexo() const{
    //implementar
    return 0;
}

int GrafoMatriz::get_grau(int vertice) const{
    int grau = 0;
    for(int i = 0; i < ordem; i++){
        grau += matriz[vertice][i];
    }
    return grau
}

int GrafoMatriz::get_ordem() const{
    //implementar
    return ordem;
}

bool GrafoMatriz::eh_direcionado() const{
    //implementar
    return direcionado;
}

bool GrafoMatriz::vertice_ponderado() const{
    //implementar
    return vertices_ponderados;
}

bool GrafoMatriz::aresta_ponderada() const{
    //implementar
    return arestas_ponderadas;
}

bool GrafoMatriz::eh_completo() const{
    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            if(i != j && matriz[i][j] == 0){
                return false;
            }
        }
    }
    return true;
}

bool GrafoMatriz::eh_arvore() const{
    //implementar
    return false;
}

bool GrafoMatriz::possui_articulacao() const{
    //implementar
    return false;
}

bool GrafoMatriz::possui_ponte() const{
    //implementar
    return false;
}

void GrafoMatriz::carrega_grafo(const std::string& arquivo){
    std::ifstream in(arquivo);
    if(!in){
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    int ord, dir, vp, ap;
    in >> ord >> dir >> vp >> ap;

    ordem = ord;
    direcionado = dir;
    vertices_ponderados = vp;
    arestas_ponderadas = ap;

    matriz.assign(ordem, vector<int>(ordem, 0));
    vector<int> pesos_vertices;

    if(vertices_ponderados){
        pesos_vertices.resize(ordem);
        for(int i = 0; i < ordem; i++){
            in >> pesos_vertices[i]
        }
    }

    int origem, destino, peso;
    while(in >> origem >> destino){
        if(arestas_ponderadas){
            in >> peso;
        }
        matriz[origem-1][destino-1] = peso;
        if(!direcionado){
            matriz[destino-1][origem-1] = peso;
        }
    }
    in.close();
}

void GrafoMatriz::novo_grafo(const std::string& descricao, const std::string& saida){
    std::ifstream config(descricao);
    if(!config){
        throw std::runtime_error("Erro ao abrir o arquivo de descrição!");
    }
    
    int grau, ord, dir, vp, ap;
    config >> grau >> ord >> dir;
    config >> dir >> vp >> ap;

    ordem = ord;
    direcionado = dir;
    vertices_ponderados = vp;
    arestas_ponderadas = ap;

    matriz.assign(ordem, vector<int>(ordem,0));
    vector<int> pesos_vertices;

    if(vertices_ponderados){
        pesos_vertices.resize(ordem);
        for(int i = 0; i < ordem; i++){
            pesos_vertices[i] = rand() % 10 + 1;
        }
    }

    srand(time(nullptr));
    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            if(i != j && rand() % 2){
                int peso = arestas_ponderadas ? rand() % 10 + 1 : 1;
                matriz[i][j] = peso;
                if(!direcionado){
                    matriz[j][i] = peso;
                }
            }
        }
    }
    config.close();

    std::ofstream output(saida);
    if(!output){
        throw std::runtime_error("Erro ao salvar o grafo!");
    }

    output << ordem << "" << direcionado <<  "" << vertices_ponderados << "" << arestas_ponderadas << endl;
    if(vertices_ponderados){
        for(const auto& peso : pesos_vertices){
            output << peso << "";
        }
        output << endl;
    }

    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            if(matriz[i][j] != 0){
                output << i+1 << "" << j+1 << "" << matriz[i][j] << endl;
            }
        }
    }
    output.close();
}