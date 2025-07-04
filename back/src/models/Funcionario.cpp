#include "Funcionario.h"

// Implementação de como funciona o a criação de um objeto do tipo Funcionario, atribuindo os valores ao construir o objeto. Também tem a implementação dos getters e setters

Funcionario::Funcionario(int usuario_id, const std::vector<std::string>& data_admissao,
                         const std::string& cargo, double salario,
                         const std::vector<std::string>& data_desligamento)
    : usuario_id(usuario_id),
      data_admissao(data_admissao),
      cargo(cargo),
      salario(salario),
      data_desligamento(data_desligamento) {}

int Funcionario::getUsuarioId() const {
    return usuario_id;
}

std::vector<std::string> Funcionario::getDataAdmissao() const {
    return data_admissao;
}

std::string Funcionario::getCargo() const {
    return cargo;
}

double Funcionario::getSalario() const {
    return salario;
}

std::vector<std::string> Funcionario::getDataDesligamento() const {
    return data_desligamento;
}

void Funcionario::setUsuarioId(int id) {
    usuario_id = id;
}

void Funcionario::setDataAdmissao(const std::vector<std::string>& data) {
    data_admissao = data;
}

void Funcionario::setCargo(const std::string& c) {
    cargo = c;
}

void Funcionario::setSalario(double s) {
    salario = s;
}

void Funcionario::setDataDesligamento(const std::vector<std::string>& data) {
    data_desligamento = data;
}
