#ifndef FILME_HANDLERS_H
#define FILME_HANDLERS_H

#include <cpprest/http_listener.h>
#include "Filme.h"
#include <vector>
#include <mutex>

extern std::vector<Filme> filmes;
extern int next_filme_id;
extern std::mutex filme_mutex;

// Definimos as funções basicas da classe Filme
void inicializar_filmes();
void criar_filme(const web::http::http_request& request);
void listar_filmes(const web::http::http_request& request);
void atualizar_filme(const web::http::http_request& request, int id);
void deletar_filme(const web::http::http_request& request, int id);

// Definimos as funções de carregamento dos dados de cliente do nosso .txt e salvar no arquivo

bool carregar_filmes();
bool salvar_filmes();

#endif // FILME_HANDLERS_H
