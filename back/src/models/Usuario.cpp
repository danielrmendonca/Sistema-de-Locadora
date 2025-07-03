#include "Usuario.h"
using namespace std;

Usuario::Usuario(int id, const std::string& nome, const std::string& email, const std::string& senha)
    : id(id), nome(nome), email(email), senha(senha) {
}

// Getters
int Usuario::getId() const {
    return id;
}

string Usuario::getNome() const {
    return nome;
}

string Usuario::getEmail() const {
    return email;
}

string Usuario::getSenha() const {
    return senha;
}

string Usuario::getGenero() const {
    return genero;
}

vector<string> Usuario::getDataNascimento() const {
    return dataNascimento;
}

vector<Usuario::Endereco> Usuario::getEnderecos() const {
    return enderecos;
}

string Usuario::getTelefone() const {
    return telefone;
}

string Usuario::getCpf() const {
    return cpf;
}

// Setters
void Usuario::setNome(const std::string& nome) {
    this->nome = nome;
}

void Usuario::setEmail(const std::string& email) {
    this->email = email;
}

void Usuario::setSenha(const std::string& senha) {
    this->senha = senha;
}

void Usuario::setGenero(const std::string& genero) {
    this->genero = genero;
}

void Usuario::setDataNascimento(const vector<string>& dataNascimento) {
    this->dataNascimento = dataNascimento;
}

void Usuario::setEnderecos(const vector<Endereco>& enderecos) {
    this->enderecos = enderecos;
}

void Usuario::setTelefone(const std::string& telefone) {
    this->telefone = telefone;
}

void Usuario::setCpf(const std::string& cpf) {
    this->cpf = cpf;
}

// Métodos para adicionar individualmente um endereço ou uma data de nascimento
void Usuario::addEndereco(const Endereco& endereco) {
    enderecos.push_back(endereco);
}

void Usuario::addDataNascimento(const std::string& valor) {
    dataNascimento.push_back(valor);
}
