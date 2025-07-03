#include "UsuarioHandlers.h"
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>  // utility::conversions

std::vector<Usuario> usuarios;
int next_id = 0;
std::mutex usuarios_mutex;

// Função para carregar usuários do arquivo
bool carregar_usuarios() {
    std::ifstream arquivo("usuario.txt");
    if (!arquivo.is_open()) {
        return false; // arquivo não existe
    }

    usuarios.clear();
    std::string linha;

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        int id = std::stoi(linha);

        std::string nome;
        if (!std::getline(arquivo, nome)) return false;

        std::string email;
        if (!std::getline(arquivo, email)) return false;

        std::string senha;
        if (!std::getline(arquivo, senha)) return false;

        std::string genero;
        if (!std::getline(arquivo, genero)) return false;

        std::string dataNascimentoLinha;
        if (!std::getline(arquivo, dataNascimentoLinha)) return false;
        std::istringstream ssData(dataNascimentoLinha);
        std::vector<std::string> dataNascimento;
        std::string parte;
        while (ssData >> parte) {
            dataNascimento.push_back(parte);
        }

        std::string telefone;
        if (!std::getline(arquivo, telefone)) return false;

        std::string cpf;
        if (!std::getline(arquivo, cpf)) return false;

        std::string qntEnderecosLinha;
        if (!std::getline(arquivo, qntEnderecosLinha)) return false;
        int qntEnderecos = std::stoi(qntEnderecosLinha);

        std::vector<Usuario::Endereco> enderecos;
        for (int i = 0; i < qntEnderecos; ++i) {
            std::string cep, street, city;
            int number;

            if (!std::getline(arquivo, cep)) return false;
            if (!std::getline(arquivo, street)) return false;
            if (!std::getline(arquivo, city)) return false;

            std::string numberStr;
            if (!std::getline(arquivo, numberStr)) return false;
            number = std::stoi(numberStr);

            enderecos.emplace_back(cep, street, city, number);
        }

        Usuario u(id, nome, email, senha);
        u.setGenero(genero);
        u.setDataNascimento(dataNascimento);
        u.setTelefone(telefone);
        u.setCpf(cpf);
        u.setEnderecos(enderecos);

        usuarios.push_back(u);

        if (id >= next_id) next_id = id + 1;
    }

    return true;
}

// Função para salvar usuários no arquivo
bool salvar_usuarios() {
    std::ofstream arquivo("usuario.txt");
    if (!arquivo.is_open()) return false;

    for (const auto& u : usuarios) {
        arquivo << u.getId() << "\n";
        arquivo << u.getNome() << "\n";
        arquivo << u.getEmail() << "\n";
        arquivo << u.getSenha() << "\n";
        arquivo << u.getGenero() << "\n";

        for (const auto& parte : u.getDataNascimento()) {
            arquivo << parte << " ";
        }
        arquivo << "\n";

        arquivo << u.getTelefone() << "\n";
        arquivo << u.getCpf() << "\n";

        const auto& enderecos = u.getEnderecos();
        arquivo << enderecos.size() << "\n";

        for (const auto& endereco : enderecos) {
            arquivo << endereco.cep << "\n";
            arquivo << endereco.street << "\n";
            arquivo << endereco.city << "\n";
            arquivo << endereco.number << "\n";
        }
    }

    return true;
}

// Função para inicializar o sistema, carregando os usuários
void inicializar_usuarios() {
    std::lock_guard<std::mutex> lock(usuarios_mutex);
    if (carregar_usuarios()) {
        std::cout << "Usuários carregados do arquivo." << std::endl;
    } else {
        std::cout << "Nenhum usuário encontrado no arquivo." << std::endl;
    }
}

// Agora ajuste suas funções para salvar após modificar os dados:

void criar_usuario(const web::http::http_request& request) {
    request.extract_json().then([=](const web::json::value& json) {
        if (!json.has_field(U("nome")) || !json.has_field(U("email")) || !json.has_field(U("senha"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campos obrigatórios faltando"));
            return;
        }

        std::lock_guard<std::mutex> lock(usuarios_mutex);

        Usuario novo_usuario(next_id++,
            utility::conversions::to_utf8string(json.at(U("nome")).as_string()),
            utility::conversions::to_utf8string(json.at(U("email")).as_string()),
            utility::conversions::to_utf8string(json.at(U("senha")).as_string()));

        usuarios.push_back(novo_usuario);

        salvar_usuarios();

        web::http::http_response response(web::http::status_codes::Created);
        std::vector<std::pair<utility::string_t, web::json::value>> props = {
            { U("id"), web::json::value::number(novo_usuario.getId()) }
        };
        response.set_body(web::json::value::object(std::move(props)));
        request.reply(response);
    }).wait();
}

void listar_usuarios(const web::http::http_request& request) {
    std::lock_guard<std::mutex> lock(usuarios_mutex);

    web::json::value json_response = web::json::value::array();
    int idx = 0;
    for (const auto& usuario : usuarios) {
        std::vector<std::pair<utility::string_t, web::json::value>> props = {
            { U("id"), web::json::value::number(usuario.getId()) },
            { U("nome"), web::json::value::string(utility::conversions::to_string_t(usuario.getNome())) },
            { U("email"), web::json::value::string(utility::conversions::to_string_t(usuario.getEmail())) }
        };
        json_response[idx++] = web::json::value::object(std::move(props));
    }

    web::http::http_response response(web::http::status_codes::OK);
    response.set_body(json_response);
    request.reply(response);
}

void atualizar_usuario(const web::http::http_request& request, int id) {
    request.extract_json().then([=](const web::json::value& json) {
        std::lock_guard<std::mutex> lock(usuarios_mutex);

        auto it = std::find_if(usuarios.begin(), usuarios.end(),
            [id](const Usuario& u) { return u.getId() == id; });

        if (it != usuarios.end()) {
            if (json.has_field(U("nome"))) it->setNome(utility::conversions::to_utf8string(json.at(U("nome")).as_string()));
            if (json.has_field(U("email"))) it->setEmail(utility::conversions::to_utf8string(json.at(U("email")).as_string()));
            if (json.has_field(U("senha"))) it->setSenha(utility::conversions::to_utf8string(json.at(U("senha")).as_string()));

            salvar_usuarios();

            web::http::http_response response(web::http::status_codes::OK);
            std::vector<std::pair<utility::string_t, web::json::value>> props = {
                { U("message"), web::json::value::string(U("Usuário atualizado com sucesso")) }
            };
            response.set_body(web::json::value::object(std::move(props)));
            request.reply(response);
        }
        else {
            request.reply(web::http::status_codes::NotFound, U("Usuário não encontrado"));
        }
    }).wait();
}

void deletar_usuario(const web::http::http_request& request, int id) {
    std::lock_guard<std::mutex> lock(usuarios_mutex);

    auto it = std::remove_if(usuarios.begin(), usuarios.end(),
        [id](const Usuario& u) { return u.getId() == id; });

    if (it != usuarios.end()) {
        usuarios.erase(it, usuarios.end());

        salvar_usuarios();

        web::http::http_response response(web::http::status_codes::OK);
        std::vector<std::pair<utility::string_t, web::json::value>> props = {
            { U("message"), web::json::value::string(U("Usuário deletado com sucesso")) }
        };
        response.set_body(web::json::value::object(std::move(props)));
        request.reply(response);
    }
    else {
        request.reply(web::http::status_codes::NotFound, U("Usuário não encontrado"));
    }
}
