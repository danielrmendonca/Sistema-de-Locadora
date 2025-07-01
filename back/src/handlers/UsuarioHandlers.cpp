#include "UsuarioHandlers.h"
#include <mutex>
#include <algorithm>
#include <iostream>
#include <cpprest/asyncrt_utils.h>  // Para utility::conversions

std::vector<Usuario> usuarios;
int next_id = 0;
std::mutex usuarios_mutex;

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
