#ifndef FUNCIONARIO_HANDLERS_H
#define FUNCIONARIO_HANDLERS_H

#include <cpprest/http_listener.h>
#include "models/Funcionario.h"
#include "models/Usuario.h" // Para acessar usuarios[]
#include <vector>

extern std::vector<Funcionario> funcionarios;

void inicializar_funcionarios();
void criar_funcionario(const web::http::http_request& request);
void listar_funcionarios(const web::http::http_request& request);
void atualizar_funcionario(const web::http::http_request& request, int usuario_id);
void deletar_funcionario(const web::http::http_request& request, int usuario_id);

bool carregar_funcionarios();
bool salvar_funcionarios();

#endif
