#include "AluguelHandlers.h"
#include "FilmeHandlers.h"
#include "ClienteHandlers.h"
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <fstream>
#include <algorithm>


using namespace web;
using namespace http;

std::vector<Aluguel> alugueis;
int next_aluguel_id = 1;
std::mutex aluguel_mutex;

void inicializar_alugueis() {
    if (!carregar_alugueis()) {
        alugueis.clear();
        next_aluguel_id = 1;
        salvar_alugueis();
    }
}

bool carregar_alugueis() {
    std::ifstream file("alugueis.txt");
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

    std::lock_guard<std::mutex> lock(aluguel_mutex);
    alugueis.clear();
    next_aluguel_id = 1;

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("id")) && item.has_field(U("id_filme")) &&
            item.has_field(U("id_cliente")) && item.has_field(U("valor")) &&
            item.has_field(U("data_devolucao"))) {

            int id = item.at(U("id")).as_integer();
            int id_filme = item.at(U("id_filme")).as_integer();
            int id_cliente = item.at(U("id_cliente")).as_integer();
            double valor = item.at(U("valor")).as_double();
            std::string data = utility::conversions::to_utf8string(item.at(U("data_devolucao")).as_string());

            alugueis.emplace_back(id, id_filme, id_cliente, valor, data);
            if (id >= next_aluguel_id) next_aluguel_id = id + 1;
        }
    }
    return true;
}

bool salvar_alugueis() {
    std::lock_guard<std::mutex> lock(aluguel_mutex);
    json::value jsonArray = json::value::array();

    for (size_t i = 0; i < alugueis.size(); i++) {
        json::value item;
        item[U("id")] = json::value::number(alugueis[i].getId());
        item[U("id_filme")] = json::value::number(alugueis[i].getIdFilme());
        item[U("id_cliente")] = json::value::number(alugueis[i].getIdCliente());
        item[U("valor")] = json::value::number(alugueis[i].getValor());
        item[U("data_devolucao")] = json::value::string(utility::conversions::to_string_t(alugueis[i].getDataDevolucao()));
        jsonArray[i] = item;
    }

    std::ofstream file("alugueis.txt");
    if (!file.is_open()) return false;

    file << utility::conversions::to_utf8string(jsonArray.serialize());
    return true;
}

void criar_aluguel(const http_request& request) {
    request.extract_json().then([&request](json::value jsonBody) {
        try {
            if (!jsonBody.has_field(U("id_filme")) || !jsonBody.has_field(U("id_cliente")) ||
                !jsonBody.has_field(U("valor")) || !jsonBody.has_field(U("data_devolucao"))) {
                return request.reply(status_codes::BadRequest, U("Campos obrigatórios faltando"));
            }

            int id_filme = jsonBody.at(U("id_filme")).as_integer();
            int id_cliente = jsonBody.at(U("id_cliente")).as_integer();
            double valor = jsonBody.at(U("valor")).as_double();
            std::string data = utility::conversions::to_utf8string(jsonBody.at(U("data_devolucao")).as_string());

            // Verificar existência do filme
            {
                std::lock_guard<std::mutex> lock(filme_mutex);
                auto itFilme = std::find_if(filmes.begin(), filmes.end(),
                    [id_filme](const Filme& f) { return f.getId() == id_filme; });
                if (itFilme == filmes.end()) {
                    return request.reply(status_codes::BadRequest, U("Filme não encontrado"));
                }
            }

            // Verificar existência do cliente
            {
                std::lock_guard<std::mutex> lock(clientes_mutex);
                auto itCliente = std::find_if(clientes.begin(), clientes.end(),
                    [id_cliente](const Cliente& c) { return c.getUsuarioId() == id_cliente; });
                if (itCliente == clientes.end()) {
                    return request.reply(status_codes::BadRequest, U("Cliente não encontrado"));
                }
            }

            std::lock_guard<std::mutex> lock(aluguel_mutex);
            Aluguel novo(next_aluguel_id++, id_filme, id_cliente, valor, data);
            alugueis.push_back(novo);
            salvar_alugueis();

            json::value responseJson;
            responseJson[U("id")] = json::value::number(novo.getId());

            return request.reply(status_codes::Created, responseJson);
        } catch (const std::exception& e) {
            return request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON: ") + e.what()));
        }
    }).wait();
}

void listar_alugueis(const http_request& request) {
    json::value jsonArray = json::value::array();

    std::lock_guard<std::mutex> lock(aluguel_mutex);
    for (size_t i = 0; i < alugueis.size(); i++) {
        json::value item;
        item[U("id")] = json::value::number(alugueis[i].getId());
        item[U("id_filme")] = json::value::number(alugueis[i].getIdFilme());
        item[U("id_cliente")] = json::value::number(alugueis[i].getIdCliente());
        item[U("valor")] = json::value::number(alugueis[i].getValor());
        item[U("data_devolucao")] = json::value::string(utility::conversions::to_string_t(alugueis[i].getDataDevolucao()));
        jsonArray[i] = item;
    }

    request.reply(status_codes::OK, jsonArray);
}

void atualizar_aluguel(const http_request& request, int id) {
    request.extract_json().then([id, &request](json::value jsonBody) {
        try {
            if (!jsonBody.has_field(U("valor")) || !jsonBody.has_field(U("data_devolucao"))) {
                return request.reply(status_codes::BadRequest, U("Campos obrigatórios faltando"));
            }

            double valor = jsonBody.at(U("valor")).as_double();
            std::string data = utility::conversions::to_utf8string(jsonBody.at(U("data_devolucao")).as_string());

            std::lock_guard<std::mutex> lock(aluguel_mutex);
            bool atualizado = false;
            for (auto& a : alugueis) {
                if (a.getId() == id) {
                    a.setValor(valor);
                    a.setDataDevolucao(data);
                    atualizado = true;
                    break;
                }
            }

            if (!atualizado) {
                return request.reply(status_codes::NotFound, U("Aluguel não encontrado"));
            }

            salvar_alugueis();
            return request.reply(status_codes::OK, U("Aluguel atualizado com sucesso"));
        } catch (const std::exception& e) {
            return request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON: ") + e.what()));
        }
    }).wait();
}

void deletar_aluguel(const http_request& request, int id) {
    std::lock_guard<std::mutex> lock(aluguel_mutex);
    auto it = std::remove_if(alugueis.begin(), alugueis.end(),
        [id](const Aluguel& a) { return a.getId() == id; });

    if (it == alugueis.end()) {
        request.reply(status_codes::NotFound, U("Aluguel não encontrado"));
    } else {
        alugueis.erase(it, alugueis.end());
        salvar_alugueis();
        request.reply(status_codes::OK, U("Aluguel removido com sucesso"));
    }
}
