#include "ClienteHandlers.h"
#include <mutex>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>
#include "handlers/UsuarioHandlers.h" // Para acessar usuarios e seus ids

std::vector<Cliente> clientes;
std::mutex clientes_mutex;

// Função responsável por carregar nosso arquivo que contem os dados de cliente
bool carregar_clientes() {
    std::ifstream arquivo("cliente.txt");
    if (!arquivo.is_open()) return false;

    clientes.clear();
    std::string linha;

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        int usuario_id = std::stoi(linha);

        std::string limiteLinha;
        if (!std::getline(arquivo, limiteLinha)) return false;
        double limite = std::stod(limiteLinha);

        std::string dataIngressoLinha;
        if (!std::getline(arquivo, dataIngressoLinha)) return false;
        std::istringstream ssData(dataIngressoLinha);
        std::vector<std::string> dataIngresso;
        std::string parte;
        while (ssData >> parte) dataIngresso.push_back(parte);

        Cliente c(usuario_id, limite, dataIngresso);
        clientes.push_back(c);
    }
    return true;
}
// Função responsável por salvar no .xtt os dados do cliente que estamos cadastrando
bool salvar_clientes() {
    std::ofstream arquivo("cliente.txt");
    if (!arquivo.is_open()) return false;

    for (const auto& c : clientes) {
        arquivo << c.getUsuarioId() << "\n";
        arquivo << c.getLimiteCredito() << "\n";

        for (const auto& parte : c.getDataIngresso())
            arquivo << parte << " ";
        arquivo << "\n";
    }
    return true;
}
// Funlção que verifica se ha clientes cadastrados ou não no arquivo
void inicializar_clientes() {
    std::lock_guard<std::mutex> lock(clientes_mutex);
    if (carregar_clientes()) {
        std::cout << "Clientes carregados do arquivo.\n";
    } else {
        std::cout << "Nenhum cliente encontrado no arquivo.\n";
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
        std::lock_guard<std::mutex> lock(clientes_mutex);

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
void listar_clientes(const web::http::http_request& request) {
    std::lock_guard<std::mutex> lock(clientes_mutex);
    web::json::value resposta = web::json::value::array();
    int idx = 0;
    for (const auto& c : clientes) {
        resposta[idx++] = web::json::value::object({
            { U("usuario_id"), web::json::value::number(c.getUsuarioId()) },
            { U("limite_credito"), web::json::value::number(c.getLimiteCredito()) }
        });
    }
    request.reply(web::http::status_codes::OK, resposta);
}
// Função responsável por atualizar um determinado cliente, buscando o cliente pelo id
void atualizar_cliente(const web::http::http_request& request, int usuario_id) {
    request.extract_json().then([=](const web::json::value& json) {
        std::lock_guard<std::mutex> lock(clientes_mutex);
        auto it = std::find_if(clientes.begin(), clientes.end(),
            [usuario_id](const Cliente& c) { return c.getUsuarioId() == usuario_id; });

        if (it != clientes.end()) {
            if (json.has_field(U("limite_credito"))) it->setLimiteCredito(json.at(U("limite_credito")).as_double());
            if (json.has_field(U("data_ingresso"))) {
                std::vector<std::string> data;
                for (const auto& v : json.at(U("data_ingresso")).as_array())
                    data.push_back(utility::conversions::to_utf8string(v.as_string()));
                it->setDataIngresso(data);
            }

            salvar_clientes();
            request.reply(web::http::status_codes::OK, U("Cliente atualizado com sucesso"));
        } else {
            request.reply(web::http::status_codes::NotFound, U("Cliente não encontrado"));
        }
    }).wait();
}
// Função que deleta um cliente
void deletar_cliente(const web::http::http_request& request, int usuario_id) {
    std::lock_guard<std::mutex> lock(clientes_mutex);
    auto it = std::remove_if(clientes.begin(), clientes.end(),
        [usuario_id](const Cliente& c) { return c.getUsuarioId() == usuario_id; });

    if (it != clientes.end()) {
        clientes.erase(it, clientes.end());
        salvar_clientes();
        request.reply(web::http::status_codes::OK, U("Cliente deletado com sucesso"));
    } else {
        request.reply(web::http::status_codes::NotFound, U("Cliente não encontrado"));
    }
}
