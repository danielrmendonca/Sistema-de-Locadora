#ifndef FUNCIONARIO_HANDLERS_H
#define FUNCIONARIO_HANDLERS_H

#include <cpprest/http_listener.h>
#include "models/Funcionario.h"
#include "models/Usuario.h" // Para acessar usuarios[]
#include <vector>

extern std::vector<Funcionario> funcionarios;

// Definimos as funções basicas da classe Funcionario

void inicializar_funcionarios();
void criar_funcionario(const web::http::http_request& request);
void listar_funcionarios(const web::http::http_request& request);
void atualizar_funcionario(const web::http::http_request& request, int usuario_id);
void deletar_funcionario(const web::http::http_request& request, int usuario_id);

// Definimos as funções de carregamento dos dados de funcionario do nosso .txt e salvar no arquivo

bool carregar_funcionarios();
bool salvar_funcionarios();

#endif
