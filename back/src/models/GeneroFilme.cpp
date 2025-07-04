#include "GeneroFilme.h"

GeneroFilme::GeneroFilme(int filme_id, int genero_id)
    : filme_id(filme_id), genero_id(genero_id) {}

int GeneroFilme::getFilmeId() const { return filme_id; }
int GeneroFilme::getGeneroId() const { return genero_id; }
