#include <cpprest/http_listener.h>
#include <iostream>
#include <thread>

#include "handlers/UsuarioHandlers.h"
#include "handlers/ClienteHandlers.h"
#include "handlers/FilmeHandlers.h"
#include "handlers/GeneroHandlers.h"
#include "handlers/GeneroFilmeHandlers.h"
#include "handlers/AluguelHandlers.h"  

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main() {
    // Inicializa dados carregando arquivos
    inicializar_usuarios();
    inicializar_clientes();
    inicializar_filmes();
    inicializar_generos();
    inicializar_genero_filmes();
    inicializar_alugueis(); 

    http_listener listener(U("http://localhost:8080"));

    listener.support(methods::POST, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());
        if (path.empty()) {
            request.reply(status_codes::BadRequest, "Requisição sem caminho");
            return;
        }

        if (path[0] == U("usuario")) {
            criar_usuario(request);
        } else if (path[0] == U("cliente")) {
            criar_cliente(request);
        } else if (path[0] == U("filme")) {
            criar_filme(request);
        } else if (path[0] == U("genero")) {
            criar_genero(request);
        } else if (path[0] == U("genero_filme")) {
            criar_genero_filme(request);
        } else if (path[0] == U("aluguel")) {   
            criar_aluguel(request);
        } else {
            request.reply(status_codes::NotFound, "Rota POST não encontrada");
        }
    });

    listener.support(methods::GET, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());
        if (path.empty()) {
            request.reply(status_codes::BadRequest, "Requisição sem caminho");
            return;
        }

        if (path[0] == U("usuario")) {
            listar_usuarios(request);
        } else if (path[0] == U("cliente")) {
            listar_clientes(request);
        } else if (path[0] == U("filme")) {
            listar_filmes(request);
        } else if (path[0] == U("genero")) {
            listar_generos(request);
        } else if (path[0] == U("genero_filme")) {
            listar_genero_filmes(request);
        } else if (path[0] == U("aluguel")) { 
            listar_alugueis(request);
        } else {
            request.reply(status_codes::NotFound, "Rota GET não encontrada");
        }
    });

    listener.support(methods::PATCH, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());
        if (path.size() == 2) {
            int id = 0;
            try {
                id = std::stoi(path[1]);
            } catch (...) {
                request.reply(status_codes::BadRequest, "ID inválido");
                return;
            }

            if (path[0] == U("usuario")) {
                atualizar_usuario(request, id);
            } else if (path[0] == U("cliente")) {
                atualizar_cliente(request, id);
            } else if (path[0] == U("filme")) {
                atualizar_filme(request, id);
            } else if (path[0] == U("genero")) {
                atualizar_genero(request, id);
            } else if (path[0] == U("aluguel")) {  
                atualizar_aluguel(request, id);
            } else {
                request.reply(status_codes::BadRequest, "Rota PATCH inválida");
            }
        } else {
            request.reply(status_codes::BadRequest, "Formato da URL incorreto para PATCH");
        }
    });

    listener.support(methods::DEL, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());

        if (path.empty()) {
            request.reply(status_codes::BadRequest, "Requisição sem caminho");
            return;
        }

        if (path[0] == U("usuario") || path[0] == U("cliente") || path[0] == U("filme") || path[0] == U("genero")) {
            if (path.size() == 2) {
                int id = 0;
                try {
                    id = std::stoi(path[1]);
                } catch (...) {
                    request.reply(status_codes::BadRequest, "ID inválido");
                    return;
                }

                if (path[0] == U("usuario")) {
                    deletar_usuario(request, id);
                } else if (path[0] == U("cliente")) {
                    deletar_cliente(request, id);
                } else if (path[0] == U("filme")) {
                    deletar_filme(request, id);
                } else if (path[0] == U("genero")) {
                    deletar_genero(request, id);
                }
            } else {
                request.reply(status_codes::BadRequest, "Formato da URL incorreto para DELETE");
            }
        } else if (path[0] == U("genero_filme")) {
            deletar_genero_filme(request);
        } else if (path[0] == U("aluguel")) {  
            if (path.size() == 2) {
                int id = 0;
                try {
                    id = std::stoi(path[1]);
                } catch (...) {
                    request.reply(status_codes::BadRequest, "ID inválido");
                    return;
                }
                deletar_aluguel(request, id);
            } else {
                request.reply(status_codes::BadRequest, "Formato da URL incorreto para DELETE");
            }
        } else {
            request.reply(status_codes::NotFound, "Rota DELETE não encontrada");
        }
    });

    try {
        listener
            .open()
            .then([]() { std::wcout << L"Servidor rodando em http://localhost:8080\n"; })
            .wait();

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro no servidor: " << e.what() << std::endl;
    }

    return 0;
}
