#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include <vector>

// Definição da clase cliente e seus getters e setters
class Cliente {
private:
    int usuario_id;                       // ID do usuário já cadastrado
    double limite_credito;
    std::vector<std::string> data_ingresso; // Ex.: {"dd", "mm", "aaaa"}

public:
    //Duas formas de criar um Cliente, Temos 2 construtores, um inicia um cliente vazio e o segundo com parametros esperados
    Cliente() = default;
    Cliente(int usuario_id, double limite_credito, const std::vector<std::string>& data_ingresso);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso Cliente.cpp
    int getUsuarioId() const;
    double getLimiteCredito() const;
    std::vector<std::string> getDataIngresso() const;

    // Setters - Deixamos que a implementação dos Setters fiquem a cargo do nosso Cliente.cpp
    void setUsuarioId(int id);
    void setLimiteCredito(double limite);
    void setDataIngresso(const std::vector<std::string>& data);
};

#endif // CLIENTE_H
