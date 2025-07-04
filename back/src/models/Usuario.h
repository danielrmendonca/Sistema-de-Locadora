#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
// Definição da clase Usuario e seus getters e setters
class Usuario {
public:
    struct Endereco {
        std::string cep;
        std::string street;
        std::string city;
        int number;

        Endereco() = default;
        Endereco(const std::string& cep, const std::string& street, const std::string& city, int number)
            : cep(cep), street(street), city(city), number(number) {}
    };

private:
    int id;
    std::string nome;
    std::string email;
    std::string senha;

    std::string genero;
    std::vector<std::string> dataNascimento; // Exemplo: {"dd", "mm", "aaaa"}
    std::vector<Endereco> enderecos;          // Multivalorado: pode ter vários endereços
    std::string telefone;
    std::string cpf;

public:

    //Duas formas de criar um usuario, Temos 2 construtores, um inicia um usuario vazio e o segundo com parametros esperados
    Usuario() = default;
    Usuario(int id, const std::string& nome, const std::string& email, const std::string& senha);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso Usuario.cpp

    int getId() const;
    std::string getNome() const;
    std::string getEmail() const;
    std::string getSenha() const;

    std::string getGenero() const;
    std::vector<std::string> getDataNascimento() const;
    std::vector<Endereco> getEnderecos() const;
    std::string getTelefone() const;
    std::string getCpf() const;

    // Setters - Deixamos que a implementação dos Setters fiquem a cargo do nosso Usuario.cpp

    void setNome(const std::string& nome);
    void setEmail(const std::string& email);
    void setSenha(const std::string& senha);

    void setGenero(const std::string& genero);
    void setDataNascimento(const std::vector<std::string>& dataNascimento);
    void setEnderecos(const std::vector<Endereco>& enderecos);
    void setTelefone(const std::string& telefone);
    void setCpf(const std::string& cpf);

    // Métodos para adicionar um endereço e uma data de nascimento (opcional)
    void addEndereco(const Endereco& endereco);
    void addDataNascimento(const std::string& valor);
};

#endif // USUARIO_H
