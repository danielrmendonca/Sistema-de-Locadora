#ifndef GENERO_H
#define GENERO_H

#include <string>
// Definição da clase Genero e seus getters e setters
class Genero {
private:
    int id;
    std::string nome;

public:

    //Duas formas de criar um genero, Temos 2 construtores, um inicia um genero vazio e o segundo com parametros esperados
    Genero() = default;
    Genero(int id, const std::string& nome);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso Genero.cpp
    int getId() const;
    std::string getNome() const;
    
    // Setters - Deixamos que a implementação dos Setters fiquem a cargo do nosso Genero.cpp
    void setNome(const std::string& nome);
};

#endif // GENERO_H
