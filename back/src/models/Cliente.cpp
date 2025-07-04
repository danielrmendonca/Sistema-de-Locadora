#include "Cliente.h"

// Implementação de como funciona o a criação de um objeto do tipo cliente, atribuindo os valores ao construir o objeto. Também tem a implementação dos getters e setters

Cliente::Cliente(int usuario_id, double limite_credito, const std::vector<std::string>& data_ingresso)
    : usuario_id(usuario_id), limite_credito(limite_credito), data_ingresso(data_ingresso) {}

int Cliente::getUsuarioId() const {
    return usuario_id;
}

double Cliente::getLimiteCredito() const {
    return limite_credito;
}

std::vector<std::string> Cliente::getDataIngresso() const {
    return data_ingresso;
}

void Cliente::setUsuarioId(int id) {
    usuario_id = id;
}

void Cliente::setLimiteCredito(double limite) {
    limite_credito = limite;
}

void Cliente::setDataIngresso(const std::vector<std::string>& data) {
    data_ingresso = data;
}
