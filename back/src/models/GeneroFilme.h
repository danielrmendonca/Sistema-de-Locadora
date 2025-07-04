#ifndef GENERO_FILME_H
#define GENERO_FILME_H

// Definição da clase GeneroFilme e seus getters e setters
class GeneroFilme {
private:
    int filme_id;
    int genero_id;

public:

    //Duas formas de criar um generoFilme, Temos 2 construtores, um inicia um generoFilme vazio e o segundo com parametros esperados
    GeneroFilme() = default;
    GeneroFilme(int filme_id, int genero_id);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso GeneroFilme.cpp
    int getFilmeId() const;
    int getGeneroId() const;
};

#endif // GENERO_FILME_H
