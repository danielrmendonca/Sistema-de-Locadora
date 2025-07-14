#include "GeneroFilme.h"

// Implementação da classe GeneroFilme

GeneroFilme::GeneroFilme(int filme_id, int genero_id)
    : filme_id(filme_id), genero_id(genero_id) {
}

int GeneroFilme::getFilmeId() const { return filme_id; }
int GeneroFilme::getGeneroId() const { return genero_id; }

void GeneroFilme::setFilmeId(int id) {
    filme_id = id;
}

void GeneroFilme::setGeneroId(int id) {
    genero_id = id;
}