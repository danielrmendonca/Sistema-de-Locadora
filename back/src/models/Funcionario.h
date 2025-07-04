#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <string>
#include <vector>

// Definição da clase funcionario e seus getters e setters

class Funcionario {
private:
    int usuario_id;                          // ID do usuário associado
    std::vector<std::string> data_admissao;  // Ex.: {"dd", "mm", "aaaa"}
    std::string cargo;
    double salario;
    std::vector<std::string> data_desligamento; // Vazio se ainda não desligado

public:

    //Duas formas de criar um funcionario, Temos 2 construtores, um inicia um funcionario vazio e o segundo com parametros esperados
    Funcionario() = default;

    Funcionario(int usuario_id,
                const std::vector<std::string>& data_admissao,
                const std::string& cargo,
                double salario,
                const std::vector<std::string>& data_desligamento);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso Funcionario.cpp
    int getUsuarioId() const;
    std::vector<std::string> getDataAdmissao() const;
    std::string getCargo() const;
    double getSalario() const;
    std::vector<std::string> getDataDesligamento() const;

    // Setters - Deixamos que a implementação dos Setters fiquem a cargo do nosso Funcionario.cpp
    void setUsuarioId(int id);
    void setDataAdmissao(const std::vector<std::string>& data);
    void setCargo(const std::string& cargo);
    void setSalario(double salario);
    void setDataDesligamento(const std::vector<std::string>& data);
};

#endif // FUNCIONARIO_H
