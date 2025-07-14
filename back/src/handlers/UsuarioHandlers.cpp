#include "UsuarioHandlers.h"
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>  // utility::conversions
using namespace web;
std::vector<Usuario> usuarios;
int next_id = 0;
std::mutex usuario_mutex;

// Função para carregar usuários do arquivo
bool carregar_usuarios() {
    std::ifstream file("usuarios.txt");
    if (!file.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    if (conteudo.empty()) return false;

    json::value jsonArray;
    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    } catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    std::lock_guard<std::mutex> lock(usuario_mutex);
    usuarios.clear();
    next_id = 1;

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("id")) && item.has_field(U("nome")) && item.has_field(U("email")) &&
            item.has_field(U("senha")) && item.has_field(U("genero")) && item.has_field(U("data_nascimento")) &&
            item.has_field(U("telefone")) && item.has_field(U("cpf")) && item.has_field(U("enderecos"))) {

            int id = item.at(U("id")).as_integer();
            std::string nome = utility::conversions::to_utf8string(item.at(U("nome")).as_string());
            std::string email = utility::conversions::to_utf8string(item.at(U("email")).as_string());
            std::string senha = utility::conversions::to_utf8string(item.at(U("senha")).as_string());
            std::string genero = utility::conversions::to_utf8string(item.at(U("genero")).as_string());

            std::vector<std::string> dataNascimento;
            for (const auto& v : item.at(U("data_nascimento")).as_array()) {
                dataNascimento.push_back(utility::conversions::to_utf8string(v.as_string()));
            }

            std::string telefone = utility::conversions::to_utf8string(item.at(U("telefone")).as_string());
            std::string cpf = utility::conversions::to_utf8string(item.at(U("cpf")).as_string());

            std::vector<Usuario::Endereco> enderecos;
            for (const auto& end : item.at(U("enderecos")).as_array()) {
                if (end.has_field(U("cep")) && end.has_field(U("street")) &&
                    end.has_field(U("city")) && end.has_field(U("number"))) {
                    std::string cep = utility::conversions::to_utf8string(end.at(U("cep")).as_string());
                    std::string street = utility::conversions::to_utf8string(end.at(U("street")).as_string());
                    std::string city = utility::conversions::to_utf8string(end.at(U("city")).as_string());
                    int number = end.at(U("number")).as_integer();

                    enderecos.emplace_back(cep, street, city, number);
                }
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
    }

    return true;
}
// Função para salvar usuários no arquivo
bool salvar_usuarios() {
    json::value jsonArray = json::value::array();

    {
        std::lock_guard<std::mutex> lock(usuario_mutex);
        for (size_t i = 0; i < usuarios.size(); i++) {
            const auto& u = usuarios[i];
            json::value item;

            item[U("id")] = json::value::number(u.getId());
            item[U("nome")] = json::value::string(utility::conversions::to_string_t(u.getNome()));
            item[U("email")] = json::value::string(utility::conversions::to_string_t(u.getEmail()));
            item[U("senha")] = json::value::string(utility::conversions::to_string_t(u.getSenha()));
            item[U("genero")] = json::value::string(utility::conversions::to_string_t(u.getGenero()));

            // data_nascimento como array
            json::value dataNascimentoJson = json::value::array();
            const auto& dataNascimento = u.getDataNascimento();
            for (size_t j = 0; j < dataNascimento.size(); j++) {
                dataNascimentoJson[j] = json::value::string(utility::conversions::to_string_t(dataNascimento[j]));
            }
            item[U("data_nascimento")] = dataNascimentoJson;

            item[U("telefone")] = json::value::string(utility::conversions::to_string_t(u.getTelefone()));
            item[U("cpf")] = json::value::string(utility::conversions::to_string_t(u.getCpf()));

            // endereços como array de objetos
            json::value enderecosJson = json::value::array();
            const auto& enderecos = u.getEnderecos();
            for (size_t k = 0; k < enderecos.size(); k++) {
                json::value enderecoJson;
                enderecoJson[U("cep")] = json::value::string(utility::conversions::to_string_t(enderecos[k].cep));
                enderecoJson[U("street")] = json::value::string(utility::conversions::to_string_t(enderecos[k].street));
                enderecoJson[U("city")] = json::value::string(utility::conversions::to_string_t(enderecos[k].city));
                enderecoJson[U("number")] = json::value::number(enderecos[k].number);
                enderecosJson[k] = enderecoJson;
            }
            item[U("enderecos")] = enderecosJson;

            jsonArray[i] = item;
        }
    }

    std::ofstream file("usuarios.txt", std::ios::trunc);
    if (!file.is_open()) return false;

    std::string json_str = utility::conversions::to_utf8string(jsonArray.serialize());
    file << json_str;
    file.flush();
    file.close();
    return true;
}


// Função para inicializar o sistema, carregando os usuários
void inicializar_usuarios() {
    if (!carregar_usuarios()) {
        usuarios.clear();
        next_id = 1;
        salvar_usuarios();
    }
}

// Agora ajuste suas funções para salvar após modificar os dados:

void criar_usuario(const web::http::http_request& request) {
    request.extract_json().then([=](const web::json::value& json) {
        if (!json.has_field(U("nome")) || !json.has_field(U("email")) || !json.has_field(U("senha"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campos obrigatórios faltando"));
            return;
        }

        std::lock_guard<std::mutex> lock(usuario_mutex);

        // Opcional: verificar se email já existe
        auto it = std::find_if(usuarios.begin(), usuarios.end(),
            [&](const Usuario& u) {
                return u.getEmail() == utility::conversions::to_utf8string(json.at(U("email")).as_string());
            });
        if (it != usuarios.end()) {
            request.reply(web::http::status_codes::Conflict, U("Email já cadastrado"));
            return;
        }

        int id = next_id++;
        Usuario novo_usuario(
            id,
            utility::conversions::to_utf8string(json.at(U("nome")).as_string()),
            utility::conversions::to_utf8string(json.at(U("email")).as_string()),
            utility::conversions::to_utf8string(json.at(U("senha")).as_string())
        );

        usuarios.push_back(novo_usuario);

        salvar_usuarios();

        web::http::http_response response(web::http::status_codes::Created);
        response.set_body(web::json::value::object({
            { U("id"), web::json::value::number(id) }
        }));
        request.reply(response);
    }).wait();
}


void listar_usuarios(const web::http::http_request& request) {
    std::lock_guard<std::mutex> lock(usuario_mutex);

    web::json::value json_response = web::json::value::array();
    int idx = 0;
    for (const auto& usuario : usuarios) {
        json_response[idx++] = web::json::value::object({
            { U("id"), web::json::value::number(usuario.getId()) },
            { U("nome"), web::json::value::string(utility::conversions::to_string_t(usuario.getNome())) },
            { U("email"), web::json::value::string(utility::conversions::to_string_t(usuario.getEmail())) }
        });
    }

    request.reply(web::http::status_codes::OK, json_response);
}


void atualizar_usuario(const web::http::http_request& request, int id) {
    request.extract_json().then([=](const web::json::value& json) {
        std::lock_guard<std::mutex> lock(usuario_mutex);

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
    std::lock_guard<std::mutex> lock(usuario_mutex);

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
