#ifndef GENERO_FILME_H
#define GENERO_FILME_H

#include <string>
#include <vector>

// Definição da classe GeneroFilme e seus getters
class GeneroFilme {
private:
    int filme_id;
    int genero_id;

public:
    // Construtores
    GeneroFilme() = default;
    GeneroFilme(int filme_id, int genero_id);

    // Getters
    int getFilmeId() const;
    int getGeneroId() const;

    // Setters
    void setFilmeId(int id);
    void setGeneroId(int id);
};

#endif // GENERO_FILME_H