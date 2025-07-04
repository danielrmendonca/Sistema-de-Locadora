#ifndef GENERO_FILME_HANDLERS_H
#define GENERO_FILME_HANDLERS_H

#include <cpprest/http_listener.h>
#include "GeneroFilme.h"
#include <vector>
#include <mutex>

extern std::vector<GeneroFilme> genero_filmes;
extern std::mutex genero_filme_mutex;

// Definimos as funções basicas da classe GeneroFilme
void inicializar_genero_filmes();
void criar_genero_filme(const web::http::http_request& request);
void listar_genero_filmes(const web::http::http_request& request);
void deletar_genero_filme(const web::http::http_request& request);


// Definimos as funções de carregamento dos dados de GereroFilme do nosso .txt e salvar no arquivo

bool carregar_genero_filmes();
bool salvar_genero_filmes();

#endif // GENERO_FILME_HANDLERS_H
