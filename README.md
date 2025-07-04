## 💻 Projeto
Projeto da disciplina ***Estrutura de dados orientada a objetos***, da graduação em [Sistemas de Informação (SI)](https://portal.cin.ufpe.br/graduacao/sistemas-de-informacao/) do [Centro de Informática (CIN)](https://portal.cin.ufpe.br/) da [Universidade Federal de Pernambuco (UFPE)](https://www.ufpe.br/), cursada em no letivo 2025.1
<br>

Este projeto apresenta a modelagem de um sistema para uma locadora de filmes, desenvolvido em C++. A solução moderniza a experiência do cliente, permitindo que a escolha e o aluguel dos filmes sejam feitos de forma online. Com isso, o cliente pode simplesmente comparecer à locadora para buscar os títulos que já selecionou, evitando o tempo de pesquisa nas prateleiras. O sistema também aprimora a gestão interna da locadora, automatizando o registro de quem locou cada filme e monitorando os prazos para devolução de maneira eficiente e online.

<br>

## 🙌 Introdução
O "ACInsta em Casa" é uma plataforma digital desenvolvida para otimizar a locação de filmes. Ela centraliza um vasto catálogo de títulos, oferecendo um sistema de busca e filtragem avançado por tema ou gênero. Através da interface, os usuários podem realizar a locação de filmes específicos de maneira totalmente remota, simplificando o processo de escolha e aluguel.

<br>

## 🖊️ Requisitos
O sistema foi projetado para oferecer as seguintes funcionalidades principais: a locação de múltiplos filmes de diversos gêneros e o acompanhamento online do status do aluguel, permitindo ao usuário verificar o tempo restante para a devolução de cada item.

<br>

## 🧬 Elementos do Modelo ER

- **Usuário** - Entidade pai que participa de uma herança sobreposta total
- **Empregado** - Especialização de usuário.
- **Cliente** - Especialização de usário.
- **Empresa** - Especialização de usuário.
- **Mídia** - Filmes cadastrados na plataforma.
- **Genero de Mídia** - Generos dos respectivos filmes cadastrados.
- **Genero** - Genero que estão cadastrados na plataforma (Ex: Terror, Comédia...).
- **Aluguel** - Se está alugado ou não o filme.

<br>

## 👍 Atributos

**Entidade** "Usuário":

- Atributos: E-mail, Nome, Gênero, Telefone (multivalorado), Senha, CPF, Data de Nascimento, Endereço (Composto - Bairro, Rua, CEP, Cidade).

**Entidade** "Empregado":

- Atributos: Posição, Data de Admissão, Salário, Data de Demissão.

**Entidade** "Cliente":

- Atributos: Data de Criação (Conta), Limite de Crédito.

**Entidade** "Empresa":

- Atributos: E-mail, CNPJ, Telefones, Nome, Endereço (Composto - Bairro, Rua, CEP, Cidade), Site.

**Entidade** "Mídia":

- Atributos: ID, Diretor, Título, Tempo, Nota, Data, Classificação Etária, Data de Lançamento, Dublado, Legendado.

**Entidade** "Gênero":

- Atributos: ID, Gênero (ou Nome do Gênero).

**Entidade** "Mídia_Gênero" (Tabela de Ligação):

- Atributos: ID_Mídia, ID_Gênero.

**Entidade** "Aluguel":

- Atributos: ID, ID_Mídia, ID_Usuário, Data de Aluguel, Data de Devolução, Preço.

<br>

## 🥰 Relacionamentos

1. Relacionamento de Especialização/Generalização (Herança)

- **Entidades Envolvidas**: User (Usuário) é a entidade genérica (superclasse), enquanto Employe (Empregado) e Client (Cliente) são as entidades especializadas (subclasses).

- **Tipo**: Herança (ou "ISA", que significa "é um").

- **Descrição**: Um Usuário no sistema pode ser um Empregado ou um Cliente. Tanto Empregado quanto Cliente herdam todos os atributos da entidade User (como Nome, CPF, Endereço) e adicionam seus próprios atributos específicos.

- **Employe** adiciona: Posição, Salário, Data de Admissão, etc.

- **Client adiciona**: Limite de Crédito e Data de Cadastro.

**2.** Relacionamento de Aluguel (Um-para-Muitos)

- **Entidades Envolvidas**: Client (Cliente) e Rented (Aluguel).

- **Tipo**: Um-para-Muitos (1:N).

- **Descrição**: Um Cliente pode realizar vários Aluguéis ao longo do tempo. No entanto, cada registro de Aluguel (Rented) pertence a um único Cliente. Isso é indicado pelo atributo ID User dentro da entidade Rented.

**3.** Relacionamento entre Mídia e Aluguel (Um-para-Muitos)

- **Entidades Envolvidas**: Media (Mídia) e Rented (Aluguel).

- **Tipo**: Um-para-Muitos (1:N).

- **Descrição**: Uma Mídia (um filme específico) pode ser alugada várias vezes. Cada registro na tabela Rented corresponde ao aluguel de uma única Mídia. A conexão é feita pelo atributo ID Film (que representa o ID da Mídia) na entidade Rented.

**4.** Relacionamento entre Mídia e Gênero (Muitos-para-Muitos)

- **Entidades Envolvidas**: Media (Mídia) e Genre (Gênero).

- **Tipo**: Muitos-para-Muitos (N:M).

- **Descrição**: Este é um relacionamento complexo, resolvido pela entidade associativa Media_Genre.

- Uma **Mídia** pode pertencer a vários Gêneros (ex: um filme pode ser "Ação" e "Ficção Científica").

- Um **Gênero** pode conter várias Mídias (ex: o gênero "Ação" tem muitos filmes).

- A entidade **Media_Genre** existe para conectar as duas, guardando pares de Media ID e Gene ID.

**5.** Relacionamento de Trabalho (Um-para-Muitos)

- **Entidades Envolvidas**: Enterprise (Empresa) e User (do tipo Employe).

- **Tipo**: Um-para-Muitos (1:N).

- **Descrição**: Uma Empresa pode ter vários Empregados (que são Usuários). O diagrama sugere que um Empregado trabalha para uma única Empresa.

<br>

## 🏦 Banco de Dados ( Esquema )

![image](https://github.com/user-attachments/assets/81f286ca-fe32-4b33-8c20-450c336b0088)

<br>

# Comandos para rodar no back

- Instalar o vcpkg e o cpprest

- vcpkg install cpprestsdk cpprestsdk:x64-windows

- Na raiz do back rodar:

-  - mkdir build

-  cd build

-  em /build

-  cmake ..

-  cmake --build .
