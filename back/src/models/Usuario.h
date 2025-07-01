#ifndef USUARIO_H
#define USUARIO_H


#include <string>
class Usuario {
private:
    int id;
    std::string nome;
    std::string email;
    std::string senha;

public:
    Usuario() = default;
    Usuario(int id, const std::string& nome, const std::string& email, const std::string& senha);

    int getId() const;
    std::string getNome() const;
    std::string getEmail() const;
    std::string getSenha() const;

    void setNome(const std::string& nome);
    void setEmail(const std::string& email);
    void setSenha(const std::string& senha);

};

#endif // USUARIO_H