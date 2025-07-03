#ifndef USUARIO_HANDLERS_H
#define USUARIO_HANDLERS_H

#include <cpprest/http_listener.h>  
#include "models/Usuario.h"
#include <vector>

extern std::vector<Usuario> usuarios;
extern int next_id;

void criar_usuario(const web::http::http_request& request);
void listar_usuarios(const web::http::http_request& request);
void atualizar_usuario(const web::http::http_request& request, int id);
void deletar_usuario(const web::http::http_request& request, int id);

// Funções para persistência dos usuários
bool carregar_usuarios();
bool salvar_usuarios();

#endif
