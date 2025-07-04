#ifndef ALUGUEL_H
#define ALUGUEL_H

#include <string>

class Aluguel {
private:
    int id;
    int id_filme;
    int id_cliente;
    double valor;
    std::string data_devolucao;

public:
    Aluguel() = default;
    Aluguel(int id, int id_filme, int id_cliente, double valor, const std::string& data_devolucao);

    int getId() const;
    int getIdFilme() const;
    int getIdCliente() const;
    double getValor() const;
    std::string getDataDevolucao() const;

    void setValor(double valor);
    void setDataDevolucao(const std::string& data_devolucao);
};

#endif // ALUGUEL_H
