#include "GeneroHandlers.h"
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <fstream>
#include <algorithm>
#include <mutex>

using namespace web;
using namespace http;
using namespace http::experimental::listener;

std::vector<Genero> generos;
int next_genero_id = 1;
std::mutex genero_mutex;


void inicializar_generos() {
    if (!carregar_generos()) {
        generos.clear();
        next_genero_id = 1;
        salvar_generos();
    }
}

bool carregar_generos() {
    std::ifstream file("generos.txt");
    if (!file.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    if (conteudo.empty()) return false;  // Arquivo vazio, nada a carregar

    json::value jsonArray;
    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    } catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    std::lock_guard<std::mutex> lock(genero_mutex);
    generos.clear();
    next_genero_id = 1;

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("id")) && item.has_field(U("nome"))) {
            int id = item.at(U("id")).as_integer();
            std::string nome = utility::conversions::to_utf8string(item.at(U("nome")).as_string());

            generos.emplace_back(id, nome);
            if (id >= next_genero_id) next_genero_id = id + 1;
        }
    }
    return true;
}

bool salvar_generos() {
    json::value jsonArray = json::value::array();

    {
        std::lock_guard<std::mutex> lock(genero_mutex);
        for (size_t i = 0; i < generos.size(); i++) {
            json::value item;
            item[U("id")] = json::value::number(generos[i].getId());
            item[U("nome")] = json::value::string(utility::conversions::to_string_t(generos[i].getNome()));
            jsonArray[i] = item;
        }
    }

    std::ofstream file("generos.txt", std::ios::trunc);
    if (!file.is_open()) return false;

    std::string json_str = utility::conversions::to_utf8string(jsonArray.serialize());
    file << json_str;
    file.flush();
    file.close();
    return true;
}

void criar_genero(const http_request& request) {
    // Capture request por valor (cópia) para evitar dangling reference
    request.extract_json().then([request](pplx::task<json::value> task) mutable {
        try {
            json::value jsonBody = task.get();

            if (!jsonBody.has_field(U("nome"))) {
                request.reply(status_codes::BadRequest, U("Campo 'nome' é obrigatório"));
                return;
            }

            std::string nome = utility::conversions::to_utf8string(jsonBody.at(U("nome")).as_string());

            int idGerado = 0;
            {
                std::lock_guard<std::mutex> lock(genero_mutex);
                Genero novoGenero(next_genero_id++, nome);
                generos.push_back(novoGenero);
                idGerado = novoGenero.getId();

                if (!salvar_generos()) {
                    request.reply(status_codes::InternalError, U("Erro ao salvar arquivo de generos"));
                    return;
                }
            }

            json::value responseJson;
            responseJson[U("id")] = json::value::number(idGerado);
            responseJson[U("nome")] = json::value::string(utility::conversions::to_string_t(nome));

            request.reply(status_codes::Created, responseJson);
        }
        catch (const std::exception& e) {
            request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON ou salvando gênero: ") + e.what()));
        }
    });
}

void listar_generos(const http_request& request) {
    json::value jsonArray = json::value::array();

    {
        std::lock_guard<std::mutex> lock(genero_mutex);
        for (size_t i = 0; i < generos.size(); i++) {
            json::value item;
            item[U("id")] = json::value::number(generos[i].getId());
            item[U("nome")] = json::value::string(utility::conversions::to_string_t(generos[i].getNome()));
            jsonArray[i] = item;
        }
    }

    request.reply(status_codes::OK, jsonArray);
}

void atualizar_genero(const http_request& request, int id) {
    request.extract_json().then([request, id](pplx::task<json::value> task) mutable {
        try {
            json::value jsonBody = task.get();

            if (!jsonBody.has_field(U("nome"))) {
                request.reply(status_codes::BadRequest, U("Campo nome é obrigatório"));
                return;
            }

            std::string novoNome = utility::conversions::to_utf8string(jsonBody.at(U("nome")).as_string());

            bool atualizado = false;
            {
                std::lock_guard<std::mutex> lock(genero_mutex);
                for (auto& g : generos) {
                    if (g.getId() == id) {
                        g.setNome(novoNome);
                        atualizado = true;
                        break;
                    }
                }
                if (atualizado) {
                    salvar_generos();
                }
            }

            if (!atualizado) {
                request.reply(status_codes::NotFound, U("Gênero não encontrado"));
            } else {
                request.reply(status_codes::OK, U("Gênero atualizado com sucesso"));
            }
        }
        catch (const std::exception& e) {
            request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON ou salvando gênero: ") + e.what()));
        }
    });
}

void deletar_genero(const http_request& request, int id) {
    bool encontrado = false;
    {
        std::lock_guard<std::mutex> lock(genero_mutex);
        auto it = std::remove_if(generos.begin(), generos.end(),
                                 [id](const Genero& g) { return g.getId() == id; });
        if (it != generos.end()) {
            generos.erase(it, generos.end());
            encontrado = true;
            salvar_generos();
        }
    }

    if (!encontrado) {
        request.reply(status_codes::NotFound, U("Gênero não encontrado"));
    } else {
        request.reply(status_codes::OK, U("Gênero removido com sucesso"));
    }
}
