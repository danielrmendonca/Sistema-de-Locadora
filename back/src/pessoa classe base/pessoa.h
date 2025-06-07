// Pessoa.h
// Defining the class Pessoa.

#ifndef _PESSOA_ 
#define _PESSOA_

#include <iostream>
#include <string>
using namespace std;

class Pessoa
{
private:
    string nome;
    string cpf;

public:
    bool init(const string i_nome, const string i_cpf);
    void display();
};

#endif // _PESSOA_
