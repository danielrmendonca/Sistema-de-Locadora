#ifndef CLIENTE_HANDLERS_H
#define CLIENTE_HANDLERS_H

#include <cpprest/http_listener.h>
#include "models/Cliente.h"
#include "models/Usuario.h" // Para acessar usuarios[]
#include <vector>
#include <mutex>  // <- Você precisa incluir <mutex> para usar std::mutex

extern std::vector<Cliente> clientes;
extern std::mutex cliente_mutex;  // <- Adicione esta linha!

// Definimos as funções basicas da classe Cliente
void inicializar_clientes();
void criar_cliente(const web::http::http_request& request);
void listar_clientes(const web::http::http_request& request);
void atualizar_cliente(const web::http::http_request& request, int usuario_id);
void deletar_cliente(const web::http::http_request& request, int usuario_id);

// Definimos as funções de carregamento dos dados de cliente do nosso .txt e salvar no arquivo
bool carregar_clientes();
bool salvar_clientes();

#endif
