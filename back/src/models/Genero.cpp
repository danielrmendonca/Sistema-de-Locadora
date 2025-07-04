#include "Genero.h"

// Implementação de como funciona o a criação de um objeto do tipo Genero, atribuindo os valores ao construir o objeto. Também tem a implementação dos getters e setters

Genero::Genero(int id, const std::string& nome) : id(id), nome(nome) {}

int Genero::getId() const { return id; }
std::string Genero::getNome() const { return nome; }
void Genero::setNome(const std::string& n) { nome = n; }
