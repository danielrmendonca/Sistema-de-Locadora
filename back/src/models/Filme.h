#ifndef FILME_H
#define FILME_H

#include <string>

// Definição da clase filme e seus getters e setters

class Filme {
private:
    int id;
    bool dublado;
    bool legendado;
    std::string lancamento;
    std::string classificacao_indicativa;
    std::string diretor;
    std::string titulo;
    double duracao;
    double nota;

public:

    //Duas formas de criar um Filme, Temos 2 construtores, um inicia um filme vazio e o segundo com parametros esperados
    Filme() = default;
    Filme(int id, bool dublado, bool legendado, const std::string& lancamento,
          const std::string& classificacao_indicativa, const std::string& diretor,
          const std::string& titulo, double duracao, double nota);

    // Getters - Deixamos que a implementação dos Getters fiquem a cargo do nosso Feilme.cpp

    int getId() const;
    bool isDublado() const;
    bool isLegendado() const;
    std::string getLancamento() const;
    std::string getClassificacaoIndicativa() const;
    std::string getDiretor() const;
    std::string getTitulo() const;
    double getDuracao() const;
    double getNota() const;

    // Setters - Deixamos que a implementação dos Setters fiquem a cargo do nosso Filme.cpp
    void setDublado(bool dublado);
    void setLegendado(bool legendado);
    void setLancamento(const std::string& lancamento);
    void setClassificacaoIndicativa(const std::string& classificacao);
    void setDiretor(const std::string& diretor);
    void setTitulo(const std::string& titulo);
    void setDuracao(double duracao);
    void setNota(double nota);
};

#endif // FILME_H
