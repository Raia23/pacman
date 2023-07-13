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
tJogo JogoVerificaColisao(tJogo jogo);
tJogo JogoAtualizarPontos(tJogo jogo);
void JogoFinalizar(tJogo jogo);
//void JogoGerarResumo(tJogo jogo);

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
        jogo = JogoVerificaColisao(jogo);
        jogo = JogoAtualizarPontos(jogo);
        JogoImprimir(jogo, movimento);
        // JogoGerarResumo(jogo);
    }
    JogoFinalizar(jogo);

    return 0;
}

// Funcoes de Fantasma 
tFantasma FantasmaCriar(int linha, int coluna, char fantasma){
    tFantasma f;
    f.linha = linha;
    f.coluna = coluna;
    f.fantasma = fantasma;

    if(fantasma == FANTASMA_B){
        f.sentido = ESQUERDA;
    }else if(fantasma == FANTASMA_C){
        f.sentido = DIREITA;
    }else if(fantasma == FANTASMA_I){
        f.sentido = BAIXO;
    }else if(fantasma == FANTASMA_P){
        f.sentido = CIMA;
    }
    return f;
}

int EhFantasma(tFantasma fantasma, int linha, int coluna){
    if(fantasma.linha == linha && fantasma.coluna == coluna){
        return 1;
    }else{
        return 0;
    }
}

int EhFantasmaLista(int tamFantasmas, tFantasma fantasma[tamFantasmas], int linha, int coluna){
    for(int i = 0; i < tamFantasmas; i++){
        if(EhFantasma(fantasma[i], linha, coluna) == 1){
            return 1;
        }
    }
    return 0; // nao tem fantasma correspondente a essa linha e coluna
}

int FantasmaLinha(tFantasma fantasma){
    return fantasma.linha;
}

int FantasmaColuna(tFantasma fantasma){
    return fantasma.coluna;
}
char FantasmaCaractere(tFantasma fantasma){
    return fantasma.fantasma;
}

char FantasmaSentido(tFantasma fantasma){
    return fantasma.sentido;
}

tFantasma FantasmaMover(tFantasma fantasma, char sentido){
    if(sentido == ESQUERDA){
        fantasma.coluna--;
    }else if(sentido == DIREITA){
        fantasma.coluna++;
    }else if(sentido == CIMA){
        fantasma.linha--;
    }else if(sentido == BAIXO){
        fantasma.linha++;
    }
    fantasma.sentido = sentido;
    return fantasma;
}

// Funcoes de PacMan

tPacMan PacManCriar(int linha, int coluna){
    tPacMan pacMan;
    pacMan.linha = linha;
    pacMan.coluna = coluna;
    pacMan.pacMan = PACMAN;
    return pacMan;
}

int EhPacMan(tPacMan pacMan, int linha, int coluna){
    if(pacMan.linha == linha && pacMan.coluna == coluna){
        return 1;
    }else{
        return 0;
    }
}

int PacManLinha(tPacMan pacMan){
    return pacMan.linha;
}

int PacManColuna(tPacMan pacMan){
    return pacMan.coluna;
}

tPacMan PacManMover(tPacMan pacMan, char sentido){
    if(sentido == ESQUERDA){
        pacMan.coluna--;
    }else if(sentido == DIREITA){
        pacMan.coluna++;
    }else if(sentido == CIMA){
        pacMan.linha--;
    }else if(sentido == BAIXO){
        pacMan.linha++;
    }
    return pacMan;
}

// Funcoes de Mapa

int EhFantasmaMapa(tMapa mapa, int linha, int coluna){
    if(mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_P ||  mapa.matriz[linha][coluna] == FANTASMA_I || mapa.matriz[linha][coluna] == FANTASMA_C){
        return 1;
    }else{
        return 0;
    }
}
int EhPacManMapa(tMapa mapa, int linha, int coluna){
    if( mapa.matriz[linha][coluna] == PACMAN){
        return 1;
    }else{
        return 0;
    }
} 

tMapa MapaCriar(FILE *arquivo, int linhas, int colunas){
    tMapa mapa;
    mapa.linhas = linhas;
    mapa.colunas = colunas;
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            fscanf(arquivo, "%c" , &mapa.matriz[i][j]);
        }
        fscanf(arquivo, "\n");
    }
    return mapa;
} 

void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma]){
    for(int linha = 0; linha <mapa.linhas; linha++){
        for(int coluna = 0; coluna < mapa.colunas; coluna++){     
            if(EhFantasmaLista(tamFantasma, fantasmas, linha, coluna) == 1){
                for(int i = 0; i < tamFantasma; i++){
                    if(EhFantasma(fantasmas[i], linha, coluna) == 1){
                        printf("%c", FantasmaCaractere(fantasmas[i]));
                        break;
                    }
                }
            }else if(EhPacMan(pacMan, linha, coluna) == 1){
                printf("%c", PACMAN);
            }
            else{
                printf("%c", mapa.matriz[linha][coluna]);
            }
        }
        printf("\n");
    }
} 

int EhParede(tMapa mapa, int linha, int coluna){
    if( mapa.matriz[linha][coluna] == PAREDE){
        return 1;
    }else{
        return 0;
    }
}

int EhComida(tMapa mapa, int linha, int coluna){
    if( mapa.matriz[linha][coluna] == COMIDA){
        return 1;
    }else{
        return 0;
    }
}

tPacMan MapaAchaPacMan(tMapa mapa){
    for(int linha = 0; linha <mapa.linhas; linha++){
        for(int coluna = 0; coluna < mapa.colunas; coluna++){
            if(mapa.matriz[linha][coluna] == PACMAN){
                tPacMan pacMan = PacManCriar(linha, coluna);
                return pacMan;
            }
        }
    }    
}

tMapa MapaRemove(tMapa mapa, int linha, int coluna){
    mapa.matriz[linha][coluna] = VAZIO;
    return mapa;
}

int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS]){
    int nFantasmas = 0;
    for(int linha = 0; linha <mapa.linhas; linha++){
        for(int coluna = 0; coluna < mapa.colunas; coluna++){
            char charFantasma = mapa.matriz[linha][coluna];
            if(mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_C || mapa.matriz[linha][coluna] == FANTASMA_I || mapa.matriz[linha][coluna] == FANTASMA_P){
                tFantasma fantasma = FantasmaCriar(linha, coluna, charFantasma);
                fantasmas[nFantasmas] = fantasma;
                nFantasmas++;
            }
        }
    } 
    return nFantasmas;   
} // dificil

int MapaNumeroLinhas(tMapa mapa){
    return mapa.linhas;
}

int MapaNumeroColunas(tMapa mapa){
    return mapa.colunas;
}

int MapaNumeroComidas(tMapa mapa){
    int n_comidas = 0;
    for(int linha = 0; linha <mapa.linhas; linha++){
        for(int coluna = 0; coluna < mapa.colunas; coluna++){
            if(mapa.matriz[linha][coluna] == COMIDA){
                n_comidas++;
            }
        }
    }
    return n_comidas;
}

// Funcoes de Jogo
tJogo JogoCriar(char *diretorio){
    // Casos/Gabarito/simples/01
    // Casos/Gabarito/simples/01/mapa.txt
    char nomeArquivoMapa[2000];
    sprintf(nomeArquivoMapa, "%s/mapa.txt", diretorio);
    FILE * arquivoMapa = fopen(nomeArquivoMapa, "r");
    if(!arquivoMapa){
        printf("ERRO: o programa nao conseguiu abrir o arquivo %s\n", nomeArquivoMapa);
        exit(1);
    }
    tJogo jogo;
    int linhas = 0, colunas = 0, limiteMovimentos = 0;
    fscanf(arquivoMapa, "%d %d %d\n", &linhas, &colunas, &limiteMovimentos);
    jogo.mapa = MapaCriar(arquivoMapa, linhas, colunas);
    jogo.pacMan = MapaAchaPacMan(jogo.mapa);
    jogo.tamFantasmas = MapaAchaFantasmas(jogo.mapa, jogo.fantasmas); 
    jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)); //remove pacman do mapa
    for(int i = 0; i < jogo.tamFantasmas; i++){
        jogo.mapa = MapaRemove(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i])); //remove todos os fantasmas
    }
    jogo.numComidas = MapaNumeroComidas(jogo.mapa);
    jogo.limiteMovimentos = limiteMovimentos;
    jogo.vivo = 1;
    jogo.pontos = 0;
    jogo.movimentos = 0;
    fclose(arquivoMapa);
    return jogo;
}

void JogoImprimir(tJogo jogo, char movimento){
    printf("Estado do jogo apos o movimento '%c':\n", movimento);
    MapaImprimir(jogo.mapa, jogo.pacMan, jogo.tamFantasmas, jogo.fantasmas);
    printf("Pontuacao: %d\n", jogo.pontos);
    printf("\n");
}

char JogoLerJogada(){
    char movimento;
    scanf("%c\n", &movimento);
    return movimento;
}

tJogo JogoMoverPacMan(tJogo jogo, char movimento){
    if(movimento == CIMA){
        if(EhParede(jogo.mapa, PacManLinha(jogo.pacMan) - 1, PacManColuna(jogo.pacMan)) == 0){ // se nao for parede na posicao de cima
            jogo.pacMan = PacManMover(jogo.pacMan, CIMA); // move pra cima
        }
        // se for parede fica parado
    }else if(movimento == ESQUERDA){
        if(EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) - 1) == 0){
            jogo.pacMan = PacManMover(jogo.pacMan, ESQUERDA);
        }
    }else if(movimento == BAIXO){
        if(EhParede(jogo.mapa, PacManLinha(jogo.pacMan) + 1, PacManColuna(jogo.pacMan)) == 0){
            jogo.pacMan = PacManMover(jogo.pacMan, BAIXO);
        }
    }else if(movimento == DIREITA){
        if(EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) + 1) == 0){
            jogo.pacMan = PacManMover(jogo.pacMan, DIREITA);
        }
    }
    return jogo;
}

tJogo JogoMoverFantasmas(tJogo jogo){
    for(int i = 0; i < jogo.tamFantasmas; i++){
        int sentido = FantasmaSentido(jogo.fantasmas[i]);
        if(sentido == CIMA){
            if(EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) - 1, FantasmaColuna(jogo.fantasmas[i])) == 0){ // se nao for parede na posicao de cima
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA); // move pra cima
            }else{
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO); // se for parede, vai pra baixo
            }
        }else if(sentido == ESQUERDA){
            if(EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) - 1) == 0){  // se nao for parede na posição da esquerda
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA);
            }else{
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA); // se for parede, vai pra direita
            }
        }else if(sentido == BAIXO){
            if(EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) + 1, FantasmaColuna(jogo.fantasmas[i])) == 0){ // se nao for parede na posicao de baixo
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO);
            }else{
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA); // se for parede, vai pra cima
            }
        }else if(sentido == DIREITA){ 
            if(EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) + 1) == 0){ // se nao for parede na posicao da direita
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA);
            }else{
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA); // se for parede, vai pra esquerda
            }
        }
    }
    return jogo;
}

tJogo JogoVerificaColisao(tJogo jogo){
    // verificar se tem colisao com um fantasma
    if(EhFantasmaLista(jogo.tamFantasmas, jogo.fantasmas, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1){
        jogo.vivo = 0;
    }
    return jogo;
}

tJogo JogoAtualizarPontos(tJogo jogo){
    if(EhComida(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1){
        jogo.pontos++;
        jogo.numComidas--;
        jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan));
    }
    return jogo;
}

int JogoAcabou(tJogo jogo){
    if(jogo.numComidas == 0 || jogo.movimentos == jogo.limiteMovimentos || jogo.vivo == 0){
        return 1;
    }
    return 0;
}

void JogoFinalizar(tJogo jogo){
    if(jogo.vivo == 1 && jogo.numComidas == 0){ // se o pacman estiver vivo e nao tiver mais comidas ou seja nao excedeu o numero maximo de movimentos
        printf("Voce venceu!\n");
    }else{
        printf("Game over!\n");
    }
    printf("Pontuacao final: %d\n", jogo.pontos);
}