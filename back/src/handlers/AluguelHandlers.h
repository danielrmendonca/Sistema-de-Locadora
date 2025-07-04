#ifndef ALUGUEL_HANDLERS_H
#define ALUGUEL_HANDLERS_H

#include <cpprest/http_listener.h>
#include "Aluguel.h"
#include <vector>
#include <mutex>

extern std::vector<Aluguel> alugueis;
extern int next_aluguel_id;
extern std::mutex aluguel_mutex;

void inicializar_alugueis();
void criar_aluguel(const web::http::http_request& request);
void listar_alugueis(const web::http::http_request& request);
void atualizar_aluguel(const web::http::http_request& request, int id);
void deletar_aluguel(const web::http::http_request& request, int id);

bool carregar_alugueis();
bool salvar_alugueis();

#endif // ALUGUEL_HANDLERS_H
