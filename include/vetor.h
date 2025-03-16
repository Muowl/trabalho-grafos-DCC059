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

    // Versão melhorada que manipula corretamente objetos complexos
    void redimensionar(int novaCapacidade) {
        // Não fazer nada para capacidades zero ou negativas
        if (novaCapacidade <= 0) {
            return;
        }
        
        // Caso de alocação inicial
        if (dados == nullptr) {
            dados = static_cast<T*>(malloc(novaCapacidade * sizeof(T)));
            if (dados) {
                capacidade = novaCapacidade;
            } else {
                capacidade = 0;
            }
            return;
        }
        
        // Para tipos complexos, não podemos simplesmente usar realloc
        // Precisamos alocar nova memória, construir novos objetos, e destruir os antigos
        T* novoDados = static_cast<T*>(malloc(novaCapacidade * sizeof(T)));
        if (!novoDados) return;  // Falha na alocação
        
        // Determinar quantos elementos copiar
        int elementosParaCopiar = (tamanho < novaCapacidade) ? tamanho : novaCapacidade;
        
        // Construir novos objetos usando placement new e o construtor de cópia
        for (int i = 0; i < elementosParaCopiar; i++) {
            new (&novoDados[i]) T(dados[i]);
        }
        
        // Destruir objetos antigos
        for (int i = 0; i < tamanho; i++) {
            dados[i].~T();
        }
        
        // Liberar memória antiga
        free(dados);
        
        // Atualizar ponteiro e capacidade
        dados = novoDados;
        capacidade = novaCapacidade;
        
        // Ajustar tamanho se necessário
        if (tamanho > capacidade) {
            tamanho = capacidade;
        }
    }

public:
    // Construtor padrão
    Vetor() : dados(nullptr), tamanho(0), capacidade(0) {}
    
    // Construtor com tamanho inicial
    explicit Vetor(int capacidadeInicial) : dados(nullptr), tamanho(0), capacidade(0) {
        if (capacidadeInicial > 0) {
            dados = (T*)malloc(capacidadeInicial * sizeof(T));
            if (dados) {
                capacidade = capacidadeInicial;
            }
        }
    }
    
    // Construtor de cópia - garantir cópia profunda
    Vetor(const Vetor& other) : dados(nullptr), tamanho(0), capacidade(0) {
        if (other.tamanho > 0) {
            // Alocamos memória suficiente
            dados = (T*)malloc(other.tamanho * sizeof(T));
            if (dados) {
                capacidade = other.tamanho;
                // Construímos cada elemento usando o construtor de cópia
                for (int i = 0; i < other.tamanho; i++) {
                    new(&dados[i]) T(other.dados[i]);
                }
                tamanho = other.tamanho;
            }
        }
    }
    
    // Operador de atribuição - garantir limpeza adequada antes da atribuição
    Vetor& operator=(const Vetor& other) {
        if (this != &other) {
            // Destruir todos os elementos existentes
            for (int i = 0; i < tamanho; i++) {
                dados[i].~T();
            }
            
            // Se precisarmos de mais espaço, realocar
            if (other.tamanho > capacidade) {
                T* novoDados = (T*)realloc(dados, other.tamanho * sizeof(T));
                if (novoDados) {
                    dados = novoDados;
                    capacidade = other.tamanho;
                } else if (other.tamanho > 0) {
                    // Falha na alocação - liberar tudo e começar do zero
                    free(dados);
                    dados = (T*)malloc(other.tamanho * sizeof(T));
                    if (dados) {
                        capacidade = other.tamanho;
                    } else {
                        capacidade = 0;
                    }
                }
            }
            
            // Copiar os elementos se temos memória suficiente
            if (dados && other.tamanho > 0 && capacidade >= other.tamanho) {
                for (int i = 0; i < other.tamanho; i++) {
                    new(&dados[i]) T(other.dados[i]);
                }
                tamanho = other.tamanho;
            } else {
                tamanho = 0;
            }
        }
        return *this;
    }
    
    // Destrutor - limpeza explícita e completa
    ~Vetor() {
        // Destruir todos os elementos
        for (int i = 0; i < tamanho; i++) {
            dados[i].~T();
        }
        
        // Liberar a memória
        if (dados) {
            free(dados);
            dados = nullptr;
        }
        
        tamanho = 0;
        capacidade = 0;
    }
    
    // Adicionar elemento no final
    void push_back(const T& valor) {
        // Se não há mais espaço, expandir
        if (tamanho >= capacidade) {
            int novaCapacidade = capacidade == 0 ? 1 : capacidade * 2;
            redimensionar(novaCapacidade);
        }
        
        // Se temos capacidade, adicionar elemento
        if (tamanho < capacidade) {
            new(&dados[tamanho]) T(valor);
            tamanho++;
        }
    }
    
    // Redimensionar com valor padrão para novos elementos
    void resize(int novoTamanho, const T& valorPadrao = T()) {
        if (novoTamanho < 0) return;
        
        // Se reduzindo o tamanho
        if (novoTamanho < tamanho) {
            // Destruir elementos excedentes
            for (int i = novoTamanho; i < tamanho; i++) {
                dados[i].~T();
            }
            tamanho = novoTamanho;
        }
        // Se aumentando o tamanho
        else if (novoTamanho > tamanho) {
            // Verificar se precisamos mais capacidade
            if (novoTamanho > capacidade) {
                redimensionar(novoTamanho);
            }
            
            // Inicializar novos elementos
            for (int i = tamanho; i < novoTamanho && i < capacidade; i++) {
                new(&dados[i]) T(valorPadrao);
            }
            
            tamanho = (novoTamanho <= capacidade) ? novoTamanho : capacidade;
        }
    }
    
    // Acesso aos elementos
    T& operator[](int indice) {
        return dados[indice];
    }
    
    const T& operator[](int indice) const {
        return dados[indice];
    }
    
    int size() const {
        return tamanho;
    }
    
    bool empty() const {
        return tamanho == 0;
    }
    
    // Limpar todos os elementos, mas manter a capacidade
    void clear() {
        for (int i = 0; i < tamanho; i++) {
            dados[i].~T();
        }
        tamanho = 0;
    }
};

#endif // VETOR_H
