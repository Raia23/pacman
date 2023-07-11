#include <stdio.h>
/*
    Trabalho de Programacao 1
    Jogo do Pacman
    Aluna: Raissa Almeida
*/

#define MAX_FANTASMAS 10
#define MAX_LINHAS 100
#define MAX_COLUNAS 100
#define PAREDE '#'
#define COMIDA '*'
#define VAZIO ' '
#define DIREITA 'd'
#define ESQUERDA 'a'
#define CIMA 'w'
#define BAIXO 's'
#define FANTASMA_B 'B'
#define FANTASMA_P 'P'
#define FANTASMA_I 'I'
#define FANTASMA_C 'C'
/*
typedef struct ranking{
    char direcao;
    int pontos;
    int colisao;
    int movimentos;
}tRanking;

tRanking RankingCriar(char direcao, int pontos, int colisao, int movimentos);
void RankingImprimir(tRanking ranking);
tRanking RankingAtualizar(tRanking ranking, char direcao, int pontos, int colisao, int movimentos);
void RankingOrdenar(int tam, tRanking ranking[tam]);
void RankingGerarArquivo(int tam, tRanking ranking[tam], char *diretorio);
*/

typedef struct fantasma
{
    int linha;
    int coluna;
    char fantasma;
    char sentido;
} tFantasma;

tFantasma FantasmaCriar(int linha, int coluna, char fantasma);
int EhFantasma(tFantasma fantasma, int linha, int coluna);
int FantasmaLinha(tFantasma fantasma);
int FantasmaColuna(tFantasma fantasma);
char FantasmaSentido(tFantasma fantasma);
tFantasma FantasmaAtualizar(tFantasma fantasma, int linha, int coluna, char sentido);

typedef struct pacMan
{
    int linha;
    int coluna;
    char pacMan;
} tPacMan;

tPacMan PacManCriar(int linha, int coluna);
int EhPacMan(tPacMan pacMan, int linha, int coluna);
int PacManLinha(tPacMan pacMan);
int PacManColuna(tPacMan pacMan);
tPacMan PacManAtualizar(tPacMan pacMan, int linha, int coluna);

typedef struct mapa
{
    int linhas;
    int colunas;
    char matriz[MAX_LINHAS][MAX_COLUNAS];
} tMapa;

tMapa MapaCriar(FILE *arquivo, int linhas, int colunas);
void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma]);
int EhParede(tMapa mapa, int linha, int coluna);
int EhComida(tMapa mapa, int linha, int coluna);
int EhPacManMapa(tMapa mapa, int linha, int coluna);
int EhFantasmaMapa(tMapa mapa, int linha, int coluna);
tPacMan MapaAchaPacMan(tMapa mapa);
int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS]);
tMapa MapaTiraComida(tMapa mapa, int linha, int coluna);
int MapaNumeroLinhas(tMapa mapa);
int MapaNumeroColunas(tMapa mapa);
int MapaNumeroComidas(tMapa mapa);

typedef struct jogo
{
    tMapa mapa;
    tPacMan pacMan;
    tFantasma fantasmas[MAX_FANTASMAS];
    int tamFantasmas;
    int numComidas;
    int limiteMovimentos; // numero maximo de movimentos que o pacman pode fazer
    int movimentos;       // numero de movimentos que o pacman fez
    int pontos;           // numero de pontos que o pacman fez
    int vivo;             // 1 - vivo, 0 - morto
} tJogo;

tJogo JogoCriar(char *diretorio);
void JogoImprimir(tJogo jogo, char movimento);
char JogoLerJogada();
tJogo JogoMoverPacMan(tJogo jogo, char movimento);
tJogo JogoMoverFantasmas(tJogo jogo);
int JogoAcabou(tJogo jogo);
void JogoGerarResumo(tJogo jogo);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Erro: Informe o diretorio do mapa\n");
        return 1;
    }

    tJogo jogo = JogoCriar(argv[1]);

    while (!JogoAcabou(jogo))
    {
        char movimento = JogoLerJogada();
        jogo = JogoMoverPacMan(jogo, movimento);
        jogo = JogoMoverFantasmas(jogo);
        JogoImprimir(jogo, movimento);
        // JogoGerarResumo(jogo);
    }

    return 0;
}

