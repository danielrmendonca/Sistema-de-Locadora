#include "FuncionarioHandlers.h"
#include <mutex>
#include <fstream>
#include <sstream>
#include <cpprest/asyncrt_utils.h>
#include "handlers/UsuarioHandlers.h" // Para acessar usuarios

using namespace web;
using namespace http;
using namespace http::experimental::listener;
std::vector<Funcionario> funcionarios;
std::mutex funcionarios_mutex;

void inicializar_funcionarios() {
    if (!carregar_funcionarios()) {
        funcionarios.clear();
        next_id = 1;
        salvar_funcionarios();
    }
}
// Função responsável por carregar nosso arquivo que contem os dados de funcionario
bool carregar_funcionarios() {
    std::ifstream arquivo("funcionario.txt");
    if (!arquivo.is_open()) return false;

    std::string conteudo((std::istreambuf_iterator<char>(arquivo)),
                         std::istreambuf_iterator<char>());

    json::value jsonArray;
    try {
        jsonArray = json::value::parse(utility::conversions::to_string_t(conteudo));
    } catch (...) {
        return false;
    }

    if (!jsonArray.is_array()) return false;

    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    funcionarios.clear();

    for (const auto& item : jsonArray.as_array()) {
        if (!item.has_field(U("usuario_id")) ||
            !item.has_field(U("data_admissao")) ||
            !item.has_field(U("cargo")) ||
            !item.has_field(U("salario")) ||
            !item.has_field(U("data_desligamento"))) {
            continue;
        }

        int usuario_id = item.at(U("usuario_id")).as_integer();

        std::vector<std::string> dataAdmissao;
        for (const auto& parte : item.at(U("data_admissao")).as_array()) {
            dataAdmissao.push_back(utility::conversions::to_utf8string(parte.as_string()));
        }

        std::string cargo = utility::conversions::to_utf8string(item.at(U("cargo")).as_string());
        double salario = item.at(U("salario")).as_double();

        std::vector<std::string> dataDesligamento;
        for (const auto& parte : item.at(U("data_desligamento")).as_array()) {
            dataDesligamento.push_back(utility::conversions::to_utf8string(parte.as_string()));
        }

        Funcionario f(usuario_id, dataAdmissao, cargo, salario, dataDesligamento);
        funcionarios.push_back(f);
    }

    return true;
}

bool salvar_funcionarios() {
    json::value jsonArray = json::value::array();

    for (size_t i = 0; i < funcionarios.size(); i++) {
        const auto& f = funcionarios[i];
        json::value item;

        item[U("usuario_id")] = json::value::number(f.getUsuarioId());

        json::value dataAdmissaoArray = json::value::array();
        for (size_t j = 0; j < f.getDataAdmissao().size(); j++) {
            dataAdmissaoArray[j] = json::value::string(utility::conversions::to_string_t(f.getDataAdmissao()[j]));
        }
        item[U("data_admissao")] = dataAdmissaoArray;

        item[U("cargo")] = json::value::string(utility::conversions::to_string_t(f.getCargo()));
        item[U("salario")] = json::value::number(f.getSalario());

        json::value dataDesligamentoArray = json::value::array();
        for (size_t j = 0; j < f.getDataDesligamento().size(); j++) {
            dataDesligamentoArray[j] = json::value::string(utility::conversions::to_string_t(f.getDataDesligamento()[j]));
        }
        item[U("data_desligamento")] = dataDesligamentoArray;

        jsonArray[i] = item;
    }

    std::ofstream arquivo("funcionario.txt");
    if (!arquivo.is_open()) return false;

    std::string jsonStr = utility::conversions::to_utf8string(jsonArray.serialize());
    arquivo << jsonStr;

    return true;
}

void listar_funcionarios(const http_request& request) {
    json::value jsonArray = json::value::array();

    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    for (size_t i = 0; i < funcionarios.size(); ++i) {
        const auto& f = funcionarios[i];
        json::value item;

        item[U("usuario_id")] = json::value::number(f.getUsuarioId());
        item[U("cargo")] = json::value::string(utility::conversions::to_string_t(f.getCargo()));
        item[U("salario")] = json::value::number(f.getSalario());

        json::value dataAdmissaoArray = json::value::array();
        for (size_t j = 0; j < f.getDataAdmissao().size(); j++) {
            dataAdmissaoArray[j] = json::value::string(utility::conversions::to_string_t(f.getDataAdmissao()[j]));
        }
        item[U("data_admissao")] = dataAdmissaoArray;

        json::value dataDesligamentoArray = json::value::array();
        for (size_t j = 0; j < f.getDataDesligamento().size(); j++) {
            dataDesligamentoArray[j] = json::value::string(utility::conversions::to_string_t(f.getDataDesligamento()[j]));
        }
        item[U("data_desligamento")] = dataDesligamentoArray;

        jsonArray[i] = item;
    }

    request.reply(status_codes::OK, jsonArray);
}

void criar_funcionario(const http_request& request) {
    request.extract_json().then([=](const json::value& json) {
        if (!json.has_field(U("usuario_id")) ||
            !json.has_field(U("data_admissao")) ||
            !json.has_field(U("cargo")) ||
            !json.has_field(U("salario"))) {
            request.reply(status_codes::BadRequest, U("Campos obrigatórios faltando"));
            return;
        }

        int usuario_id = json.at(U("usuario_id")).as_integer();

        std::lock_guard<std::mutex> lock(funcionarios_mutex);

        auto usuario_it = std::find_if(usuarios.begin(), usuarios.end(),
            [usuario_id](const Usuario& u) { return u.getId() == usuario_id; });

        if (usuario_it == usuarios.end()) {
            request.reply(status_codes::NotFound, U("Usuário não encontrado para associar ao funcionário"));
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

        std::string cargo = utility::conversions::to_utf8string(json.at(U("cargo")).as_string());
        double salario = json.at(U("salario")).as_double();

        Funcionario novo_funcionario(usuario_id, dataAdmissao, cargo, salario, dataDesligamento);
        funcionarios.push_back(novo_funcionario);
        salvar_funcionarios();

        request.reply(status_codes::Created, U("Funcionário cadastrado com sucesso"));
    }).wait();
}


void atualizar_funcionario(const http_request& request, int usuario_id) {
    request.extract_json().then([usuario_id, &request](const json::value& json) {
        std::lock_guard<std::mutex> lock(funcionarios_mutex);
        auto it = std::find_if(funcionarios.begin(), funcionarios.end(),
            [usuario_id](const Funcionario& f) { return f.getUsuarioId() == usuario_id; });

        if (it == funcionarios.end()) {
            request.reply(status_codes::NotFound, U("Funcionário não encontrado"));
            return;
        }

        if (json.has_field(U("cargo")))
            it->setCargo(utility::conversions::to_utf8string(json.at(U("cargo")).as_string()));
        if (json.has_field(U("salario")))
            it->setSalario(json.at(U("salario")).as_double());

        if (json.has_field(U("data_desligamento"))) {
            std::vector<std::string> dataDesligamento;
            for (const auto& v : json.at(U("data_desligamento")).as_array())
                dataDesligamento.push_back(utility::conversions::to_utf8string(v.as_string()));
            it->setDataDesligamento(dataDesligamento);
        }

        salvar_funcionarios();
        request.reply(status_codes::OK, U("Funcionário atualizado com sucesso"));
    }).wait();
}

void deletar_funcionario(const http_request& request, int usuario_id) {
    std::lock_guard<std::mutex> lock(funcionarios_mutex);
    auto it = std::remove_if(funcionarios.begin(), funcionarios.end(),
                             [usuario_id](const Funcionario& f) { return f.getUsuarioId() == usuario_id; });

    if (it == funcionarios.end()) {
        request.reply(status_codes::NotFound, U("Funcionário não encontrado"));
    } else {
        funcionarios.erase(it, funcionarios.end());
        salvar_funcionarios();
        request.reply(status_codes::OK, U("Funcionário deletado com sucesso"));
    }
}

