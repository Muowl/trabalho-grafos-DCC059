#ifndef VETOR_H
#define VETOR_H

#include <cstdlib>  // para malloc, realloc, free

template <typename T>
class Vetor {
private:
    T* dados;
    int tamanho;
    int capacidade;

    void redimensionar(int novaCapacidade) {
        T* novoDados = (T*)realloc(dados, novaCapacidade * sizeof(T));
        if (novoDados) {
            dados = novoDados;
            capacidade = novaCapacidade;
        }
    }

public:
    Vetor() : dados(nullptr), tamanho(0), capacidade(0) {}
    
    Vetor(int capacidadeInicial) : tamanho(0), capacidade(capacidadeInicial) {
        dados = (T*)malloc(capacidadeInicial * sizeof(T));
    }
    
    ~Vetor() {
        if (dados) {
            free(dados);
        }
    }
    
    void push_back(const T& valor) {
        if (tamanho >= capacidade) {
            redimensionar(capacidade == 0 ? 1 : capacidade * 2);
        }
        dados[tamanho++] = valor;
    }
    
    void resize(int novoTamanho, const T& valorPadrao = T()) {
        if (novoTamanho > capacidade) {
            redimensionar(novoTamanho);
        }
        
        // Inicializar novos elementos com o valor padrão
        for (int i = tamanho; i < novoTamanho; i++) {
            dados[i] = valorPadrao;
        }
        
        tamanho = novoTamanho;
    }
    
    T& operator[](int indice) {
        return dados[indice];
    }
    
    const T& operator[](int indice) const {
        return dados[indice];
    }
    
    int size() const {
        return tamanho;
    }
    
    void clear() {
        tamanho = 0;
    }
};

#endif // VETOR_H
