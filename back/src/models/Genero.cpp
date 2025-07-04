#include "Genero.h"

Genero::Genero(int id, const std::string& nome) : id(id), nome(nome) {}

int Genero::getId() const { return id; }
std::string Genero::getNome() const { return nome; }
void Genero::setNome(const std::string& n) { nome = n; }
