#ifndef CLIENTE_HANDLERS_H
#define CLIENTE_HANDLERS_H

#include <cpprest/http_listener.h>
#include "models/Cliente.h"
#include "models/Usuario.h" // Para acessar usuarios[]
#include <vector>

extern std::vector<Cliente> clientes;

void inicializar_clientes();
void criar_cliente(const web::http::http_request& request);
void listar_clientes(const web::http::http_request& request);
void atualizar_cliente(const web::http::http_request& request, int usuario_id);
void deletar_cliente(const web::http::http_request& request, int usuario_id);

bool carregar_clientes();
bool salvar_clientes();

#endif
