#pragma once
#include <string>

class No {
private:
    int id;
    std::string info;
    int grau; // Número de conexões

public:
    // Construtor padrão e construtor parametrizado
    No(int id = -1, const std::string& info = "")
        : id(id), info(info), grau(0) {}
    
    // Construtor de cópia adequado
    No(const No& other) 
        : id(other.id), info(other.info), grau(other.grau) {}
    
    // Operador de atribuição adequado
    No& operator=(const No& other) {
        if (this != &other) {
            id = other.id;
            info = other.info;
            grau = other.grau;
        }
        return *this;
    }
    
    // Destrutor (usa comportamento padrão)
    ~No() = default;

    int getId() const { return id; }
    std::string getInfo() const { return info; }
    int getGrau() const { return grau; }
    
    void setId(int i) { id = i; }
    void setInfo(const std::string& i) { info = i; }
    void incrementarGrau() { grau++; }
    void decrementarGrau() { if (grau > 0) grau--; }
    void setGrau(int g) { grau = g; }
};
