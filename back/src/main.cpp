#include <cpprest/http_listener.h>
#include <iostream>
#include <thread>
#include "handlers/UsuarioHandlers.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main() {
    http_listener listener(U("http://localhost:8080"));

    listener.support(methods::POST, [](const http_request& request) {
        if (request.relative_uri().path() == U("/usuario")) {
            criar_usuario(request);
        }
        });

    listener.support(methods::GET, [](const http_request& request) {
        if (request.relative_uri().path() == U("/usuario")) {
            listar_usuarios(request);
        }
        });

    listener.support(methods::PATCH, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());
        if (path.size() == 2 && path[0] == U("usuario")) {
            int id = std::stoi(path[1]);
            atualizar_usuario(request, id);
        }
        else {
            request.reply(status_codes::BadRequest, "Formato da URL incorreto");
        }
        });

    listener.support(methods::DEL, [](const http_request& request) {
        auto path = uri::split_path(request.relative_uri().path());
        if (path.size() == 2 && path[0] == U("usuario")) {
            int id = std::stoi(path[1]);
            deletar_usuario(request, id);
        }
        else {
            request.reply(status_codes::BadRequest, "Formato da URL incorreto");
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
    }
    catch (const std::exception& e) {
        std::cerr << "Erro no servidor: " << e.what() << std::endl;
    }

    return 0;
}
