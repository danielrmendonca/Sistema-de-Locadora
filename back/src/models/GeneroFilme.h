#ifndef GENERO_FILME_H
#define GENERO_FILME_H

class GeneroFilme {
private:
    int filme_id;
    int genero_id;

public:
    GeneroFilme() = default;
    GeneroFilme(int filme_id, int genero_id);

    int getFilmeId() const;
    int getGeneroId() const;
};

#endif // GENERO_FILME_H
