#include "nba_stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_HASH 41669 // Tamanho da tabela hash

int main() {
  int i = 0;
  int num_players = CarregaPlayers();
  printf("%d dados estao sendo carregados e o menu esta sendo gerado...\n\n", num_players);

  // Inicializar a tabela hash
  HashTable *hashTable = InicializaHash(TAMANHO_HASH);
  Abb *arvore = NULL;

  // Inserir jogadores na tabela hash e na árvore binária de busca
  for (i = 0; i < num_players; i++) {
    InsereHash(hashTable, Players[i].player_id, &Players[i]);
    arvore = Inserir(arvore, Players[i].player_id, &Players[i]);
  }

  int opcao, id;
  PlayerStats *player;
  system("pause");

  while (1) {
    printf("\n MENU:\n");
    printf(" 1. Buscar jogador por ID e comparar o custo das buscas (sequencial e HASH):\n");
    printf(" 2. Apresentar a tabela hash completa\n");
    printf(" 3. Sair da aplicacao\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Digite o ID do jogador: ");
      scanf("%d", &id);

      // Buscar na árvore binária de busca
      Abb *res = AbbBusca(arvore, id);
      if (res) {
        printf("Todas as temporadas do jogador encontrado:\n");
        MostraListaDados(res->ListaDados);
        printf("Esse foram todos os dados encontrados do jogador com id %d.\n",id);
      } else {
        printf("Jogador nao encontrado na arvore binaria de busca.\n");
      }

      // Buscar na tabela hash
      player = BuscaHash(hashTable, id);
      if (player) {
        printf("Sucesso! Jogador encontrado na tabela hash!\n");
        printf("Dados de sua ultima temporada:\n");
        printf("Jogador: %s, Equipe: %s, Pontos marcados: %d\n", player->player_name,player->team, player->points_scored);
            PesquisaSequencial(hashTable, id);
      } else {
        printf("Jogador nao encontrado na tabela hash.\n");
      }

      break;

    case 2:
      MostraHash(hashTable);
      break;

    case 3:
      exit(0);

    default:
      printf("Opcao invalida! Tente novamente.\n");
    }
  }

  return 0;
}
