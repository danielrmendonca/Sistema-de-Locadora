#include "Filme.h"


// Implementação de como funciona o a criação de um objeto do tipo Filme, atribuindo os valores ao construir o objeto. Também tem a implementação dos getters e setters

Filme::Filme(int id, bool dublado, bool legendado, const std::string& lancamento,
             const std::string& classificacao_indicativa, const std::string& diretor,
             const std::string& titulo, double duracao, double nota)
    : id(id), dublado(dublado), legendado(legendado), lancamento(lancamento),
      classificacao_indicativa(classificacao_indicativa), diretor(diretor),
      titulo(titulo), duracao(duracao), nota(nota) {}

int Filme::getId() const { return id; }
bool Filme::isDublado() const { return dublado; }
bool Filme::isLegendado() const { return legendado; }
std::string Filme::getLancamento() const { return lancamento; }
std::string Filme::getClassificacaoIndicativa() const { return classificacao_indicativa; }
std::string Filme::getDiretor() const { return diretor; }
std::string Filme::getTitulo() const { return titulo; }
double Filme::getDuracao() const { return duracao; }
double Filme::getNota() const { return nota; }

void Filme::setDublado(bool d) { dublado = d; }
void Filme::setLegendado(bool l) { legendado = l; }
void Filme::setLancamento(const std::string& l) { lancamento = l; }
void Filme::setClassificacaoIndicativa(const std::string& c) { classificacao_indicativa = c; }
void Filme::setDiretor(const std::string& d) { diretor = d; }
void Filme::setTitulo(const std::string& t) { titulo = t; }
void Filme::setDuracao(double d) { duracao = d; }
void Filme::setNota(double n) { nota = n; }
