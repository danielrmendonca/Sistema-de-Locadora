## 💻 Projeto
Projeto da disciplina ***Estrutura de dados orientada a objetos***, da graduação em [Sistemas de Informação (SI)](https://portal.cin.ufpe.br/graduacao/sistemas-de-informacao/) do [Centro de Informática (CIN)](https://portal.cin.ufpe.br/) da [Universidade Federal de Pernambuco (UFPE)](https://www.ufpe.br/), cursada em no letivo 2025.1
<br>

Este projeto apresenta a modelagem de um sistema para uma locadora de filmes, desenvolvido em C++. A solução moderniza a experiência do cliente, permitindo que a escolha e o aluguel dos filmes sejam feitos de forma online. Com isso, o cliente pode simplesmente comparecer à locadora para buscar os títulos que já selecionou, evitando o tempo de pesquisa nas prateleiras. O sistema também aprimora a gestão interna da locadora, automatizando o registro de quem locou cada filme e monitorando os prazos para devolução de maneira eficiente e online.

<br>

## Introdução
O "ACInsta em Casa" é uma plataforma digital desenvolvida para otimizar a locação de filmes. Ela centraliza um vasto catálogo de títulos, oferecendo um sistema de busca e filtragem avançado por tema ou gênero. Através da interface, os usuários podem realizar a locação de filmes específicos de maneira totalmente remota, simplificando o processo de escolha e aluguel.

<br>

## 🖊️ Requisitos
O sistema foi projetado para oferecer as seguintes funcionalidades principais: a locação de múltiplos filmes de diversos gêneros e o acompanhamento online do status do aluguel, permitindo ao usuário verificar o tempo restante para a devolução de cada item.

## 🧬 Elementos do Modelo ER
- **Pessoa** - Entidade pai que participa de uma herança sobreposta total


# Sistema-de-Locadora: Locadora ACInsta em Casa
Sistema de gerenciamento de uma locadora de filmes em C++

Esquema de Banco de Dados:


![image](https://github.com/user-attachments/assets/81f286ca-fe32-4b33-8c20-450c336b0088)




# Comandos para rodar no back

Instalar o vcpkg e o cpprest

vcpkg install cpprestsdk cpprestsdk:x64-windows

Na raiz do back rodar:

 mkdir build
 cd build

 em /build
 cmake ..

 cmake --build .
