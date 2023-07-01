#include <stdio.h>
/*
    Trabalho de Programacao 1
    Jogo do Pacman
    Aluna: Raissa Almeida
*/

#define MAX_FANTASMAS 10
#define MAX_LINHAS 100
#define MAX_COLUNAS 100
#define DIREITA '>'
#define ESQUERDA '<'
#define CIMA '^'
#define BAIXO 'v'
#define PAREDE '#'
#define COMIDA '*'
#define VAZIO ' '


typedef struct fantasma{
    int linha;
    int coluna;
    char fantasma;
    char sentido;
}tFantasma;

/*
    Funcao: fantasma_criar
    Descricao: Cria um fantasma
    Parametros: linha - linha que o fantasma está
                coluna - coluna que o fantasma está
                fantasma - fantasma
    Retorno: fantasma - fantasma criado
*/
tFantasma fantasma_criar(int linha, int coluna, char fantasma);

/*
    Funcao: fantasma_exibir
    Descricao: Exibe o fantasma na tela
    Parametros: fantasma - fantasma
*/
void fantasma_exibir(tFantasma fantasma);

/*
    Funcao: fantasma_atualizar_posicao
    Descricao: Atualiza a posicao do fantasma no mapa
    Parametros: fantasma - fantasma
                linha - linha que o fantasma está
                coluna - coluna que o fantasma está
                sentido - sentido que o fantasma está indo
    Retorno: fantasma - fantasma atualizado
*/
tFantasma fantasma_atualizar_posicao(tFantasma fantasma, int linha, int coluna, char sentido);

/*
    Funcao: fantasma_sentido
    Descricao: Retorna o sentido que o fantasma está indo
    Parametros: fantasma - fantasma
    Retorno: sentido - sentido que o fantasma está indo
*/
char fantasma_sentido(tFantasma fantasma);

typedef struct pacMan{
    int linha;
    int coluna;
    char pacMan;
}tPacMan;

/*
    Funcao: pacMan_criar
    Descricao: Cria um pacman
    Parametros: linha - linha que o pacman está
                coluna - coluna que o pacman está
                pacMan - pacman
    Retorno: pacMan - pacman criado
*/
tPacMan pacMan_criar(int linha, int coluna, char pacMan);

/*
    Funcao: pacMan_atualizar_posicao
    Descricao: Atualiza a posicao do pacman no mapa
    Parametros: pacMan - pacman
                linha - linha que o pacman está
                coluna - coluna que o pacman está
                direcao - direcao que o pacman está indo
    Retorno: pacMan - pacman atualizado
*/
tPacMan pacMan_atualizar_posicao(tPacMan pacMan, int linha, int coluna, char direcao);

/*
    Funcao: pacMan_exibir
    Descricao: Exibe o pacman na tela
    Parametros: pacMan - pacman
*/
void pacMan_exibir(tPacMan pacMan);

typedef struct mapa{
    int linhas;
    int colunas;
    char matriz[MAX_LINHAS][MAX_COLUNAS];
}tMapa;

/*
    Funcao: mapa_carregar
    Descricao: Carrega o mapa do arquivo mapa.txt se o arquivo existir, senao encerra o programa e exibe uma mensagem de erro 
    Parametros: diretorio - pasta onde está o mapa.txt - argv[?]
    Retorno: mapa - mapa carregado
*/
tMapa mapa_carregar(char *diretorio);

/*
    Funcao: mapa_gerar_arquivo_inicializacao
    Descricao: Gera o arquivo inicializacao.txt com as informacoes do mapa e onde o pacman comeca
    Parametros: mapa - mapa carregado
                diretorio - pasta onde criará o arquivo inicializacao.txt - argv[?]
*/
void mapa_gerar_arquivo_inicializacao(tMapa mapa, char *diretorio);

/*
    Funcao: mapa_exibir
    Descricao: Exibe o mapa na tela e o pacMan e os fantasmas
    Parametros: mapa, pacMan, fantasmas e quantidade de fantasmas
*/
void mapa_exibir(tMapa mapa, tPacMan pacMan, tFantasma fantasmas[], int quantidadeFantasmas);

/*
    Funcao: mapa_celula
    Descricao: Retorna o que tem na celula do mapa
    Parametros: mapa - mapa carregado
                linha - linha que o pacman está
                coluna - coluna que o pacman está
    Retorno: celula - o que tem na celula do mapa
*/
char mapa_celula(tMapa mapa, int linha, int coluna);

typedef struct jogo{
    tMapa mapa;
    tPacMan pacMan;
    tFantasma fantasmas[MAX_FANTASMAS];
    int limiteMovimentos; // numero maximo de movimentos que o pacman pode fazer
    int movimentos; // numero de movimentos que o pacman fez
    int pontos; // numero de pontos que o pacman fez
    int vivo ; // 1 - vivo, 0 - morto
}tJogo;

/*
    Funcao: jogo_inicializar
    Descricao: Inicializa o jogo
    Parametros: diretorio - pasta onde está o mapa.txt - argv[?]
    Retorno: jogo - jogo inicializado
*/
tJogo jogo_inicializar(char *diretorio);

/*
    Funcao: jogo_ler_jogada
    Descricao: Le a jogada do usuario
    Retorno: direcao - direcao que o pacman está indo < ^ > v
*/
char jogo_ler_jogada();

/*
    Funcao: jogo_processa_jogada
    Descricao: Processa a jogada do usuario, anda com o pacman no mapa e atualiza o jogo
    Parametros: jogo - jogo inicializado
                direcao - direcao que o pacman está indo
    Retorno: jogo - jogo atualizado
*/
tJogo jogo_processa_jogada(tJogo jogo, char direcao);

/*
    Funcao: jogo_move_fantasmas
    Descricao: Move os fantasmas no mapa
    Parametros: jogo - jogo inicializado
    Retorno: jogo - jogo atualizado
*/
tJogo jogo_move_fantasmas(tJogo jogo);

/*
    Funcao: jogo_verificar_fim
    Descricao: Verifica se o jogo acabou
    Parametros: jogo - jogo inicializado
    Retorno: 1 - jogo acabou
             0 - jogo nao acabou
*/
int jogo_verificar_fim(tJogo jogo);

/*
    Funcao: jogo_exibir
    Descricao: Exibe o jogo na tela
    Parametros: jogo - jogo inicializado
*/
void jogo_exibir(tJogo jogo);

/*
    Funcao: jogo_finalizar
    Descricao: Finaliza o jogo e exive as mensagens de vitoria ou derrota
    Parametros: jogo - jogo inicializado
*/
void jogo_finalizar(tJogo jogo);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Erro: Informe o diretorio do mapa\n");
        return 1;
    }

    tJogo jogo = jogo_inicializar(argv[1]);
    while(!jogo_verificar_fim(jogo)){
        jogo_exibir(jogo);
        char direcao = jogo_ler_jogada();
        jogo = jogo_processa_jogada(jogo, direcao);
        jogo = jogo_move_fantasmas(jogo);
    }

    jogo_finalizar(jogo);

    return 0;
}