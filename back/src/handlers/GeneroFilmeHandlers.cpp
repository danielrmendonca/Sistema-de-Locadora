#include "GeneroFilmeHandlers.h"
#include "FilmeHandlers.h"
#include "GeneroHandlers.h"
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace web;
using namespace http;

std::vector<GeneroFilme> genero_filmes;
std::mutex genero_filme_mutex;

void inicializar_genero_filmes() {
    if (!carregar_genero_filmes()) {
        genero_filmes.clear();
        next_genero_id = 1;
        salvar_genero_filmes();
    }
}

bool carregar_genero_filmes() {
    std::ifstream arquivo("genero_filme.txt");
    if (!arquivo.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    json::value jsonArray;

    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    } catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    genero_filmes.clear();

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("filme_id")) && item.has_field(U("genero_id"))) {
            int filme_id = item.at(U("filme_id")).as_integer();
            int genero_id = item.at(U("genero_id")).as_integer();
            genero_filmes.emplace_back(filme_id, genero_id);
        }
    }

    return true;
}

bool salvar_genero_filmes() {
    json::value jsonArray = json::value::array();

    for (size_t i = 0; i < genero_filmes.size(); ++i) {
        const auto& gf = genero_filmes[i];
        json::value item;
        item[U("filme_id")] = json::value::number(gf.getFilmeId());
        item[U("genero_id")] = json::value::number(gf.getGeneroId());
        jsonArray[i] = item;
    }

    std::ofstream arquivo("genero_filme.txt");
    if (!arquivo.is_open()) return false;

    std::string jsonStr = utility::conversions::to_utf8string(jsonArray.serialize());
    arquivo << jsonStr;
    return true;
}

void criar_genero_filme(const web::http::http_request& request) {
    request.extract_json().then([=](const web::json::value& json) {
        if (!json.has_field(U("descricao"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campo 'descricao' é obrigatório"));
            return;
        }

        std::string descricao = utility::conversions::to_utf8string(json.at(U("descricao")).as_string());

        std::lock_guard<std::mutex> lock(genero_filme_mutex);

        int novo_id = 1;
        for (const auto& g : generos) {
            if (g.getId() >= novo_id) {
                novo_id = g.getId() + 1;
            }
        }

        Genero novo_genero(novo_id, descricao);
        generos.push_back(novo_genero);
        salvar_generos();

        request.reply(web::http::status_codes::Created, U("Gênero criado com sucesso"));
    }).wait();
}


void listar_genero_filme(const web::http::http_request& request) {
    std::lock_guard<std::mutex> lock(genero_filme_mutex);
    json::value resposta = json::value::array();

    for (size_t i = 0; i < generos.size(); ++i) {
        resposta[i] = json::value::object({
            { U("id"), json::value::number(generos[i].getId()) },
            { U("descricao"), json::value::string(utility::conversions::to_string_t(generos[i].getNome())) }
        });
    }

    request.reply(web::http::status_codes::OK, resposta);
}

void atualizar_genero_filme(const web::http::http_request& request, int id) {
    request.extract_json().then([=](const web::json::value& json) {
        std::lock_guard<std::mutex> lock(genero_filme_mutex);

        auto it = std::find_if(generos.begin(), generos.end(),
            [id](const Genero& g) { return g.getId() == id; });

        if (it == generos.end()) {
            request.reply(web::http::status_codes::NotFound, U("Gênero não encontrado"));
            return;
        }

        if (!json.has_field(U("descricao"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campo 'descricao' é obrigatório"));
            return;
        }

        it->setNome(utility::conversions::to_utf8string(json.at(U("descricao")).as_string()));

        salvar_generos();
        request.reply(web::http::status_codes::OK, U("Gênero atualizado com sucesso"));
    }).wait();
}

void deletar_genero_filme(const web::http::http_request& request, int filme_id, int genero_id) {
    std::lock_guard<std::mutex> lock(genero_filme_mutex);

    auto it = std::remove_if(genero_filmes.begin(), genero_filmes.end(),
        [filme_id, genero_id](const GeneroFilme& gf) {
            return gf.getFilmeId() == filme_id && gf.getGeneroId() == genero_id;
        });

    if (it == genero_filmes.end()) {
        request.reply(web::http::status_codes::NotFound, U("Associação não encontrada"));
    } else {
        genero_filmes.erase(it, genero_filmes.end());
        salvar_genero_filmes();
        request.reply(web::http::status_codes::OK, U("Associação deletada com sucesso"));
    }
}
