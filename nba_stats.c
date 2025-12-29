#include "nba_stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TotalPlayers = 0; // Variável para armazenar o total de jogadores carregados

// Função para calcular o hash de uma chave
int Hash(int Chave, int tamanho) { return Chave % tamanho; }

// Função para inicializar a tabela hash
HashTable *InicializaHash(int tamanho) {
  HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
  hashTable->tamanho = tamanho;
  hashTable->elementos = (Elemento *)malloc(tamanho * sizeof(Elemento));
  for (int i = 0; i < tamanho; i++) {
    hashTable->elementos[i].chave =
        -1; // Chave -1 indica posição vazia na tabela
    hashTable->elementos[i].dados = NULL;
  }
  return hashTable;
}

// Função para inserir um jogador na tabela hash
void InsereHash(HashTable *hashTable, int chave, PlayerStats *p) {
  int posicao = Hash(chave, hashTable->tamanho);
  while (hashTable->elementos[posicao].chave != -1) {
    posicao =
        (posicao + 1) % hashTable->tamanho; // Próxima posição livre na tabela
  }
  hashTable->elementos[posicao].chave = chave;
  hashTable->elementos[posicao].dados = p;
}

// Função para buscar dados na tabela hash
PlayerStats *BuscaHash(HashTable *hashTable, int chave) {
  int posicao = Hash(chave, hashTable->tamanho);
  int k = 1;
  while (hashTable->elementos[posicao].chave != chave &&
         hashTable->elementos[posicao].chave != -1) {
    posicao = (posicao + 1) % hashTable->tamanho;// Próxima posição na tabela
  }
  if (hashTable->elementos[posicao].chave == chave) {
    printf("\nQuantidade de acessos para a busca usando a tabela HASH: %d\n", k );
    return hashTable->elementos[posicao].dados;
  } else {
    return NULL; // Não encontrou
  }
}

// Função para mostrar a tabela hash
void MostraHash(HashTable *hashTable) {
  for (int i = 0; i < hashTable->tamanho; i++) {
    printf("\nPosicao %d: ", i);
    if (hashTable->elementos[i].chave != -1) {
      PlayerStats *p = hashTable->elementos[i].dados;
      printf("Rank: %d, Year: %s, Season Start Year: %d, Season Type: %s, "
             "Player ID: %d\n",
             p->rank, p->year, p->season_start_year, p->season_type,
             p->player_id);
      printf("Player Name: %s, Team ID: %d, Team: %s\n", p->player_name,
             p->team_id, p->team);
      printf("Games Played: %d, Minutes Played: %d\n", p->games_played,
             p->minutes_played);
      printf("FG Made: %d, FG Attempts: %d, FG %%: %.3f\n", p->fg_made,
             p->fg_attempts, p->fg_percentage);
      printf("3-Pt FG Made: %d, 3-Pt FG Attempts: %d, 3-Pt FG %%: %.3f\n",
             p->three_pt_fg_made, p->three_pt_fg_attempts,
             p->three_pt_fg_percentage);
      printf("FT Made: %d, FT Attempts: %d, FT %%: %.3f\n", p->ft_made,
             p->ft_attempts, p->ft_percentage);
      printf("Offensive Rebounds: %d, Defensive Rebounds: %d, Rebounds: %d\n",
             p->offensive_rebounds, p->defensive_rebounds, p->rebounds);
      printf("Assists: %d, Steals: %d, Blocks: %d\n", p->assists, p->steals,
             p->blocks);
      printf("Turnovers: %d, Personal Fouls: %d, Points Scored: %d\n",
             p->turnovers, p->personal_fouls, p->points_scored);
      printf("Efficiency: %d, AST/TOV: %.3f, STL/TOV: %.3f\n", p->efficiency,
             p->ast_tov, p->stl_tov);
    } else {
      printf("Vazio\n");
    }
  }
}

// Função para inserir um jogador na lista
Lista *ListaInserir(Lista *l, PlayerStats *p) {
  Lista *Novo = (Lista *)malloc(sizeof(Lista));
  Novo->Dados = p;
  Novo->prox = l;
  return Novo;
}

// Função para inserir um jogador na árvore binária de busca
Abb *Inserir(Abb *a, int Chave, PlayerStats *p) {
  if (a == NULL) {
    Abb *Novo = (Abb *)malloc(sizeof(Abb));
    Novo->Chave = Chave;
    Novo->esq = Novo->dir = NULL;
    Novo->ListaDados = NULL;
    Novo->ListaDados = ListaInserir(Novo->ListaDados, p);
    return Novo;
  }
  if (Chave < a->Chave)
    a->esq = Inserir(a->esq, Chave, p);
  else if (Chave > a->Chave)
    a->dir = Inserir(a->dir, Chave, p);
  else {
    a->ListaDados = ListaInserir(
        a->ListaDados, p); // ao achar um duplicado ele manda inserir na lista.
  }
  return a;
}

void remove_quotes(char *field) {
  char *src = field, *dst = field;
  while (*src) {
    if (*src != '\"') {
      *dst++ = *src;
    }
    src++;
  }
  *dst = '\0';
}

char *get_csv_field(char *line, int *pos) {
  int start = *pos;
  int length = 0;
  int in_quotes = 0;

  if (line[*pos] == '"') {
    in_quotes = 1;
    (*pos)++;
    start++;
  }

  while (line[*pos] != '\0' && (line[*pos] != ',' || in_quotes)) {
    if (line[*pos] == '"') {
      if (line[*pos + 1] == '"') {
        (*pos)++;
      } else {
        in_quotes = !in_quotes;
      }
    }
    (*pos)++;
    length++;
  }

  if (line[*pos] == ',') {
    (*pos)++;
  }

  char *field = (char *)malloc((length + 1) * sizeof(char));
  if (field == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(EXIT_FAILURE);
  }
  strncpy(field, &line[start], length);
  field[length] = '\0';

  remove_quotes(field);

  return field;
}

int CarregaPlayers() {
  char Linha[10000];
  int cont = 0;

  FILE *fp = fopen("nba.csv", "r");
  if (fp == NULL) {
    printf("Nao foi possivel abrir o arquivo nba.csv!\n");
    exit(1);
  }

  if (fgets(Linha, sizeof(Linha), fp) == NULL)
    return 0; // Lê o cabeçalho

  while (fgets(Linha, sizeof(Linha), fp) != NULL && cont < MAX_PLAYERS) {
    int pos = 0;
    char *field;

    field = get_csv_field(Linha, &pos);
    Players[cont].rank = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    strcpy(Players[cont].year, field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].season_start_year = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    strcpy(Players[cont].season_type, field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].player_id = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    strcpy(Players[cont].player_name, field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].team_id = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    strcpy(Players[cont].team, field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].games_played = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].minutes_played = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].fg_made = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].fg_attempts = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].fg_percentage = atof(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].three_pt_fg_made = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].three_pt_fg_attempts = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].three_pt_fg_percentage = field[0] ? atof(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].ft_made = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].ft_attempts = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].ft_percentage = atof(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].offensive_rebounds = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].defensive_rebounds = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].rebounds = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].assists = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].steals = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].blocks = field[0] ? atoi(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].turnovers = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].personal_fouls = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].points_scored = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].efficiency = atoi(field);
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].ast_tov = field[0] ? atof(field) : 0;
    free(field);

    field = get_csv_field(Linha, &pos);
    Players[cont].stl_tov = field[0] ? atof(field) : 0;
    free(field);

    cont++;
  }

  fclose(fp);
  return cont;
}

// Função para mostrar os dados da lista
void MostraListaDados(Lista *l) {
  Lista *aux;
  for (aux = l; aux != NULL; aux = aux->prox) {
    PlayerStats *p = aux->Dados;
    printf("Jogador: %s, Equipe: %s\n", p->player_name, p->team);
    printf("Rank: %d, Year: %s, Season Start Year: %d, Season Type: %s, Player "
           "ID: %d\n",
           p->rank, p->year, p->season_start_year, p->season_type,
           p->player_id);
    printf("Games Played: %d, Minutes Played: %d\n", p->games_played,
           p->minutes_played);
    printf("FG Made: %d, FG Attempts: %d, FG %%: %.3f\n", p->fg_made,
           p->fg_attempts, p->fg_percentage);
    printf("3-Pt FG Made: %d, 3-Pt FG Attempts: %d, 3-Pt FG %%: %.3f\n",
           p->three_pt_fg_made, p->three_pt_fg_attempts,
           p->three_pt_fg_percentage);
    printf("FT Made: %d, FT Attempts: %d, FT %%: %.3f\n", p->ft_made,
           p->ft_attempts, p->ft_percentage);
    printf("Offensive Rebounds: %d, Defensive Rebounds: %d, Rebounds: %d\n",
           p->offensive_rebounds, p->defensive_rebounds, p->rebounds);
    printf("Assists: %d, Steals: %d, Blocks: %d\n", p->assists, p->steals,
           p->blocks);
    printf("Turnovers: %d, Personal Fouls: %d, Points Scored: %d\n",
           p->turnovers, p->personal_fouls, p->points_scored);
    printf("Efficiency: %d, AST/TOV: %.3f, STL/TOV: %.3f\n\n", p->efficiency,
           p->ast_tov, p->stl_tov);
  }
}

// Função para buscar na árvore binária de busca
Abb *AbbBusca(Abb *a, int Chave) {
  if (a == NULL)
    return NULL;
  if (Chave < a->Chave)
    return AbbBusca(a->esq, Chave);
  if (Chave > a->Chave)
    return AbbBusca(a->dir, Chave);
  return a; // encontrou
}

// Função para substituir caracteres em uma string
char *replace(char s[40], char c1, char c2) {
  int i;
  for (i = 0; i < strlen(s); i++)
    if (s[i] == c1)
      s[i] = c2;
  return s;
}

void PesquisaSequencial(HashTable *hashTable, int chave) {
    int acessos = 0;
    int encontrado = 0;

    for (int i = 0; i < hashTable->tamanho; i++) {
        acessos++; // Conta cada acesso realizado

        if (hashTable->elementos[i].chave == chave) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado) {
        printf("Quantidade de acessos para a busca do ID %d usando a BUSCA SEQUENCIAL: %d\n", chave, acessos);
    } else {
        printf("Jogador com ID %d nao encontrado.\n", chave);
    }
}
