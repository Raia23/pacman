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
#define TUNEL '@'

typedef struct tunel {
    int linha;
    int coluna;
} tTunel;

tTunel TunelCriar(int linha, int coluna);
int EhTunel(tTunel tunel[2], int linha, int coluna);
tTunel TunelEscolheTunelParaTeleportar(tTunel tunel1, tTunel tunel2, int linha, int coluna);

typedef struct estatistica {
    int w;
    int a;
    int s;
    int d;
    int colisao;
} tEstatistica;

tEstatistica EstatisticaCriar();
tEstatistica EstatisticaAtualizar(tEstatistica estatistica, char direcao);

typedef struct ranking {
    char direcao;
    int pontos;
    int colisao;
    int movimentos;
} tRanking;

tRanking RankingCriar(int i);
void RankingAtualizarMovimentos(tRanking rankings[4], char direcao);
void RankingAtualizarColisao(tRanking rankings[4], char movimento);
void RankingAtualizarPontos(tRanking rankings[4], char movimento);
void RankingGerarArquivo(int tamRanking, tRanking ranking[tamRanking], FILE *arqRanking);

typedef struct fantasma {
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

typedef struct pacMan {
    int linha;
    int coluna;
    char pacMan;
} tPacMan;

tPacMan PacManCriar(int linha, int coluna);
int EhPacMan(tPacMan pacMan, int linha, int coluna);
int PacManLinha(tPacMan pacMan);
int PacManColuna(tPacMan pacMan);
tPacMan PacManMover(tPacMan pacMan, char sentido);
tPacMan PacManTeleportar(tPacMan pacMan, int linha, int coluna);

typedef struct mapa {
    int linhas;
    int colunas;
    char matriz[MAX_LINHAS][MAX_COLUNAS];
} tMapa;

int EhFantasmaMapa(tMapa mapa, int linha, int coluna);
int EhPacManMapa(tMapa mapa, int linha, int coluna);
tMapa MapaCriar(FILE *arquivo, int linhas, int colunas);
void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], tTunel tuneis[2], int numTuneis);
int EhParede(tMapa mapa, int linha, int coluna);
int EhComida(tMapa mapa, int linha, int coluna);
tPacMan MapaAchaPacMan(tMapa mapa);
tMapa MapaRemove(tMapa mapa, int linha, int coluna);
int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS]);
int MapaNumeroLinhas(tMapa mapa);
int MapaNumeroColunas(tMapa mapa);
int MapaNumeroComidas(tMapa mapa);
void MapaGerarInicializacao(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], FILE *arqInicializacao, tTunel tuneis[2],
                            int numTuneis);
int MapaAchaTuneis(tMapa mapa, tTunel tunel[2]);

typedef struct trilha {
    int linha;
    int coluna;
    int matrix[MAX_LINHAS][MAX_COLUNAS];
} tTrilha;

tTrilha TrilhaCriar(int linhas, int colunas);
tTrilha TrilhaAtualizar(tTrilha trilha, int linha, int coluna, int movimento);
void TrilhaGerarArquivo(tTrilha trilha, FILE *arqTrilha);

typedef struct jogo {
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
    tEstatistica estatistica;
    tTunel tunel[2]; // tem dois tuneis na fase bonus
    int numTuneis;
} tJogo;

tJogo JogoCriar(char *diretorio);
void JogoImprimir(tJogo jogo, char movimento);
char JogoLerJogada();
tJogo JogoMoverPacMan(tJogo jogo, char movimento);
int JogoPodeMoverPacMan(tJogo jogo, char movimento);
tJogo JogoMoverFantasmas(tJogo jogo);
int JogoAcabou(tJogo jogo);
int JogoVerificaColisaoFantasma(tJogo jogo);
int JogoEhComida(tJogo jogo);
void JogoFinalizar(tJogo jogo);
void JogoGerarInicializacao(tJogo jogo, FILE *arqInicializacao);
void JogoGerarEstatistica(tJogo jogo, FILE *arqEstatistica);
void JogoGerarResumoMorreu(tJogo jogo, FILE *arqResumo, char movimento);
void JogoGerarResumoComeu(tJogo jogo, FILE *arqResumo, char movimento);
void JogoGerarResumoColidiu(tJogo jogo, FILE *arqResumo, char movimento);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Erro: Informe o diretorio do mapa\n");
        return 1;
    }

    tJogo jogo = JogoCriar(argv[1]);

    char nomeArqResumo[1000];
    sprintf(nomeArqResumo, "%s/saida/resumo.txt", argv[1]);
    FILE *arqResumo = fopen(nomeArqResumo, "w");
    if (arqResumo == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo de resumo\n");
        return 1;
    }

    char nomeArqInicializacao[1000];
    sprintf(nomeArqInicializacao, "%s/saida/inicializacao.txt", argv[1]);
    FILE *arqInicializacao = fopen(nomeArqInicializacao, "w");
    if (arqInicializacao == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo de inicializacao\n");
        return 1;
    }

    JogoGerarInicializacao(jogo, arqInicializacao);
    fclose(arqInicializacao);

    char nomeArqRanking[1000];
    sprintf(nomeArqRanking, "%s/saida/ranking.txt", argv[1]);
    FILE *arqRanking = fopen(nomeArqRanking, "w");
    if (arqRanking == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo de ranking\n");
        return 1;
    }

    char nomeArqEstatistica[1000];
    sprintf(nomeArqEstatistica, "%s/saida/estatisticas.txt", argv[1]);
    FILE *arqEstatistica = fopen(nomeArqEstatistica, "w");
    if (arqEstatistica == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo de estatistica\n");
        return 1;
    }

    char nomeArqTrilha[1000];
    sprintf(nomeArqTrilha, "%s/saida/trilha.txt", argv[1]);
    FILE *arqTrilha = fopen(nomeArqTrilha, "w");
    if (arqTrilha == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo de trilha\n");
        return 1;
    }

    tTrilha trilha = TrilhaCriar(MapaNumeroLinhas(jogo.mapa), MapaNumeroColunas(jogo.mapa));
    trilha = TrilhaAtualizar(trilha, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan), jogo.movimentos);

    while (!JogoAcabou(jogo)) {
        char movimento = JogoLerJogada();
        jogo.movimentos++;
        jogo = JogoMoverFantasmas(jogo);
        int colisao = 0;
        if (JogoPodeMoverPacMan(jogo, movimento) == 1) {
            jogo.pacMan = PacManMover(jogo.pacMan, movimento);
        } else if (JogoPodeMoverPacMan(jogo, movimento) == 0) {
            colisao = 1;
            jogo.estatistica.colisao++;
        }

        // caso bonus
        if (EhTunel(jogo.tunel, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1) {
            tTunel tunel = TunelEscolheTunelParaTeleportar(jogo.tunel[0], jogo.tunel[1], PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan));
            trilha = TrilhaAtualizar(trilha, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan), jogo.movimentos);
            jogo.pacMan = PacManTeleportar(jogo.pacMan, tunel.linha, tunel.coluna);
            colisao = 0;
        }

        RankingAtualizarMovimentos(jogo.ranking, movimento);
        jogo.estatistica = EstatisticaAtualizar(jogo.estatistica, movimento);

        if (JogoVerificaColisaoFantasma(jogo) == 1 || JogoPodeMoverPacMan(jogo, movimento == 2)) {
            jogo.vivo = 0;
            JogoGerarResumoMorreu(jogo, arqResumo, movimento);
            if (colisao == 1) {
                RankingAtualizarColisao(jogo.ranking, movimento);
                JogoGerarResumoColidiu(jogo, arqResumo, movimento);
            }
            JogoImprimir(jogo, movimento);
            break;
        }

        if (colisao == 1) {
            RankingAtualizarColisao(jogo.ranking, movimento);
            JogoGerarResumoColidiu(jogo, arqResumo, movimento);
        }

        if (JogoEhComida(jogo) == 1) {
            jogo.pontos++;
            jogo.numComidas--;
            jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan));
            RankingAtualizarPontos(jogo.ranking, movimento);
            JogoGerarResumoComeu(jogo, arqResumo, movimento);
        }

        JogoImprimir(jogo, movimento);
        trilha = TrilhaAtualizar(trilha, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan), jogo.movimentos);
    }

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
tFantasma FantasmaCriar(int linha, int coluna, char fantasma) {
    tFantasma f;
    f.linha = linha;
    f.coluna = coluna;
    f.fantasma = fantasma;

    if (fantasma == FANTASMA_B) {
        f.sentido = ESQUERDA;
    } else if (fantasma == FANTASMA_C) {
        f.sentido = DIREITA;
    } else if (fantasma == FANTASMA_I) {
        f.sentido = BAIXO;
    } else if (fantasma == FANTASMA_P) {
        f.sentido = CIMA;
    }
    return f;
}

int EhFantasma(tFantasma fantasma, int linha, int coluna) {
    if (fantasma.linha == linha && fantasma.coluna == coluna) {
        return 1;
    } else {
        return 0;
    }
}

int EhFantasmaLista(int tamFantasmas, tFantasma fantasma[tamFantasmas], int linha, int coluna) {
    for (int i = 0; i < tamFantasmas; i++) {
        if (EhFantasma(fantasma[i], linha, coluna) == 1) {
            return 1;
        }
    }
    return 0; // nao tem fantasma correspondente a essa linha e coluna
}

int FantasmaLinha(tFantasma fantasma) { return fantasma.linha; }

int FantasmaColuna(tFantasma fantasma) { return fantasma.coluna; }
char FantasmaCaractere(tFantasma fantasma) { return fantasma.fantasma; }

char FantasmaSentido(tFantasma fantasma) { return fantasma.sentido; }

tFantasma FantasmaMover(tFantasma fantasma, char sentido) {
    if (sentido == ESQUERDA) {
        fantasma.coluna--;
    } else if (sentido == DIREITA) {
        fantasma.coluna++;
    } else if (sentido == CIMA) {
        fantasma.linha--;
    } else if (sentido == BAIXO) {
        fantasma.linha++;
    }
    fantasma.sentido = sentido;
    return fantasma;
}

// Funcoes de PacMan

tPacMan PacManCriar(int linha, int coluna) {
    tPacMan pacMan;
    pacMan.linha = linha;
    pacMan.coluna = coluna;
    pacMan.pacMan = PACMAN;
    return pacMan;
}

int EhPacMan(tPacMan pacMan, int linha, int coluna) {
    if (pacMan.linha == linha && pacMan.coluna == coluna) {
        return 1;
    } else {
        return 0;
    }
}

int PacManLinha(tPacMan pacMan) { return pacMan.linha; }

int PacManColuna(tPacMan pacMan) { return pacMan.coluna; }

tPacMan PacManMover(tPacMan pacMan, char sentido) {
    if (sentido == ESQUERDA) {
        pacMan.coluna--;
    } else if (sentido == DIREITA) {
        pacMan.coluna++;
    } else if (sentido == CIMA) {
        pacMan.linha--;
    } else if (sentido == BAIXO) {
        pacMan.linha++;
    }
    return pacMan;
}

// Funcoes de Mapa

int EhFantasmaMapa(tMapa mapa, int linha, int coluna) {
    if (mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_P || mapa.matriz[linha][coluna] == FANTASMA_I ||
        mapa.matriz[linha][coluna] == FANTASMA_C) {
        return 1;
    } else {
        return 0;
    }
}
int EhPacManMapa(tMapa mapa, int linha, int coluna) {
    if (mapa.matriz[linha][coluna] == PACMAN) {
        return 1;
    } else {
        return 0;
    }
}

tMapa MapaCriar(FILE *arquivo, int linhas, int colunas) {
    tMapa mapa;
    mapa.linhas = linhas;
    mapa.colunas = colunas;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%c", &mapa.matriz[i][j]);
        }
        fscanf(arquivo, "\n");
    }
    return mapa;
}

void MapaImprimir(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], tTunel tuneis[2], int numTuneis) {
    for (int linha = 0; linha < mapa.linhas; linha++) {
        for (int coluna = 0; coluna < mapa.colunas; coluna++) {
            if (EhFantasmaLista(tamFantasma, fantasmas, linha, coluna) == 1) {
                for (int i = 0; i < tamFantasma; i++) {
                    if (EhFantasma(fantasmas[i], linha, coluna) == 1) {
                        printf("%c", FantasmaCaractere(fantasmas[i]));
                        break;
                    }
                }
            } else if (EhPacMan(pacMan, linha, coluna) == 1) {
                printf("%c", PACMAN);
            } else if (numTuneis > 0 && EhTunel(tuneis, linha, coluna)) {
                printf("%c", TUNEL);
            } else {
                printf("%c", mapa.matriz[linha][coluna]);
            }
        }
        printf("\n");
    }
}

int EhParede(tMapa mapa, int linha, int coluna) {
    if (mapa.matriz[linha][coluna] == PAREDE) {
        return 1;
    } else {
        return 0;
    }
}

int EhComida(tMapa mapa, int linha, int coluna) {
    if (mapa.matriz[linha][coluna] == COMIDA) {
        return 1;
    } else {
        return 0;
    }
}

tPacMan MapaAchaPacMan(tMapa mapa) {
    for (int linha = 0; linha < mapa.linhas; linha++) {
        for (int coluna = 0; coluna < mapa.colunas; coluna++) {
            if (mapa.matriz[linha][coluna] == PACMAN) {
                tPacMan pacMan = PacManCriar(linha, coluna);
                return pacMan;
            }
        }
    }
}

tMapa MapaRemove(tMapa mapa, int linha, int coluna) {
    mapa.matriz[linha][coluna] = VAZIO;
    return mapa;
}

int MapaAchaFantasmas(tMapa mapa, tFantasma fantasmas[MAX_FANTASMAS]) {
    int nFantasmas = 0;
    for (int linha = 0; linha < mapa.linhas; linha++) {
        for (int coluna = 0; coluna < mapa.colunas; coluna++) {
            char charFantasma = mapa.matriz[linha][coluna];
            if (mapa.matriz[linha][coluna] == FANTASMA_B || mapa.matriz[linha][coluna] == FANTASMA_C || mapa.matriz[linha][coluna] == FANTASMA_I ||
                mapa.matriz[linha][coluna] == FANTASMA_P) {
                tFantasma fantasma = FantasmaCriar(linha, coluna, charFantasma);
                fantasmas[nFantasmas] = fantasma;
                nFantasmas++;
            }
        }
    }
    return nFantasmas;
} // dificil

int MapaNumeroLinhas(tMapa mapa) { return mapa.linhas; }

int MapaNumeroColunas(tMapa mapa) { return mapa.colunas; }

int MapaNumeroComidas(tMapa mapa) {
    int n_comidas = 0;
    for (int linha = 0; linha < mapa.linhas; linha++) {
        for (int coluna = 0; coluna < mapa.colunas; coluna++) {
            if (mapa.matriz[linha][coluna] == COMIDA) {
                n_comidas++;
            }
        }
    }
    return n_comidas;
}

void MapaGerarInicializacao(tMapa mapa, tPacMan pacMan, int tamFantasma, tFantasma fantasmas[tamFantasma], FILE *arqInicializacao, tTunel tuneis[2],
                            int numTuneis) {
    for (int linha = 0; linha < mapa.linhas; linha++) {
        for (int coluna = 0; coluna < mapa.colunas; coluna++) {
            if (EhFantasmaLista(tamFantasma, fantasmas, linha, coluna) == 1) {
                for (int i = 0; i < tamFantasma; i++) {
                    if (EhFantasma(fantasmas[i], linha, coluna) == 1) {
                        fprintf(arqInicializacao, "%c", FantasmaCaractere(fantasmas[i]));
                        break;
                    }
                }
            } else if (EhPacMan(pacMan, linha, coluna) == 1) {
                fprintf(arqInicializacao, "%c", PACMAN);
            } else if (numTuneis > 0 && EhTunel(tuneis, linha, coluna) == 1) {
                fprintf(arqInicializacao, "%c", TUNEL);
            } else {
                fprintf(arqInicializacao, "%c", mapa.matriz[linha][coluna]);
            }
        }
        fprintf(arqInicializacao, "\n");
    }
}

// Funcoes de Jogo
tJogo JogoCriar(char *diretorio) {
    // Casos/Gabarito/simples/01
    // Casos/Gabarito/simples/01/mapa.txt
    char nomeArquivoMapa[2000];
    sprintf(nomeArquivoMapa, "%s/mapa.txt", diretorio);
    FILE *arquivoMapa = fopen(nomeArquivoMapa, "r");
    if (!arquivoMapa) {
        printf("ERRO: o programa nao conseguiu abrir o arquivo %s\n", nomeArquivoMapa);
        exit(1);
    }
    tJogo jogo;
    int linhas = 0, colunas = 0, limiteMovimentos = 0;
    fscanf(arquivoMapa, "%d %d %d\n", &linhas, &colunas, &limiteMovimentos);
    jogo.mapa = MapaCriar(arquivoMapa, linhas, colunas);
    jogo.pacMan = MapaAchaPacMan(jogo.mapa);
    jogo.tamFantasmas = MapaAchaFantasmas(jogo.mapa, jogo.fantasmas);
    jogo.numTuneis = MapaAchaTuneis(jogo.mapa, jogo.tunel);

    if (jogo.numTuneis > 0) {
        for (int i = 0; i < 2; i++) {
            jogo.mapa = MapaRemove(jogo.mapa, jogo.tunel[i].linha, jogo.tunel[i].coluna);
        }
    }

    jogo.mapa = MapaRemove(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)); // remove pacman do mapa
    for (int i = 0; i < jogo.tamFantasmas; i++) {
        jogo.mapa = MapaRemove(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i])); // remove todos os fantasmas
    }

    for (int i = 0; i < 4; i++) {
        jogo.ranking[i] = RankingCriar(i);
    }

    jogo.estatistica = EstatisticaCriar();
    jogo.numComidas = MapaNumeroComidas(jogo.mapa);
    jogo.limiteMovimentos = limiteMovimentos;
    jogo.vivo = 1;
    jogo.pontos = 0;
    jogo.movimentos = 0;
    fclose(arquivoMapa);
    return jogo;
}

void JogoImprimir(tJogo jogo, char movimento) {
    printf("Estado do jogo apos o movimento '%c':\n", movimento);
    MapaImprimir(jogo.mapa, jogo.pacMan, jogo.tamFantasmas, jogo.fantasmas, jogo.tunel, jogo.numTuneis);
    printf("Pontuacao: %d\n", jogo.pontos);
    printf("\n");
}

char JogoLerJogada() {
    char movimento;
    scanf("%c\n", &movimento);
    return movimento;
}

int JogoPodeMoverPacMan(tJogo jogo, char movimento) {
    if (EhFantasmaLista(jogo.tamFantasmas, jogo.fantasmas, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1) {
        for (int i = 0; i < jogo.tamFantasmas; i++) {
            if (EhFantasma(jogo.fantasmas[i], PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1) {
                char fantasmaSentido = FantasmaSentido(jogo.fantasmas[i]);
                if (fantasmaSentido == CIMA && movimento == BAIXO) {
                    return 2;
                } else if (fantasmaSentido == BAIXO && movimento == CIMA) {
                    return 2;
                } else if (fantasmaSentido == ESQUERDA && movimento == DIREITA) {
                    return 2;
                } else if (fantasmaSentido == DIREITA && movimento == ESQUERDA) {
                    return 2;
                }
            }
        }
    }

    if (movimento == CIMA) {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan) - 1, PacManColuna(jogo.pacMan)) == 0) { // se nao for parede na posicao de cima
            return 1;
        }
        return 0;
    } else if (movimento == ESQUERDA) {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) - 1) == 0) {
            return 1;
        }
        return 0;
    } else if (movimento == BAIXO) {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan) + 1, PacManColuna(jogo.pacMan)) == 0) {
            return 1;
        }
        return 0;
    } else if (movimento == DIREITA) {
        if (EhParede(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan) + 1) == 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}

tJogo JogoMoverFantasmas(tJogo jogo) {
    for (int i = 0; i < jogo.tamFantasmas; i++) {
        int sentido = FantasmaSentido(jogo.fantasmas[i]);
        if (sentido == CIMA) {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) - 1, FantasmaColuna(jogo.fantasmas[i])) == 0) { // se nao for parede na posicao de cima
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA);
            } else {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO); // se for parede, vai pra baixo
                sentido = BAIXO;
            }
        } else if (sentido == ESQUERDA) {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) - 1) == 0) { // se nao for parede na posição da esquerda
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA);
            } else {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA); // se for parede, vai pra direita
                sentido = DIREITA;
            }
        } else if (sentido == BAIXO) {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]) + 1, FantasmaColuna(jogo.fantasmas[i])) == 0) { // se nao for parede na posicao de baixo
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], BAIXO);
            } else {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], CIMA); // se for parede, vai pra cima
                sentido = CIMA;
            }
        } else if (sentido == DIREITA) {
            if (EhParede(jogo.mapa, FantasmaLinha(jogo.fantasmas[i]), FantasmaColuna(jogo.fantasmas[i]) + 1) == 0) { // se nao for parede na posicao da direita
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], DIREITA);
            } else {
                jogo.fantasmas[i] = FantasmaMover(jogo.fantasmas[i], ESQUERDA); // se for parede, vai pra esquerda
                sentido = ESQUERDA;
            }
        }
    }
    return jogo;
}

int JogoVerificaColisaoFantasma(tJogo jogo) {
    // verificar se tem colisao com um fantasma
    if (EhFantasmaLista(jogo.tamFantasmas, jogo.fantasmas, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1) {
        return 1;
    }
    return 0;
}

int JogoEhComida(tJogo jogo) {
    if (EhComida(jogo.mapa, PacManLinha(jogo.pacMan), PacManColuna(jogo.pacMan)) == 1 && jogo.vivo == 1) {
        return 1;
    }
    return 0;
}

int JogoAcabou(tJogo jogo) {
    if (jogo.numComidas == 0 || jogo.movimentos == jogo.limiteMovimentos || jogo.vivo == 0) {
        return 1;
    }
    return 0;
}

void JogoFinalizar(tJogo jogo) {
    if (jogo.vivo == 1 && jogo.numComidas == 0) { // se o pacman estiver vivo e nao tiver mais comidas ou seja nao excedeu o numero maximo de movimentos
        printf("Voce venceu!\n");
    } else {
        printf("Game over!\n");
    }
    printf("Pontuacao final: %d\n", jogo.pontos);
}

void JogoGerarResumoMorreu(tJogo jogo, FILE *arqResumo, char movimento) {
    fprintf(arqResumo, "Movimento %d (%c) fim de jogo por encostar em um fantasma\n", jogo.movimentos, movimento);
}

void JogoGerarResumoComeu(tJogo jogo, FILE *arqResumo, char movimento) { fprintf(arqResumo, "Movimento %d (%c) pegou comida\n", jogo.movimentos, movimento); }

void JogoGerarResumoColidiu(tJogo jogo, FILE *arqResumo, char movimento) {
    fprintf(arqResumo, "Movimento %d (%c) colidiu na parede\n", jogo.movimentos, movimento);
}

void JogoGerarInicializacao(tJogo jogo, FILE *arqInicializacao) {
    MapaGerarInicializacao(jogo.mapa, jogo.pacMan, jogo.tamFantasmas, jogo.fantasmas, arqInicializacao, jogo.tunel, jogo.numTuneis);
    fprintf(arqInicializacao, "Pac-Man comecara o jogo na linha %d e coluna %d\n", PacManLinha(jogo.pacMan) + 1, PacManColuna(jogo.pacMan) + 1);
}

void JogoGerarEstatistica(tJogo jogo, FILE *arqEstatistica) {
    fprintf(arqEstatistica, "Numero de movimentos: %d\n", jogo.movimentos);
    fprintf(arqEstatistica, "Numero de movimentos sem pontuar: %d\n", jogo.movimentos - jogo.pontos);
    fprintf(arqEstatistica, "Numero de colisoes com parede: %d\n", jogo.estatistica.colisao);
    fprintf(arqEstatistica, "Numero de movimentos para baixo: %d\n", jogo.estatistica.s);
    fprintf(arqEstatistica, "Numero de movimentos para cima: %d\n", jogo.estatistica.w);
    fprintf(arqEstatistica, "Numero de movimentos para esquerda: %d\n", jogo.estatistica.a);
    fprintf(arqEstatistica, "Numero de movimentos para direita: %d\n", jogo.estatistica.d);
}
// ranking
tRanking RankingCriar(int i) {
    tRanking ranking;
    if (i == W) {
        ranking.direcao = 'w';
    } else if (i == A) {
        ranking.direcao = 'a';
    } else if (i == S) {
        ranking.direcao = 's';
    } else if (i == D) {
        ranking.direcao = 'd';
    }

    ranking.pontos = 0;
    ranking.colisao = 0;
    ranking.movimentos = 0;
    return ranking;
}

void RankingAtualizarMovimentos(tRanking rankings[4], char direcao) {
    if (direcao == 'w') {
        rankings[W].movimentos++;
    }
    if (direcao == 'a') {
        rankings[A].movimentos++;
    }
    if (direcao == 's') {
        rankings[S].movimentos++;
    }
    if (direcao == 'd') {
        rankings[D].movimentos++;
    }
}

void RankingAtualizarColisao(tRanking rankings[4], char movimento) {
    if (movimento == 'w') {
        rankings[W].colisao++;
    }
    if (movimento == 'a') {
        rankings[A].colisao++;
    }
    if (movimento == 's') {
        rankings[S].colisao++;
    }
    if (movimento == 'd') {
        rankings[D].colisao++;
    }
}

void RankingAtualizarPontos(tRanking rankings[4], char movimento) {
    if (movimento == 'w') {
        rankings[W].pontos++;
    }
    if (movimento == 'a') {
        rankings[A].pontos++;
    }
    if (movimento == 's') {
        rankings[S].pontos++;
    }
    if (movimento == 'd') {
        rankings[D].pontos++;
    }
}

void RankingGerarArquivo(int tamRanking, tRanking ranking[tamRanking], FILE *arqRanking) {
    // primeira etapa ordenar o ranking
    // o melhor movimento eh aquele que mais pontos fez
    // criterio de desempate eh o que menos colisoes teve
    // segundo criterio de desempate eh o que mais movimentos fez
    // terceiro criterio de desempate eh a ordem alfabetica
    int i, j;
    tRanking aux;
    for (i = 0; i < tamRanking - 1; i++) {
        for (j = i + 1; j < tamRanking; j++) {
            if (ranking[i].pontos < ranking[j].pontos) { // ordenar por pontos
                aux = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = aux;
            } else if (ranking[i].pontos == ranking[j].pontos) { // se os pontos forem iguais, ordenar por colisao
                if (ranking[i].colisao > ranking[j].colisao) {
                    aux = ranking[i];
                    ranking[i] = ranking[j];
                    ranking[j] = aux;
                } else if (ranking[i].colisao == ranking[j].colisao) { // se os pontos e as colisoes forem iguais, ordenar por movimentos
                    if (ranking[i].movimentos < ranking[j].movimentos) {
                        aux = ranking[i];
                        ranking[i] = ranking[j];
                        ranking[j] = aux;
                    } else if (ranking[i].movimentos ==
                               ranking[j].movimentos) { // se os pontos, as colisoes e os movimentos forem iguais, ordenar por ordem alfabetica
                        if (ranking[i].direcao > ranking[j].direcao) {
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
    for (i = 0; i < tamRanking; i++) {
        fprintf(arqRanking, "%c,%d,%d,%d\n", ranking[i].direcao, ranking[i].pontos, ranking[i].colisao, ranking[i].movimentos);
    }
}

// trilha
tTrilha TrilhaCriar(int linhas, int colunas) {
    tTrilha trilha;
    trilha.linha = linhas;
    trilha.coluna = colunas;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            trilha.matrix[i][j] = -1;
        }
    }
    return trilha;
}
tTrilha TrilhaAtualizar(tTrilha trilha, int linha, int coluna, int movimento) {
    trilha.matrix[linha][coluna] = movimento;
    return trilha;
}
void TrilhaGerarArquivo(tTrilha trilha, FILE *arqTrilha) {
    for (int i = 0; i < trilha.linha; i++) {
        for (int j = 0; j < trilha.coluna; j++) {
            if (trilha.matrix[i][j] == -1) {
                fprintf(arqTrilha, "%c", PAREDE);
            } else {
                fprintf(arqTrilha, "%d", trilha.matrix[i][j]);
            }
            if (j < trilha.coluna - 1) {
                fprintf(arqTrilha, " ");
            }
        }
        fprintf(arqTrilha, "\n");
    }
}

// estatica
tEstatistica EstatisticaCriar() {
    tEstatistica e;
    e.w = 0;
    e.a = 0;
    e.s = 0;
    e.d = 0;
    e.colisao = 0;
    return e;
}

tEstatistica EstatisticaAtualizar(tEstatistica estatistica, char direcao) {
    if (direcao == 'w') {
        estatistica.w++;
    }
    if (direcao == 'a') {
        estatistica.a++;
    }
    if (direcao == 's') {
        estatistica.s++;
    }
    if (direcao == 'd') {
        estatistica.d++;
    }
    return estatistica;
}

// bonus
tTunel TunelCriar(int linha, int coluna) {
    tTunel tunel;
    tunel.linha = linha;
    tunel.coluna = coluna;
    return tunel;
}
int EhTunel(tTunel tunel[2], int linha, int coluna) {
    if (linha == tunel[0].linha && coluna == tunel[0].coluna) {
        return 1;
    } else if (linha == tunel[1].linha && coluna == tunel[1].coluna) {
        return 1;
    } else {
        return 0;
    }
}

tTunel TunelEscolheTunelParaTeleportar(tTunel tunel1, tTunel tunel2, int linha, int coluna) {
    if (linha == tunel1.linha && coluna == tunel1.coluna) {
        return tunel2;
    } else {
        return tunel1;
    }
}

int MapaAchaTuneis(tMapa mapa, tTunel tunel[2]) {
    int i, j;
    int n = 0;
    int achou = 0;
    for (i = 0; i < mapa.linhas; i++) {
        for (j = 0; j < mapa.colunas; j++) {
            if (mapa.matriz[i][j] == TUNEL) {
                tunel[n].linha = i;
                tunel[n].coluna = j;
                n++;
                achou = 1;
            }
        }
    }
    return achou;
}

tPacMan PacManTeleportar(tPacMan pacMan, int linha, int coluna) {
    pacMan.linha = linha;
    pacMan.coluna = coluna;
    return pacMan;
}