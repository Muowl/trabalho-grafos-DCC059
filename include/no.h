#pragma once
#include <string>

class No {
private:
    int id;
    std::string info;
    int grau; // Number of connections

public:
    No(int id = -1, const std::string& info = "")
        : id(id), info(info), grau(0) {}

    int getId() const { return id; }
    std::string getInfo() const { return info; }
    int getGrau() const { return grau; }
    
    void setId(int i) { id = i; }
    void setInfo(const std::string& i) { info = i; }
    void incrementarGrau() { grau++; }
    void decrementarGrau() { if (grau > 0) grau--; }
    void setGrau(int g) { grau = g; }
};
