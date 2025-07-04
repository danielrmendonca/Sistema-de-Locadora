#include "Aluguel.h"

Aluguel::Aluguel(int id, int id_filme, int id_cliente, double valor, const std::string& data_devolucao)
    : id(id), id_filme(id_filme), id_cliente(id_cliente), valor(valor), data_devolucao(data_devolucao) {}

int Aluguel::getId() const { return id; }
int Aluguel::getIdFilme() const { return id_filme; }
int Aluguel::getIdCliente() const { return id_cliente; }
double Aluguel::getValor() const { return valor; }
std::string Aluguel::getDataDevolucao() const { return data_devolucao; }

void Aluguel::setValor(double v) { valor = v; }
void Aluguel::setDataDevolucao(const std::string& d) { data_devolucao = d; }
