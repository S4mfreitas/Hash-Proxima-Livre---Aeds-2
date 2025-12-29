# Trabalho Prático: Indexação e Pesquisa de Dados (NBA Stats)
Este repositório contém a implementação do trabalho prático da disciplina de Algoritmos e Estruturas de Dados II do curso de Sistemas de Informação da Universidade Estadual de Montes Claros (UNIMONTES).

📋 Sobre o Projeto

O objetivo deste trabalho é desenvolver um programa que utilize conhecimentos avançados de estruturas de dados para processar, armazenar e buscar informações de grandes conjuntos de dados.

O sistema carrega dados de um arquivo de texto, organiza-os em memória utilizando registros (structs) e cria uma estrutura de indexação dinâmica para otimizar as consultas. Além da busca indexada, o software realiza comparativos de desempenho com a pesquisa sequencial.

⚙️ Especificações Técnicas

Conforme a definição para a Equipe 6, o projeto utiliza as seguintes tecnologias e estruturas:


- Estrutura de Indexação: Tabela Hash com tratamento de colisão "Próxima Livre" (Endereçamento Aberto / Tentativa Linear).


- Dataset (Base de Dados): NBA Stats from the last 71 Years.


- Interface: Modo texto (Console).

🚀 Funcionalidades

O programa executa as seguintes tarefas principais:


- Carregamento de Dados: Leitura do arquivo de dataset e organização dos dados em memória via structs.


- Indexação: Criação de uma Tabela Hash (Próxima Livre) para indexar os registros carregados.


- Análise de Desempenho: Comparação entre a busca utilizando a Tabela Hash e a Pesquisa Sequencial, contabilizando a quantidade de acessos à memória e/ou tempo de execução.


- Interação: Interface via terminal para operação do sistema.

👨‍💻 Autores (Equipe 6)
Luís Felipe Silva Barbosa

Samuel Freitas de Oliveira


Professor: Heveraldo Rodrigues de Oliveira Semestre: 1º/2024
