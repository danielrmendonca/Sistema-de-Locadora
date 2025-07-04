#ifndef GENERO_H
#define GENERO_H

#include <string>

class Genero {
private:
    int id;
    std::string nome;

public:
    Genero() = default;
    Genero(int id, const std::string& nome);

    int getId() const;
    std::string getNome() const;
    void setNome(const std::string& nome);
};

#endif // GENERO_H
