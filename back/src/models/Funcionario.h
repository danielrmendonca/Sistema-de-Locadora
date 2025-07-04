#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <string>
#include <vector>

class Funcionario {
private:
    int usuario_id;                          // ID do usuário associado
    std::vector<std::string> data_admissao;  // Ex.: {"dd", "mm", "aaaa"}
    std::string cargo;
    double salario;
    std::vector<std::string> data_desligamento; // Vazio se ainda não desligado

public:
    Funcionario() = default;

    Funcionario(int usuario_id,
                const std::vector<std::string>& data_admissao,
                const std::string& cargo,
                double salario,
                const std::vector<std::string>& data_desligamento);

    // Getters
    int getUsuarioId() const;
    std::vector<std::string> getDataAdmissao() const;
    std::string getCargo() const;
    double getSalario() const;
    std::vector<std::string> getDataDesligamento() const;

    // Setters
    void setUsuarioId(int id);
    void setDataAdmissao(const std::vector<std::string>& data);
    void setCargo(const std::string& cargo);
    void setSalario(double salario);
    void setDataDesligamento(const std::vector<std::string>& data);
};

#endif // FUNCIONARIO_H
