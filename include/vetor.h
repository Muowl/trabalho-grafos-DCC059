#ifndef VETOR_H
#define VETOR_H

#include <cstdlib>  // para malloc, realloc, free
#include <cstring>  // para memset

template <typename T>
class Vetor {
private:
    T* dados;
    int tamanho;
    int capacidade;

    void redimensionar(int novaCapacidade) {
        // Não fazer nada para capacidades zero ou negativas
        if (novaCapacidade <= 0) {
            return;
        }
        
        // Caso de alocação inicial
        if (dados == nullptr) {
            dados = (T*)malloc(novaCapacidade * sizeof(T));
            if (dados) {
                capacidade = novaCapacidade;
            } else {
                // Falha na alocação
                capacidade = 0;
            }
            return;
        }
        
        // Redimensionando dados existentes - usar um novo ponteiro para evitar vazamentos de memória
        T* novoDados = (T*)realloc(dados, novaCapacidade * sizeof(T));
        if (novoDados) {
            dados = novoDados;
            capacidade = novaCapacidade;
        }
        // Se realloc falhar, o bloco original permanece intacto
    }

public:
    // Construtor padrão - inicializa explicitamente todos os campos
    Vetor() : dados(nullptr), tamanho(0), capacidade(0) {}
    
    // Construtor com capacidade inicial
    Vetor(int capacidadeInicial) : dados(nullptr), tamanho(0), capacidade(0) {
        if (capacidadeInicial > 0) {
            dados = (T*)malloc(capacidadeInicial * sizeof(T));
            if (dados) {
                capacidade = capacidadeInicial;
            }
        }
    }
    
    // Construtor de cópia
    Vetor(const Vetor& other) : dados(nullptr), tamanho(0), capacidade(0) {
        if (other.tamanho > 0) {
            // Alocar exatamente o que precisamos
            dados = (T*)malloc(other.tamanho * sizeof(T));
            if (dados) {
                capacidade = other.tamanho;
                tamanho = other.tamanho;
                
                // Copiar cada elemento (usa o construtor de cópia para T)
                for (int i = 0; i < tamanho; i++) {
                    new(&dados[i]) T(other.dados[i]);
                }
            }
        }
    }
    
    // Operador de atribuição
    Vetor& operator=(const Vetor& other) {
        if (this != &other) {
            // Liberar dados existentes
            if (dados) {
                free(dados);
                dados = nullptr;
            }
            
            tamanho = 0;
            capacidade = 0;
            
            // Alocar e copiar se necessário
            if (other.tamanho > 0) {
                dados = (T*)malloc(other.tamanho * sizeof(T));
                if (dados) {
                    capacidade = other.tamanho;
                    tamanho = other.tamanho;
                    
                    // Copiar cada elemento (usa o operador de atribuição para T)
                    for (int i = 0; i < tamanho; i++) {
                        new(&dados[i]) T(other.dados[i]);
                    }
                }
            }
        }
        return *this;
    }
    
    // Destrutor - libera corretamente a memória
    ~Vetor() {
        if (dados) {
            // Chamar destrutores para objetos se necessário (para tipos não-POD)
            for (int i = 0; i < tamanho; i++) {
                dados[i].~T();
            }
            free(dados);
        }
    }
    
    void push_back(const T& valor) {
        // Verifica se precisamos expandir
        if (tamanho >= capacidade) {
            int novaCapacidade = capacidade == 0 ? 1 : capacidade * 2;
            redimensionar(novaCapacidade);
        }
        
        // Só prosseguir se tivermos capacidade suficiente
        if (dados && tamanho < capacidade) {
            // Usar placement new para construir objeto no local
            new(&dados[tamanho]) T(valor);
            tamanho++;
        }
    }
    
    void resize(int novoTamanho, const T& valorPadrao = T()) {
        // Não permitir tamanhos negativos
        if (novoTamanho < 0) {
            return;
        }
        
        // Se reduzindo o tamanho
        if (novoTamanho < tamanho) {
            // Chamar destrutores para elementos removidos
            for (int i = novoTamanho; i < tamanho; i++) {
                dados[i].~T();
            }
            tamanho = novoTamanho;
            return;
        }
        
        // Se aumentando o tamanho
        if (novoTamanho > capacidade) {
            redimensionar(novoTamanho);
        }
        
        // Inicializar novos elementos se tivermos capacidade
        if (dados) {
            for (int i = tamanho; i < novoTamanho && i < capacidade; i++) {
                new(&dados[i]) T(valorPadrao);
            }
        }
        
        tamanho = (novoTamanho <= capacidade) ? novoTamanho : capacidade;
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
        // Chamar destrutores para todos os elementos
        for (int i = 0; i < tamanho; i++) {
            dados[i].~T();
        }
        // Redefinir tamanho, mas manter a memória alocada
        tamanho = 0;
    }
};

#endif // VETOR_H
