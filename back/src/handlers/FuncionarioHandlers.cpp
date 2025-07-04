#include "FuncionarioHandlers.h"
#include <mutex>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>
#include "handlers/UsuarioHandlers.h" // Para acessar usuarios

std::vector<Funcionario> funcionarios;
std::mutex funcionarios_mutex;

bool carregar_funcionarios() {
    std::ifstream arquivo("funcionario.txt");
    if (!arquivo.is_open()) return false;

    funcionarios.clear();
    std::string linha;

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        int usuario_id = std::stoi(linha);

        std::string dataAdmissaoLinha;
        if (!std::getline(arquivo, dataAdmissaoLinha)) return false;
        std::istringstream ssAdm(dataAdmissaoLinha);
        std::vector<std::string> dataAdmissao;
        std::string parte;
        while (ssAdm >> parte) dataAdmissao.push_back(parte);

        std::string cargo;
        if (!std::getline(arquivo, cargo)) return false;

        std::string salarioLinha;
        if (!std::getline(arquivo, salarioLinha)) return false;
        double salario = std::stod(salarioLinha);

        std::string dataDesligamentoLinha;
        if (!std::getline(arquivo, dataDesligamentoLinha)) return false;
        std::vector<std::string> dataDesligamento;
        std::istringstream ssDesl(dataDesligamentoLinha);
        while (ssDesl >> parte) dataDesligamento.push_back(parte);

        Funcionario f(usuario_id, dataAdmissao, cargo, salario, dataDesligamento);
        funcionarios.push_back(f);
    }
    return true;
}

bool salvar_funcionarios() {
    std::ofstream arquivo("funcionario.txt");
    if (!arquivo.is_open()) return false;

    for (const auto& f : funcionarios) {
        arquivo << f.getUsuarioId() << "\n";

        for (const auto& parte : f.getDataAdmissao()) arquivo << parte << " ";
        arquivo << "\n";

        arquivo << f.getCargo() << "\n";
        arquivo << f.getSalario() << "\n";

        for (const auto& parte : f.getDataDesligamento()) arquivo << parte << " ";
        arquivo << "\n";
    }
    return true;
}

void inicializar_funcionarios() {
    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    if (carregar_funcionarios()) {
        std::cout << "Funcionários carregados do arquivo.\n";
    } else {
        std::cout << "Nenhum funcionário encontrado no arquivo.\n";
    }
}

void criar_funcionario(const web::http::http_request& request) {
    request.extract_json().then([=](const web::json::value& json) {
        if (!json.has_field(U("usuario_id")) ||
            !json.has_field(U("data_admissao")) ||
            !json.has_field(U("cargo")) ||
            !json.has_field(U("salario"))) {
            request.reply(web::http::status_codes::BadRequest, U("Campos obrigatórios faltando"));
            return;
        }

        int usuario_id = json.at(U("usuario_id")).as_integer();

        std::lock_guard<std::mutex> lock(funcionarios_mutex);

        auto usuario_it = std::find_if(usuarios.begin(), usuarios.end(),
            [usuario_id](const Usuario& u) { return u.getId() == usuario_id; });

        if (usuario_it == usuarios.end()) {
            request.reply(web::http::status_codes::NotFound, U("Usuário não encontrado para associar ao funcionário"));
            return;
        }

        std::vector<std::string> dataAdmissao;
        for (const auto& v : json.at(U("data_admissao")).as_array())
            dataAdmissao.push_back(utility::conversions::to_utf8string(v.as_string()));

        std::vector<std::string> dataDesligamento;
        if (json.has_field(U("data_desligamento"))) {
            for (const auto& v : json.at(U("data_desligamento")).as_array())
                dataDesligamento.push_back(utility::conversions::to_utf8string(v.as_string()));
        }

        Funcionario novo_funcionario(usuario_id,
            dataAdmissao,
            utility::conversions::to_utf8string(json.at(U("cargo")).as_string()),
            json.at(U("salario")).as_double(),
            dataDesligamento);

        funcionarios.push_back(novo_funcionario);
        salvar_funcionarios();

        request.reply(web::http::status_codes::Created, U("Funcionário cadastrado com sucesso"));
    }).wait();
}

void listar_funcionarios(const web::http::http_request& request) {
    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    web::json::value resposta = web::json::value::array();
    int idx = 0;
    for (const auto& f : funcionarios) {
        resposta[idx++] = web::json::value::object({
            { U("usuario_id"), web::json::value::number(f.getUsuarioId()) },
            { U("cargo"), web::json::value::string(utility::conversions::to_string_t(f.getCargo())) },
            { U("salario"), web::json::value::number(f.getSalario()) }
        });
    }
    request.reply(web::http::status_codes::OK, resposta);
}

void atualizar_funcionario(const web::http::http_request& request, int usuario_id) {
    request.extract_json().then([=](const web::json::value& json) {
        std::lock_guard<std::mutex> lock(funcionarios_mutex);
        auto it = std::find_if(funcionarios.begin(), funcionarios.end(),
            [usuario_id](const Funcionario& f) { return f.getUsuarioId() == usuario_id; });

        if (it != funcionarios.end()) {
            if (json.has_field(U("cargo"))) it->setCargo(utility::conversions::to_utf8string(json.at(U("cargo")).as_string()));
            if (json.has_field(U("salario"))) it->setSalario(json.at(U("salario")).as_double());
            if (json.has_field(U("data_desligamento"))) {
                std::vector<std::string> data;
                for (const auto& v : json.at(U("data_desligamento")).as_array())
                    data.push_back(utility::conversions::to_utf8string(v.as_string()));
                it->setDataDesligamento(data);
            }

            salvar_funcionarios();
            request.reply(web::http::status_codes::OK, U("Funcionário atualizado com sucesso"));
        } else {
            request.reply(web::http::status_codes::NotFound, U("Funcionário não encontrado"));
        }
    }).wait();
}

void deletar_funcionario(const web::http::http_request& request, int usuario_id) {
    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    auto it = std::remove_if(funcionarios.begin(), funcionarios.end(),
        [usuario_id](const Funcionario& f) { return f.getUsuarioId() == usuario_id; });

    if (it != funcionarios.end()) {
        funcionarios.erase(it, funcionarios.end());
        salvar_funcionarios();
        request.reply(web::http::status_codes::OK, U("Funcionário deletado com sucesso"));
    } else {
        request.reply(web::http::status_codes::NotFound, U("Funcionário não encontrado"));
    }
}
