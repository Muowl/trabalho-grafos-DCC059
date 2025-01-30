#include "leitura.h"

using namespace std;

int main() {
    leitura l("../entradas/grafo.txt");
    // Impressões simples para testes
    std::cout << "Num nos : " << l.get_num_nos() << std::endl;
    std::cout << "Direcionado : " << l.get_direcionado() << std::endl;
    std::cout << "Ponderado nos : " << l.get_ponderado_vertices() << std::endl;
    std::cout << "Ponderado arestas : " << l.get_ponderado_arestas() << std::endl;

    // Chamar a função de impressão de nós, precisa de ajustar a implementação
    l.imprimir_matriz_nos();
    
    return 0;
}
