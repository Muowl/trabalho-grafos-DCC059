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
    bool* visitado = new bool[ordem]();
    int componentes = 0;
    //busca em profundidade
    auto bep = [&](int v, auto& bep_refs) -> void {
        visitado[v] = true;
        for(int u = 0; u < ordem; u++){
            if(matriz[v][u] != 0 && !visitado[u]){
                bep_refs(u, bep_refs);
            }
        }
    };
    
    for(int i = 0; i < ordem; i++){
        if(!visitado[i]){
            ++componentes;
            bep(i, bep);
        }
    }
    delete[] visitado;
    return componentes;  
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

    in >> ordem;

    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            matriz[i][j] = 0;
        }
    }

    int origem, destino, peso;
    while(in >> origem >> destino >> peso){
        matriz[origem-1][destino-1] = peso;
        if(!direcionado){
            matriz[destino-1][origem-1] = peso;
        }
    }
    in.close();
}

void GrafoMatriz::novo_grafo(const std::string& descricao, const std::string& saida){
    std::ifstream in(descricao);
    if(!in){
        throw std::runtime_error("Erro ao abrir o arquivo de descrição!");
    }
    
    in >> ordem;
    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            matriz[i][j] = 0;
        }
    }

    srand(time(nullptr)){
        for(int i = 0; i < ordem; i++){
            for(int j = 0; j < ordem; j++){
                if(rand() % 2){ //verifica se uma aresta foi criada entre 2 vertices gerados
                    matriz[i][j] = matriz[j][i] = rand() % 10 + 1;
                }
            }
        }
    }

    in.close();

    std::ofstream out(saida);
    if(!out){
        throw std::runtime_error("Erro ao criar o arquivo de saida!");
    }

    out << ordem << " " << direcionado << " " << vertices_ponderados << " " << arestas_ponderadas << endl;
    for(int i = 0; i < ordem; i++){
        for(int j = 0; j < ordem; j++){
            if(matriz[i][j] != 0){
                out << i+1 << "" << j+1 << "" << matriz[i][j] << endl;
            }
        }
    }

    out.close();
}