#ifndef GENERO_FILME_HANDLERS_H
#define GENERO_FILME_HANDLERS_H

#include <cpprest/http_listener.h>
#include "GeneroFilme.h"
#include <vector>
#include <mutex>

extern std::vector<GeneroFilme> genero_filmes;
extern std::mutex genero_filme_mutex;

// Funções básicas de manipulação
void inicializar_genero_filmes();
void criar_genero_filme(const web::http::http_request& request);
void listar_genero_filme(const web::http::http_request& request);
void atualizar_genero_filme(const web::http::http_request& request, int id);
void deletar_genero_filme(const web::http::http_request& request, int filme_id, int genero_id);

// Funções de carregamento e salvamento
bool carregar_genero_filmes();
bool salvar_genero_filmes();

#endif // GENERO_FILME_HANDLERS_H