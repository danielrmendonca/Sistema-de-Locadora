#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include <vector>

class Cliente {
private:
    int usuario_id;                       // ID do usuário já cadastrado
    double limite_credito;
    std::vector<std::string> data_ingresso; // Ex.: {"dd", "mm", "aaaa"}

public:
    Cliente() = default;
    Cliente(int usuario_id, double limite_credito, const std::vector<std::string>& data_ingresso);

    // Getters
    int getUsuarioId() const;
    double getLimiteCredito() const;
    std::vector<std::string> getDataIngresso() const;

    // Setters
    void setUsuarioId(int id);
    void setLimiteCredito(double limite);
    void setDataIngresso(const std::vector<std::string>& data);
};

#endif // CLIENTE_H
