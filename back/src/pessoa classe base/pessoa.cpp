// Pessoa.cpp
// Defines methods init() and display().

#include "Pessoa.h"
#include <iostream>
using namespace std;

bool Pessoa::init(const string i_nome, const string i_cpf)
{
    if (i_nome.size() < 1 || i_cpf.size() < 11)   // Conferir nome e cpf
        return false;
    
    nome = i_nome;
    cpf  = i_cpf;
    return true;
}

void Pessoa::display()
{
    cout << "<< --------------------------- >>\n"
         << "<< Nome:    " << nome << '\n'
         << "<< CPF:     " << cpf  << '\n'
         << "<< --------------------------- >>\n"
         << endl;
}

//to fazendo de acordo com os slides do professor, a gente vai ajustando
