#ifndef GENERO_HANDLERS_H
#define GENERO_HANDLERS_H

#include <cpprest/http_listener.h>
#include "Genero.h"
#include <vector>
#include <mutex>

extern std::vector<Genero> generos;
extern int next_genero_id;
extern std::mutex genero_mutex;
// Definimos as funções basicas da classe genero
void inicializar_generos();
void criar_genero(const web::http::http_request& request);
void listar_generos(const web::http::http_request& request);
void atualizar_genero(const web::http::http_request& request, int id);
void deletar_genero(const web::http::http_request& request, int id);

// Definimos as funções de carregamento dos dados de genero do nosso .txt e salvar no arquivo
bool carregar_generos();
bool salvar_generos();

#endif // GENERO_HANDLERS_H
