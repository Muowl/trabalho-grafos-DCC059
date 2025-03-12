#pragma once
#include <string>

class No {
private:
    int id;
    std::string info;

public:
    No(int id = -1, const std::string& info = "")
        : id(id), info(info) {}

    int getId() const { return id; }
    std::string getInfo() const { return info; }
    
    void setId(int i) { id = i; }
    void setInfo(const std::string& i) { info = i; }
};
