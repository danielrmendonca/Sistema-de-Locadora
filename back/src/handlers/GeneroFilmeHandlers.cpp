#include "GeneroFilmeHandlers.h"
#include "FilmeHandlers.h"
#include "GeneroHandlers.h"
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <fstream>
#include <algorithm>

using namespace web;
using namespace http;

// Se você quiser manter as associações em memória, pode criar isso:
// std::vector<std::pair<int, int>> genero_filmes;
// std::mutex genero_filme_mutex;

void criar_genero_filme(const http_request& request) {
    request.extract_json().then([&request](json::value jsonBody) {
        try {
            if (!jsonBody.has_field(U("filme_id")) || !jsonBody.has_field(U("genero_id"))) {
                return request.reply(status_codes::BadRequest, U("Campos 'filme_id' e 'genero_id' são obrigatórios"));
            }

            int filme_id = jsonBody.at(U("filme_id")).as_integer();
            int genero_id = jsonBody.at(U("genero_id")).as_integer();

            // Verificar se o filme existe
            {
                std::lock_guard<std::mutex> lock(filme_mutex);
                auto itFilme = std::find_if(filmes.begin(), filmes.end(),
                                           [filme_id](const Filme& f) { return f.getId() == filme_id; });
                if (itFilme == filmes.end()) {
                    return request.reply(status_codes::NotFound, U("Filme não encontrado"));
                }
            }

            // Verificar se o gênero existe
            {
                std::lock_guard<std::mutex> lock(genero_mutex);
                auto itGenero = std::find_if(generos.begin(), generos.end(),
                                            [genero_id](const Genero& g) { return g.getId() == genero_id; });
                if (itGenero == generos.end()) {
                    return request.reply(status_codes::NotFound, U("Gênero não encontrado"));
                }
            }

            // Salvar associação no arquivo
            if (!salvar_genero_filme(filme_id, genero_id)) {
                return request.reply(status_codes::InternalError, U("Erro ao salvar genero_filme.txt"));
            }

            // Opcional: se quiser guardar em memória, acrescente aqui
            // std::lock_guard<std::mutex> lock(genero_filme_mutex);
            // genero_filmes.emplace_back(filme_id, genero_id);

            return request.reply(status_codes::Created, U("Associação criada com sucesso"));
        }
        catch (const std::exception& e) {
            return request.reply(status_codes::BadRequest, utility::conversions::to_string_t(
                std::string("Erro processando JSON: ") + e.what()));
        }
    }).wait();
}

bool salvar_genero_filme(int filme_id, int genero_id) {
    std::ofstream file("genero_filme.txt", std::ios::app);
    if (!file.is_open()) return false;

    file << filme_id << "," << genero_id << "\n";
    return true;
}
