#include "FilmeHandlers.h"
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <fstream>
#include <algorithm>

using namespace web;
using namespace http;
using namespace http::experimental::listener;

std::vector<Filme> filmes;
int next_filme_id = 1;
std::mutex filme_mutex;

void inicializar_filmes() {
    if (!carregar_filmes()) {
        filmes.clear();
        next_filme_id = 1;
        salvar_filmes();
    }
}

bool carregar_filmes() {
    std::ifstream file("filmes.txt");
    if (!file.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    json::value jsonArray;
    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    } catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    std::lock_guard<std::mutex> lock(filme_mutex);
    filmes.clear();
    next_filme_id = 1;

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("id")) && item.has_field(U("dublado")) && item.has_field(U("legendado")) &&
            item.has_field(U("lancamento")) && item.has_field(U("classificacao_indicativa")) &&
            item.has_field(U("diretor")) && item.has_field(U("titulo")) &&
            item.has_field(U("duracao")) && item.has_field(U("nota"))) {

            int id = item.at(U("id")).as_integer();
            bool dublado = item.at(U("dublado")).as_bool();
            bool legendado = item.at(U("legendado")).as_bool();

            std::string lancamento = utility::conversions::to_utf8string(item.at(U("lancamento")).as_string());
            std::string classificacao = utility::conversions::to_utf8string(item.at(U("classificacao_indicativa")).as_string());
            std::string diretor = utility::conversions::to_utf8string(item.at(U("diretor")).as_string());
            std::string titulo = utility::conversions::to_utf8string(item.at(U("titulo")).as_string());

            double duracao = item.at(U("duracao")).as_double();
            double nota = item.at(U("nota")).as_double();

            filmes.emplace_back(id, dublado, legendado, lancamento, classificacao, diretor, titulo, duracao, nota);
            if (id >= next_filme_id) next_filme_id = id + 1;
        }
    }
    return true;
}

bool salvar_filmes() {
    // Removi o lock_guard daqui!
    json::value jsonArray = json::value::array();

    for (size_t i = 0; i < filmes.size(); i++) {
        json::value item;
        item[U("id")] = json::value::number(filmes[i].getId());
        item[U("dublado")] = json::value::boolean(filmes[i].isDublado());
        item[U("legendado")] = json::value::boolean(filmes[i].isLegendado());

        item[U("lancamento")] = json::value::string(utility::conversions::to_string_t(filmes[i].getLancamento()));
        item[U("classificacao_indicativa")] = json::value::string(utility::conversions::to_string_t(filmes[i].getClassificacaoIndicativa()));
        item[U("diretor")] = json::value::string(utility::conversions::to_string_t(filmes[i].getDiretor()));
        item[U("titulo")] = json::value::string(utility::conversions::to_string_t(filmes[i].getTitulo()));

        item[U("duracao")] = json::value::number(filmes[i].getDuracao());
        item[U("nota")] = json::value::number(filmes[i].getNota());
        jsonArray[i] = item;
    }

    std::ofstream file("filmes.txt");
    if (!file.is_open()) return false;

    std::string json_str = utility::conversions::to_utf8string(jsonArray.serialize());
    file << json_str;
    return true;
}

void criar_filme(const http_request& request) {
    request.extract_json().then([&request](json::value jsonBody) {
        try {
            if (!jsonBody.has_field(U("dublado")) || !jsonBody.has_field(U("legendado")) ||
                !jsonBody.has_field(U("lancamento")) || !jsonBody.has_field(U("classificacao_indicativa")) ||
                !jsonBody.has_field(U("diretor")) || !jsonBody.has_field(U("titulo")) ||
                !jsonBody.has_field(U("duracao")) || !jsonBody.has_field(U("nota"))) {
                return request.reply(status_codes::BadRequest, U("Campos obrigatórios faltando"));
            }

            bool dublado = jsonBody.at(U("dublado")).as_bool();
            bool legendado = jsonBody.at(U("legendado")).as_bool();

            std::string lancamento = utility::conversions::to_utf8string(jsonBody.at(U("lancamento")).as_string());
            std::string classificacao = utility::conversions::to_utf8string(jsonBody.at(U("classificacao_indicativa")).as_string());
            std::string diretor = utility::conversions::to_utf8string(jsonBody.at(U("diretor")).as_string());
            std::string titulo = utility::conversions::to_utf8string(jsonBody.at(U("titulo")).as_string());

            double duracao = jsonBody.at(U("duracao")).as_double();
            double nota = jsonBody.at(U("nota")).as_double();

            std::lock_guard<std::mutex> lock(filme_mutex);
            Filme novoFilme(next_filme_id++, dublado, legendado, lancamento, classificacao, diretor, titulo, duracao, nota);
            filmes.push_back(novoFilme);
            salvar_filmes();

            json::value responseJson;
            responseJson[U("id")] = json::value::number(novoFilme.getId());
            responseJson[U("titulo")] = json::value::string(utility::conversions::to_string_t(novoFilme.getTitulo()));

            return request.reply(status_codes::Created, responseJson);
        }
        catch (const std::exception& e) {
            return request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON: ") + e.what()));
        }
    }).wait();
}

void listar_filmes(const http_request& request) {
    json::value jsonArray = json::value::array();

    std::lock_guard<std::mutex> lock(filme_mutex);
    for (size_t i = 0; i < filmes.size(); i++) {
        json::value item;
        item[U("id")] = json::value::number(filmes[i].getId());
        item[U("dublado")] = json::value::boolean(filmes[i].isDublado());
        item[U("legendado")] = json::value::boolean(filmes[i].isLegendado());

        item[U("lancamento")] = json::value::string(utility::conversions::to_string_t(filmes[i].getLancamento()));
        item[U("classificacao_indicativa")] = json::value::string(utility::conversions::to_string_t(filmes[i].getClassificacaoIndicativa()));
        item[U("diretor")] = json::value::string(utility::conversions::to_string_t(filmes[i].getDiretor()));
        item[U("titulo")] = json::value::string(utility::conversions::to_string_t(filmes[i].getTitulo()));

        item[U("duracao")] = json::value::number(filmes[i].getDuracao());
        item[U("nota")] = json::value::number(filmes[i].getNota());
        jsonArray[i] = item;
    }

    request.reply(status_codes::OK, jsonArray);
}

void atualizar_filme(const http_request& request, int id) {
    request.extract_json().then([id, &request](json::value jsonBody) {
        if (!jsonBody.has_field(U("dublado")) || !jsonBody.has_field(U("legendado")) ||
            !jsonBody.has_field(U("lancamento")) || !jsonBody.has_field(U("classificacao_indicativa")) ||
            !jsonBody.has_field(U("diretor")) || !jsonBody.has_field(U("titulo")) ||
            !jsonBody.has_field(U("duracao")) || !jsonBody.has_field(U("nota"))) {
            return request.reply(status_codes::BadRequest, U("Campos inválidos"));
        }

        bool dublado = jsonBody.at(U("dublado")).as_bool();
        bool legendado = jsonBody.at(U("legendado")).as_bool();

        std::string lancamento = utility::conversions::to_utf8string(jsonBody.at(U("lancamento")).as_string());
        std::string classificacao = utility::conversions::to_utf8string(jsonBody.at(U("classificacao_indicativa")).as_string());
        std::string diretor = utility::conversions::to_utf8string(jsonBody.at(U("diretor")).as_string());
        std::string titulo = utility::conversions::to_utf8string(jsonBody.at(U("titulo")).as_string());

        double duracao = jsonBody.at(U("duracao")).as_double();
        double nota = jsonBody.at(U("nota")).as_double();

        std::lock_guard<std::mutex> lock(filme_mutex);
        bool atualizado = false;
        for (auto& f : filmes) {
            if (f.getId() == id) {
                f.setDublado(dublado);
                f.setLegendado(legendado);
                f.setLancamento(lancamento);
                f.setClassificacaoIndicativa(classificacao);
                f.setDiretor(diretor);
                f.setTitulo(titulo);
                f.setDuracao(duracao);
                f.setNota(nota);
                atualizado = true;
                break;
            }
        }

        if (!atualizado) {
            return request.reply(status_codes::NotFound, U("Filme não encontrado"));
        } else {
            salvar_filmes();
            return request.reply(status_codes::OK, U("Filme atualizado com sucesso"));
        }
    }).wait();
}

void deletar_filme(const http_request& request, int id) {
    std::lock_guard<std::mutex> lock(filme_mutex);
    auto it = std::remove_if(filmes.begin(), filmes.end(),
                             [id](const Filme& f) { return f.getId() == id; });
    if (it == filmes.end()) {
        request.reply(status_codes::NotFound, U("Filme não encontrado"));
    } else {
        filmes.erase(it, filmes.end());
        salvar_filmes();
        request.reply(status_codes::OK, U("Filme removido com sucesso"));
    }
}
