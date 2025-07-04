#include "GeneroFilme.h"

// Implementação de como funciona o a criação de um objeto do tipo GeneroFilme, atribuindo os valores ao construir o objeto. Também tem a implementação dos getters e setters

GeneroFilme::GeneroFilme(int filme_id, int genero_id)
    : filme_id(filme_id), genero_id(genero_id) {}

int GeneroFilme::getFilmeId() const { return filme_id; }
int GeneroFilme::getGeneroId() const { return genero_id; }
