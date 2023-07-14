#include <stdio.h>
#include <stdlib.h>
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
#define PACMAN '>'
#define W 0
#define A 1
#define S 2
#define D 3

typedef struct ranking
{
    char direcao;
    int pontos;
    int colisao;
    int movimentos;
} tRanking;

tRanking RankingCriar(int i);
tRanking RankingAtualizar(tRanking ranking, char direcao, int pontos, int colisao, int movimentos);
void RankingGerarArquivo(int tamRanking, tRanking ranking[tamRanking], FILE *arqRanking);

typedef struct fantasma
{
    int linha;
    int coluna;
    char fantasma;
    char sentido;
} tFantasma;

tFantasma FantasmaCriar(int linha, int coluna, char fantasma);
int EhFantasma(tFantasma fantasma, int linha, int coluna);
int EhFantasmaLista(int tamFantasmas, tFantasma fantasma[tamFantasmas], int linha, int coluna);
int FantasmaLinha(tFantasma fantasma);
int FantasmaColuna(tFantasma fantasma);
char FantasmaSentido(tFantasma fantasma);
char FantasmaCaractere(tFantasma fantasma);
tFantasma FantasmaMover(tFantasma fantasma, char sentido);

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
tPacMan PacManMover(tPacMan pacMan, char sentido);

typedef struct mapa
{
    int linhas;
    int colunas;
    char matriz[MAX_LINHAS][MAX_COLUNAS];
} tMapa;

int EhFantasmaMapa(tMapa mapa, int linha, int coluna);
int EhPacManMapa(tMapa mapa, int linha, int coluna);
tMapa MapaCriar(FILE *arquivo, int linhas, int colunas);
void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma]);
int EhParede(tMapa mapa, int linha, int coluna);
int EhComida(tMapa mapa, int linha, int coluna);
tPacMan MapaAchaPacMan(tMapa mapa);
tMapa MapaRemove(tMapa mapa, int linha, int coluna);
int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS]);
int MapaNumeroLinhas(tMapa mapa);
int MapaNumeroColunas(tMapa mapa);
int MapaNumeroComidas(tMapa mapa);
void MapaGerarInicializacao(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], FILE *arqInicializacao);

typedef struct trilha
{
    int linha;
    int coluna;
    int matrix[MAX_LINHAS][MAX_COLUNAS];
} tTrilha;

tTrilha TrilhaCriar(int linhas, int colunas);
tTrilha TrilhaAtualizar(tTrilha trilha, int linha, int coluna, int movimento);
void TrilhaGerarArquivo(tTrilha trilha, FILE *arqTrilha);

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
    tRanking ranking[4];
    char jogada;
    int colidiu;
} tJogo;

tJogo JogoCriar(char *diretorio);
void JogoImprimir(tJogo jogo, char movimento);
char JogoLerJogada();
tJogo JogoMoverPacMan(tJogo jogo, char movimento);
tJogo JogoMoverFantasmas(tJogo jogo);
int JogoAcabou(tJogo jogo);
tJogo JogoVerificaColisao(tJogo jogo);
tJogo JogoAtualizarPontos(tJogo jogo);
void JogoFinalizar(tJogo jogo);
void JogoGerarResumo(tJogo jogo, FILE *arqResumo, char movimento);
void JogoGerarInicializacao(tJogo jogo, FILE *arqInicializacao);
void JogoGerarEstatistica(tJogo jogo, FILE *arqEstatistica);
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Erro: Informe o diretorio do mapa\n");
        return 1;
    }

    tJogo jogo = JogoCriar(argv[1]);

    char nomeArqResumo[1000];
    sprintf(nomeArqResumo, "%s/saida/resumo.txt", argv[1]);
    FILE *arqResumo = fopen(nomeArqResumo, "w");
    if (arqResumo == NULL)
    {
        printf("Erro: Nao foi possivel criar o arquivo de resumo\n");
        return 1;
    }

    char nomeArqInicializacao[1000];
    sprintf(nomeArqInicializacao, "%s/saida/inicializacao.txt", argv[1]);
    FILE *arqInicializacao = fopen(nomeArqInicializacao, "w");
    if (arqInicializacao == NULL)
    {
        printf("Erro: Nao foi possivel criar o arquivo de inicializacao\n");
        return 1;
    }

    JogoGerarInicializacao(jogo, arqInicializacao);
    fclose(arqInicializacao);

    char nomeArqRanking[1000];
    sprintf(nomeArqRanking, "%s/saida/ranking.txt", argv[1]);
    FILE *arqRanking = fopen(nomeArqRanking, "w");
    if (arqRanking == NULL)
    {
        printf("Erro: Nao foi possivel criar o arquivo de ranking\n");
        return 1;
    }

    char nomeArqEstatistica[1000];
    sprintf(nomeArqEstatistica, "%s/saida/estatistica.txt", argv[1]);
    FILE *arqEstatistica = fopen(nomeArqEstatistica, "w");
    if (arqEstatistica == NULL)
    {
        printf("Erro: Nao foi possivel criar o arquivo de estatistica\n");
        return 1;
    }

    char nomeArqTrilha[1000];
    sprintf(nomeArqTrilha, "%s/saida/trilha.txt", argv[1]);
    FILE *arqTrilha = fopen(nomeArqTrilha, "w");
    if (arqTrilha == NULL)
    {
        printf("Erro: Nao foi possivel criar o arquivo de trilha\n");
        return 1;
    }

    tTrilha trilha = TrilhaCriar(MapaNumeroLinhas(jogo.mapa), MapaNumeroColunas(jogo.mapa));

    while (!JogoAcabou(jogo))
    {
        trilha = TrilhaAtualizar(trilha, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan), jogo.movimentos);
        char movimento = JogoLerJogada();
        jogo.jogada = movimento;
        jogo = JogoMoverPacMan(jogo, movimento);
        jogo = JogoMoverFantasmas(jogo);
        jogo = JogoVerificaColisao(jogo);
        jogo.movimentos++;
        JogoGerarResumo(jogo, arqResumo, movimento);
        jogo = JogoAtualizarPontos(jogo);
        JogoImprimir(jogo, movimento);
        jogo.colidiu = 0;
    }

    trilha = TrilhaAtualizar(trilha, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan), jogo.movimentos);
    JogoFinalizar(jogo);
    RankingGerarArquivo(4, jogo.ranking, arqRanking);
    JogoGerarEstatistica(jogo, arqEstatistica);
    TrilhaGerarArquivo(trilha, arqTrilha);

    fclose(arqTrilha);
    fclose(arqEstatistica);
    fclose(arqRanking);
    fclose(arqResumo);
    return 0;
}

// Funcoes de Fantasma
tFantasma FantasmaCriar(int linha, int coluna, char fantasma)
{
    tFantasma f;
    f.linha = linha;
    f.coluna = coluna;
    f.fantasma = fantasma;

    if (fantasma == FANTASMA_B)
    {
        f.sentido = ESQUERDA;
    }
    else if (fantasma == FANTASMA_C)
    {
        f.sentido = DIREITA;
    }
    else if (fantasma == FANTASMA_I)
    {
        f.sentido = BAIXO;
    }
    else if (fantasma == FANTASMA_P)
    {
        f.sentido = CIMA;
    }
    return f;
}

int EhFantasma(tFantasma fantasma, int linha, int coluna)
{
    if (fantasma.linha == linha && fantasma.coluna == coluna)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int EhFantasmaLista(int tamFantasmas, tFantasma fantasma[tamFantasmas], int linha, int coluna)
{
    for (int i = 0; i < tamFantasmas; i++)
    {
        if (EhFantasma(fantasma[i], linha, coluna) == 1)
        {
            return 1;
        }
    }
    return 0; // nao tem fantasma correspondente a essa linha e coluna
}

int FantasmaLinha(tFantasma fantasma)
{
    return fantasma.linha;
}

int FantasmaColuna(tFantasma fantasma)
{
    return fantasma.coluna;
}
char FantasmaCaractere(tFantasma fantasma)
{
    return fantasma.fantasma;
}

char FantasmaSentido(tFantasma fantasma)
{
    return fantasma.sentido;
}

tFantasma FantasmaMover(tFantasma fantasma, char sentido)
{
    if (sentido == ESQUERDA)
    {
        fantasma.coluna--;
    }
    else if (sentido == DIREITA)
    {
        fantasma.coluna++;
    }
    else if (sentido == CIMA)
    {
        fantasma.linha--;
    }
    else if (sentido == BAIXO)
    {
        fantasma.linha++;
    }
    fantasma.sentido = sentido;
    return fantasma;
}

// Funcoes de PacMan

tPacMan PacManCriar(int linha, int coluna)
{
    tPacMan pacMan;
    pacMan.linha = linha;
    pacMan.coluna = coluna;
    pacMan.pacMan = PACMAN;
    return pacMan;
}

int EhPacMan(tPacMan pacMan, int linha, int coluna)
{
    if (pacMan.linha == linha && pacMan.coluna == coluna)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int PacManLinha(tPacMan pacMan)
{
    return pacMan.linha;
}

int PacManColuna(tPacMan pacMan)
{
    return pacMan.coluna;
}

tPacMan PacManMover(tPacMan pacMan, char sentido)
{
    if (sentido == ESQUERDA)
    {
        pacMan.coluna--;
    }
    else if (sentido == DIREITA)
    {
        pacMan.coluna++;
    }
    else if (sentido == CIMA)
    {
        pacMan.linha--;
    }
    else if (sentido == BAIXO)
    {
        pacMan.linha++;
    }
    return pacMan;
}

// Funcoes de Mapa

int EhFantasmaMapa(tMapa mapa, int linha, int coluna)
{
    if (mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_P || mapa.matriz[linha][coluna] == FANTASMA_I || mapa.matriz[linha][coluna] == FANTASMA_C)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int EhPacManMapa(tMapa mapa, int linha, int coluna)
{
    if (mapa.matriz[linha][coluna] == PACMAN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

tMapa MapaCriar(FILE *arquivo, int linhas, int colunas)
{
    tMapa mapa;
    mapa.linhas = linhas;
    mapa.colunas = colunas;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            fscanf(arquivo, "%c", &mapa.matriz[i][j]);
        }
        fscanf(arquivo, "\n");
    }
    return mapa;
}

void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma])
{
    for (int linha = 0; linha < mapa.linhas; linha++)
    {
        for (int coluna = 0; coluna < mapa.colunas; coluna++)
        {
            if (EhFantasmaLista(tamFantasma, fantasmas, linha, coluna) == 1)
            {
                for (int i = 0; i < tamFantasma; i++)
                {
                    if (EhFantasma(fantasmas[i], linha, coluna) == 1)
                    {
                        printf("%c", FantasmaCaractere(fantasmas[i]));
                        break;
                    }
                }
            }
            else if (EhPacMan(pacMan, linha, coluna) == 1)
            {
                printf("%c", PACMAN);
            }
            else
            {
                printf("%c", mapa.matriz[linha][coluna]);
            }
        }
        printf("\n");
    }
}

int EhParede(tMapa mapa, int linha, int coluna)
{
    if (mapa.matriz[linha][coluna] == PAREDE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int EhComida(tMapa mapa, int linha, int coluna)
{
    if (mapa.matriz[linha][coluna] == COMIDA)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

tPacMan MapaAchaPacMan(tMapa mapa)
{
    for (int linha = 0; linha < mapa.linhas; linha++)
    {
        for (int coluna = 0; coluna < mapa.colunas; coluna++)
        {
            if (mapa.matriz[linha][coluna] == PACMAN)
            {
                tPacMan pacMan = PacManCriar(linha, coluna);
                return pacMan;
            }
        }
    }
}

tMapa MapaRemove(tMapa mapa, int linha, int coluna)
{
    mapa.matriz[linha][coluna] = VAZIO;
    return mapa;
}

int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS])
{
    int nFantasmas = 0;
    for (int linha = 0; linha < mapa.linhas; linha++)
    {
        for (int coluna = 0; coluna < mapa.colunas; coluna++)
        {
            char charFantasma = mapa.matriz[linha][coluna];
            if (mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_C || mapa.matriz[linha][coluna] == FANTASMA_I || mapa.matriz[linha][coluna] == FANTASMA_P)
            {
                tFantasma fantasma = FantasmaCriar(linha, coluna, charFantasma);
                fantasmas[nFantasmas] = fantasma;
                nFantasmas++;
            }
        }
    }
    return nFantasmas;
} // dificil

int MapaNumeroLinhas(tMapa mapa)
{
    return mapa.linhas;
}

int MapaNumeroColunas(tMapa mapa)
{
    return mapa.colunas;
}

int MapaNumeroComidas(tMapa mapa)
{
    int n_comidas = 0;
    for (int linha = 0; linha < mapa.linhas; linha++)
    {
        for (int coluna = 0; coluna < mapa.colunas; coluna++)
        {
            if (mapa.matriz[linha][coluna] == COMIDA)
            {
                n_comidas++;
            }
        }
    }
    return n_comidas;
}

void MapaGerarInicializacao(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], FILE *arqInicializacao)
{
    for (int linha = 0; linha < mapa.linhas; linha++)
    {
        for (int coluna = 0; coluna < mapa.colunas; coluna++)
        {
            if (EhFantasmaLista(tamFantasma, fantasmas, linha, coluna) == 1)
            {
                for (int i = 0; i < tamFantasma; i++)
                {
                    if (EhFantasma(fantasmas[i], linha, coluna) == 1)
                    {
                        fprintf(arqInicializacao, "%c", FantasmaCaractere(fantasmas[i]));
                        break;
                    }
                }
            }
            else if (EhPacMan(pacMan, linha, coluna) == 1)
            {
                fprintf(arqInicializacao, "%c", PACMAN);
            }
            else
            {
                fprintf(arqInicializacao, "%c", mapa.matriz[linha][coluna]);
            }
        }
        fprintf(arqInicializacao, "\n");
    }
}

// Funcoes de Jogo
tJogo JogoCriar(char *diretorio)
{
    // Casos/Gabarito/simples/01
    // Casos/Gabarito/simples/01/mapa.txt
    char nomeArquivoMapa[2000];
    sprintf(nomeArquivoMapa, "%s/mapa.txt", diretorio);
    FILE *arquivoMapa = fopen(nomeArquivoMapa, "r");
    if (!arquivoMapa)
    {
        printf("ERRO: o programa nao conseguiu abrir o arquivo %s\n", nomeArquivoMapa);
        exit(1);
    }
    tJogo jogo;
    int linhas = 0, colunas = 0, limiteMovimentos = 0;
    fscanf(arquivoMapa, "%d %d %d\n", &linhas, &colunas, &limiteMovimentos);
    jogo.mapa = MapaCriar(arquivoMapa, linhas, colunas);
    jogo.pacMan = MapaAchaPacMan(jogo.mapa);
    jogo.tamFantasmas = MapaAchaFantasmas(jogo.mapa, jogo.fantasmas);
    jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)); // remove pacman do mapa
    for (int i = 0; i < jogo.tamFantasmas; i++)
    {
        jogo.mapa = MapaRemove(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i])); // remove todos os fantasmas
    }

    for (int i = 0; i < 4; i++)
    {
        jogo.ranking[i] = RankingCriar(i);
    }
    jogo.colidiu = 0;
    jogo.numComidas = MapaNumeroComidas(jogo.mapa);
    jogo.limiteMovimentos = limiteMovimentos;
    jogo.vivo = 1;
    jogo.pontos = 0;
    jogo.movimentos = 0;
    fclose(arquivoMapa);
    return jogo;
}

void JogoImprimir(tJogo jogo, char movimento)
{
    printf("Estado do jogo apos o movimento '%c':\n", movimento);
    MapaImprimir(jogo.mapa, jogo.pacMan, jogo.tamFantasmas, jogo.fantasmas);
    printf("Pontuacao: %d\n", jogo.pontos);
    printf("\n");
}

char JogoLerJogada()
{
    char movimento;
    scanf("%c\n", &movimento);
    return movimento;
}

tJogo JogoMoverPacMan(tJogo jogo, char movimento)
{
    if (movimento == CIMA)
    {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan) - 1, PacManColuna(jogo.pacMan)) == 0)
        {                                                 // se nao for parede na posicao de cima
            jogo.pacMan = PacManMover(jogo.pacMan, CIMA); // move pra cima
        }
        else
        {
            jogo.ranking[W].colisao++;
            jogo.colidiu = 1;
        }
        jogo.ranking[W].movimentos++;
    }
    else if (movimento == ESQUERDA)
    {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) - 1) == 0)
        {
            jogo.pacMan = PacManMover(jogo.pacMan, ESQUERDA);
        }
        else
        {
            jogo.ranking[A].colisao++;
            jogo.colidiu = 1;
        }
        jogo.ranking[A].movimentos++;
    }
    else if (movimento == BAIXO)
    {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan) + 1, PacManColuna(jogo.pacMan)) == 0)
        {
            jogo.pacMan = PacManMover(jogo.pacMan, BAIXO);
        }
        else
        {
            jogo.ranking[S].colisao++;
            jogo.colidiu = 1;
        }
        jogo.ranking[S].movimentos++;
    }
    else if (movimento == DIREITA)
    {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) + 1) == 0)
        {
            jogo.pacMan = PacManMover(jogo.pacMan, DIREITA);
        }
        else
        {
            jogo.ranking[D].colisao++;
            jogo.colidiu = 1;
        }
        jogo.ranking[D].movimentos++;
    }
    return jogo;
}

tJogo JogoMoverFantasmas(tJogo jogo)
{
    for (int i = 0; i < jogo.tamFantasmas; i++)
    {
        int sentido = FantasmaSentido(jogo.fantasmas[i]);
        if (sentido == CIMA)
        {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) - 1, FantasmaColuna(jogo.fantasmas[i])) == 0)
            {                                                               // se nao for parede na posicao de cima
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA); // move pra cima
            }
            else
            {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO); // se for parede, vai pra baixo
                sentido = BAIXO;
            }
        }
        else if (sentido == ESQUERDA)
        {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) - 1) == 0)
            { // se nao for parede na posição da esquerda
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA);
            }
            else
            {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA); // se for parede, vai pra direita
                sentido = DIREITA;
            }
        }
        else if (sentido == BAIXO)
        {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) + 1, FantasmaColuna(jogo.fantasmas[i])) == 0)
            { // se nao for parede na posicao de baixo
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO);
            }
            else
            {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA); // se for parede, vai pra cima
                sentido = CIMA;
            }
        }
        else if (sentido == DIREITA)
        {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) + 1) == 0)
            { // se nao for parede na posicao da direita
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA);
            }
            else
            {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA); // se for parede, vai pra esquerda
                sentido = ESQUERDA;
            }
        }
        /*
            if(EhFantasmaLista(jogo.tamFantasmas, jogo.fantasmas, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1){
                char sentidoPac = jogo.jogada;
                if(sentidoPac == DIREITA && sentido == ESQUERDA){
                    jogo.vivo = 0;
                }else if(sentidoPac == ESQUERDA && sentido == DIREITA){
                    jogo.vivo = 0;
                }else if(sentidoPac == CIMA && sentido == BAIXO){
                    jogo.vivo = 0;
                }else if(sentidoPac == BAIXO && sentido == CIMA){
                    jogo.vivo = 0;
                }
            }
        */
    }
    return jogo;
}

tJogo JogoVerificaColisao(tJogo jogo)
{
    // verificar se tem colisao com um fantasma
    if (EhFantasmaLista(jogo.tamFantasmas, jogo.fantasmas, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1)
    {
        jogo.vivo = 0;
    }
    return jogo;
}

tJogo JogoAtualizarPontos(tJogo jogo)
{
    if (EhComida(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1 && jogo.vivo == 1)
    {
        jogo.pontos++;
        jogo.numComidas--;
        jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan));

        if (jogo.jogada == DIREITA)
        {
            jogo.ranking[D].pontos++;
        }
        else if (jogo.jogada == ESQUERDA)
        {
            jogo.ranking[A].pontos++;
        }
        else if (jogo.jogada == CIMA)
        {
            jogo.ranking[W].pontos++;
        }
        else if (jogo.jogada == BAIXO)
        {
            jogo.ranking[S].pontos++;
        }
    }
    return jogo;
}

int JogoAcabou(tJogo jogo)
{
    if (jogo.numComidas == 0 || jogo.movimentos == jogo.limiteMovimentos || jogo.vivo == 0)
    {
        return 1;
    }
    return 0;
}

void JogoFinalizar(tJogo jogo)
{
    if (jogo.vivo == 1 && jogo.numComidas == 0)
    { // se o pacman estiver vivo e nao tiver mais comidas ou seja nao excedeu o numero maximo de movimentos
        printf("Voce venceu!\n");
    }
    else
    {
        printf("Game over!\n");
    }
    printf("Pontuacao final: %d\n", jogo.pontos);
}

void JogoGerarResumo(tJogo jogo, FILE *arqResumo, char movimento)
{
    if (jogo.vivo == 0)
    {
        fprintf(arqResumo, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n", jogo.movimentos, movimento);
    }
    else if (EhComida(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1)
    {
        fprintf(arqResumo, "Movimento %d (%c) pegou comida\n", jogo.movimentos, movimento);
    }
    else if (jogo.colidiu == 1)
    {
        fprintf(arqResumo, "Movimento %d (%c) colidiu na parede\n", jogo.movimentos, movimento);
    }
}

void JogoGerarInicializacao(tJogo jogo, FILE *arqInicializacao)
{
    MapaGerarInicializacao(jogo.mapa, jogo.pacMan, jogo.tamFantasmas, jogo.fantasmas, arqInicializacao);
    fprintf(arqInicializacao, "Pac-Man comecara o jogo na linha %d e coluna %d\n", PacManLinha(jogo.pacMan) + 1, PacManColuna(jogo.pacMan) + 1);
}

void JogoGerarEstatistica(tJogo jogo, FILE *arqEstatistica)
{
    fprintf(arqEstatistica, "Numero de movimentos: %d\n", jogo.movimentos);
    fprintf(arqEstatistica, "Numero de movimentos sem pontuar: %d\n", jogo.movimentos - jogo.pontos);
    fprintf(arqEstatistica, "Numero de colisoes com parede: %d\n", jogo.ranking[D].colisao + jogo.ranking[A].colisao + jogo.ranking[W].colisao + jogo.ranking[S].colisao);
    fprintf(arqEstatistica, "Numero de movimentos para baixo: %d\n", jogo.ranking[S].movimentos);
    fprintf(arqEstatistica, "Numero de movimentos para cima: %d\n", jogo.ranking[W].movimentos);
    fprintf(arqEstatistica, "Numero de movimentos para a esquerda: %d\n", jogo.ranking[A].movimentos);
    fprintf(arqEstatistica, "Numero de movimentos para a direita: %d\n", jogo.ranking[D].movimentos);
}
// ranking
tRanking RankingCriar(int i)
{
    tRanking ranking;
    if (i == W)
    {
        ranking.direcao = 'w';
    }
    else if (i == A)
    {
        ranking.direcao = 'a';
    }
    else if (i == S)
    {
        ranking.direcao = 's';
    }
    else if (i == D)
    {
        ranking.direcao = 'd';
    }

    ranking.pontos = 0;
    ranking.colisao = 0;
    ranking.movimentos = 0;
    return ranking;
}

tRanking RankingAtualizar(tRanking ranking, char direcao, int pontos, int colisao, int movimentos)
{
    ranking.direcao = direcao;
    ranking.pontos = pontos;
    ranking.colisao = colisao;
    ranking.movimentos = movimentos;
    return ranking;
}

void RankingGerarArquivo(int tamRanking, tRanking ranking[tamRanking], FILE *arqRanking)
{
    // primeira etapa ordenar o ranking
    // o melhor movimento eh aquele que mais pontos fez
    // criterio de desempate eh o que menos colisoes teve
    // segundo criterio de desempate eh o que mais movimentos fez
    // terceiro criterio de desempate eh a ordem alfabetica
    int i, j;
    tRanking aux;
    for (i = 0; i < tamRanking - 1; i++)
    {
        for (j = i + 1; j < tamRanking; j++)
        {
            if (ranking[i].pontos < ranking[j].pontos)
            { // ordenar por pontos
                aux = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = aux;
            }
            else if (ranking[i].pontos == ranking[j].pontos)
            { // se os pontos forem iguais, ordenar por colisao
                if (ranking[i].colisao > ranking[j].colisao)
                {
                    aux = ranking[i];
                    ranking[i] = ranking[j];
                    ranking[j] = aux;
                }
                else if (ranking[i].colisao == ranking[j].colisao)
                { // se os pontos e as colisoes forem iguais, ordenar por movimentos
                    if (ranking[i].movimentos < ranking[j].movimentos)
                    {
                        aux = ranking[i];
                        ranking[i] = ranking[j];
                        ranking[j] = aux;
                    }
                    else if (ranking[i].movimentos == ranking[j].movimentos)
                    { // se os pontos, as colisoes e os movimentos forem iguais, ordenar por ordem alfabetica
                        if (ranking[i].direcao > ranking[j].direcao)
                        {
                            aux = ranking[i];
                            ranking[i] = ranking[j];
                            ranking[j] = aux;
                        }
                    }
                }
            }
        }
    }
    // segunda etapa gerar o arquivo
    for (i = 0; i < tamRanking; i++)
    {
        fprintf(arqRanking, "%c,%d,%d,%d\n", ranking[i].direcao, ranking[i].pontos, ranking[i].colisao, ranking[i].movimentos);
    }
}

// trilha
tTrilha TrilhaCriar(int linhas, int colunas)
{
    tTrilha trilha;
    trilha.linha = linhas;
    trilha.coluna = colunas;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            trilha.matrix[i][j] = -1;
        }
    }
    return trilha;
}
tTrilha TrilhaAtualizar(tTrilha trilha, int linha, int coluna, int movimento)
{
    trilha.matrix[linha][coluna] = movimento;
    return trilha;
}
void TrilhaGerarArquivo(tTrilha trilha, FILE *arqTrilha)
{
    for (int i = 0; i < trilha.linha; i++)
    {
        for (int j = 0; j < trilha.coluna; j++)
        {
            if (trilha.matrix[i][j] == -1)
            {
                fprintf(arqTrilha, "%c", PAREDE);
            }
            else
            {
                fprintf(arqTrilha, "%d", trilha.matrix[i][j]);
            }
            if (j < trilha.coluna - 1)
            {
                fprintf(arqTrilha, " ");
            }
        }
        fprintf(arqTrilha, "\n");
    }
}
