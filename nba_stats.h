#ifndef NBA_STATS_H_INCLUDED
#define NBA_STATS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 41669 // Tamanho da tabela hash
#define MAX_PLAYERS 33331

typedef struct {
  int player_id;
  char player_name[50];
  int rank;
  char year[20];
  int season_start_year;
  char season_type[20];
  int team_id;
  char team[50];
  int games_played;
  int minutes_played;
  int fg_made;
  int fg_attempts;
  float fg_percentage;
  int three_pt_fg_made;
  int three_pt_fg_attempts;
  float three_pt_fg_percentage;
  int ft_made;
  int ft_attempts;
  float ft_percentage;
  int offensive_rebounds;
  int defensive_rebounds;
  int rebounds;
  int assists;
  int steals;
  int blocks;
  int turnovers;
  int personal_fouls;
  int points_scored;
  int efficiency;
  float ast_tov;
  float stl_tov;
} PlayerStats;

PlayerStats
    Players[MAX_PLAYERS]; // Vetor para guardar os dados carregados do arquivo

// Definição da estrutura Lista para uso na árvore binária de busca
typedef struct lista {
  PlayerStats *Dados;
  struct lista *prox;
} Lista;

// Definição da estrutura Abb (árvore binária de busca)
typedef struct Abb {
  int Chave;
  struct Abb *esq;
  struct Abb *dir;
  Lista *ListaDados; // Lista encadeada para dados com chaves duplicadas
} Abb;

// Definição da estrutura HashTable para tabela hash
typedef struct {
  int chave;
  PlayerStats *dados;
} Elemento;

typedef struct {
  Elemento *elementos;
  int tamanho;
} HashTable;

// Declarações de funções
int Hash(int Chave, int tamanho);
HashTable *InicializaHash(int tamanho);
void InsereHash(HashTable *hashTable, int chave, PlayerStats *p);
PlayerStats *BuscaHash(HashTable *hashTable, int chave);
void MostraHash(HashTable *hashTable);
Lista *ListaInserir(Lista *l, PlayerStats *p);
Abb *Inserir(Abb *a, int Chave, PlayerStats *p);
char *replace(char s[40], char c1, char c2);
int CarregaPlayers();
void MostraListaDados(Lista *l);
Abb *AbbBusca(Abb *a, int Chave);
char *get_csv_field(char *line, int *pos);
void remove_quotes(char *field);
void PesquisaSequencial(HashTable *hashTable, int id);

#endif // NBA_STATS_H_INCLUDED
