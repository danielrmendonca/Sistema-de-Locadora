#include "ClienteHandlers.h"
#include <mutex>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>
#include "handlers/UsuarioHandlers.h" // Para acessar usuarios e seus ids

using namespace web;
using namespace http;
using namespace http::experimental::listener;
std::vector<Cliente> clientes;
std::mutex cliente_mutex;

// Função responsável por carregar nosso arquivo que contem os dados de cliente
bool carregar_clientes() {
    std::ifstream file("cliente.txt");
    if (!file.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    json::value jsonArray;
    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    }
    catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    std::lock_guard<std::mutex> lock(cliente_mutex);
    clientes.clear();

    for (const auto& item : jsonArray.as_array()) {
        if (item.has_field(U("usuario_id")) &&
            item.has_field(U("limite")) &&
            item.has_field(U("data_ingresso"))) {

            int usuario_id = item.at(U("usuario_id")).as_integer();
            double limite = item.at(U("limite")).as_double();

            std::vector<std::string> dataIngresso;
            for (const auto& parte : item.at(U("data_ingresso")).as_array()) {
                dataIngresso.push_back(utility::conversions::to_utf8string(parte.as_string()));
            }

            Cliente c(usuario_id, limite, dataIngresso);
            clientes.push_back(c);
        }
    }

    return true;
}

// Função responsável por salvar no .xtt os dados do cliente que estamos cadastrando
bool salvar_clientes() {
    json::value jsonArray = json::value::array();

    for (size_t i = 0; i < clientes.size(); i++) {
        const Cliente& c = clientes[i];

        json::value item;
        item[U("usuario_id")] = json::value::number(c.getUsuarioId());
        item[U("limite")] = json::value::number(c.getLimiteCredito());

        json::value data_array = json::value::array();
        const auto& data = c.getDataIngresso();
        for (size_t j = 0; j < data.size(); j++) {
            data_array[j] = json::value::string(utility::conversions::to_string_t(data[j]));
        }

        item[U("data_ingresso")] = data_array;

        jsonArray[i] = item;
    }

    std::ofstream file("cliente.txt");
    if (!file.is_open()) return false;

    std::string json_str = utility::conversions::to_utf8string(jsonArray.serialize());
    file << json_str;

    return true;
}

void inicializar_clientes() {
    if (!carregar_clientes()) {
        clientes.clear();
        next_id = 1;
        salvar_clientes();
    }
}
// Função responsável por fazer o cadastro do cliente ao chamar uma rota do tipo POST e com os parametros necessarios sendo passados no Body da requisição
void criar_cliente(const web::http::http_request& request) {
    request.extract_json().then([=](const web::json::value& json) {
        if (!json.has_field(U("usuario_id")) ||
            !json.has_field(U("limite_credito")) ||
            !json.has_field(U("data_ingresso"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campos obrigatórios faltando"));
            return;
        }

        int usuario_id = json.at(U("usuario_id")).as_integer();

        // Verifica se usuário existe
        std::lock_guard<std::mutex> lock(cliente_mutex);

        auto usuario_it = std::find_if(usuarios.begin(), usuarios.end(),
            [usuario_id](const Usuario& u) { return u.getId() == usuario_id; });

        if (usuario_it == usuarios.end()) {
            request.reply(web::http::status_codes::NotFound, U("Usuário não encontrado para associar ao cliente"));
            return;
        }

        std::vector<std::string> data;
        for (const auto& v : json.at(U("data_ingresso")).as_array())
            data.push_back(utility::conversions::to_utf8string(v.as_string()));

        Cliente novo_cliente(usuario_id,
            json.at(U("limite_credito")).as_double(),
            data);

        clientes.push_back(novo_cliente);
        salvar_clientes();

        request.reply(web::http::status_codes::Created, U("Cliente cadastrado com sucesso"));
    }).wait();
}
// Função que lista todos os clientes
void listar_clientes(const http_request& request) {
    json::value jsonArray = json::value::array();

    std::lock_guard<std::mutex> lock(cliente_mutex);
    for (size_t i = 0; i < clientes.size(); i++) {
        const auto& c = clientes[i];
        json::value item;

        item[U("usuario_id")] = json::value::number(c.getUsuarioId());
        item[U("limite")] = json::value::number(c.getLimiteCredito());

        json::value dataArray = json::value::array();
        const auto& data = c.getDataIngresso();
        for (size_t j = 0; j < data.size(); j++) {
            dataArray[j] = json::value::string(utility::conversions::to_string_t(data[j]));
        }

        item[U("data_ingresso")] = dataArray;

        jsonArray[i] = item;
    }

    request.reply(status_codes::OK, jsonArray);
}
// Função responsável por atualizar um determinado cliente, buscando o cliente pelo id
void atualizar_cliente(const http_request& request, int usuario_id) {
    request.extract_json().then([usuario_id, &request](json::value jsonBody) {
        if (!jsonBody.has_field(U("limite")) || !jsonBody.has_field(U("data_ingresso"))) {
            return request.reply(status_codes::BadRequest, U("Campos obrigatórios ausentes"));
        }

        double novo_limite = jsonBody.at(U("limite")).as_double();

        std::vector<std::string> nova_data;
        for (const auto& parte : jsonBody.at(U("data_ingresso")).as_array()) {
            nova_data.push_back(utility::conversions::to_utf8string(parte.as_string()));
        }

        std::lock_guard<std::mutex> lock(cliente_mutex);
        bool encontrado = false;
        for (auto& c : clientes) {
            if (c.getUsuarioId() == usuario_id) {
                c.setLimiteCredito(novo_limite);
                c.setDataIngresso(nova_data);
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            return request.reply(status_codes::NotFound, U("Cliente não encontrado"));
        }

        salvar_clientes();
        return request.reply(status_codes::OK, U("Cliente atualizado com sucesso"));
    }).wait();
}
// Função que deleta um cliente
void deletar_cliente(const http_request& request, int usuario_id) {
    std::lock_guard<std::mutex> lock(cliente_mutex);
    auto it = std::remove_if(clientes.begin(), clientes.end(),
                             [usuario_id](const Cliente& c) { return c.getUsuarioId() == usuario_id; });

    if (it == clientes.end()) {
        request.reply(status_codes::NotFound, U("Cliente não encontrado"));
    } else {
        clientes.erase(it, clientes.end());
        salvar_clientes();
        request.reply(status_codes::OK, U("Cliente removido com sucesso"));
    }
}