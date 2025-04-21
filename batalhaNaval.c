#include <stdio.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define TAMANHO_HABILIDADE 5

// Inicializa o tabuleiro preenchendo com 0 (água)
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for(int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for(int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = 0;
        }
    }
}

// Exibe o tabuleiro com legendas para cada tipo de valor
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("    ");
    for (int col = 0; col < TAMANHO_TABULEIRO; col++) {
        printf("%2d ", col);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d |", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == 0)
                printf(" 0 ");  // água
            else if (tabuleiro[i][j] == 3)
                printf(" 3 ");  // navio
            else if (tabuleiro[i][j] == 5)
                printf(" 5 ");  // área de efeito
        }
        printf("\n");
    }
}

// Posiciona navios no tabuleiro (Horizontal, Vertical, Diagonal Principal e Secundária)
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, char orientacao) {
    int i;

    switch(orientacao) {
        case 'H':
            if (coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                if (tabuleiro[linha][coluna + i] != 0) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                tabuleiro[linha][coluna + i] = 3;
            break;

        case 'V':
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                if (tabuleiro[linha + i][coluna] != 0) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                tabuleiro[linha + i][coluna] = 3;
            break;

        case 'D':
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                if (tabuleiro[linha + i][coluna + i] != 0) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                tabuleiro[linha + i][coluna + i] = 3;
            break;

        case 'd':
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna - (TAMANHO_NAVIO - 1) < 0) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                if (tabuleiro[linha + i][coluna - i] != 0) return 0;
            for (i = 0; i < TAMANHO_NAVIO; i++)
                tabuleiro[linha + i][coluna - i] = 3;
            break;

        default:
            return 0;
    }

    return 1;
}

// Constrói matriz de habilidade: Cone, Cruz ou Octaedro
void construirHabilidade(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], char tipo) {
    int i, j, centro = TAMANHO_HABILIDADE / 2;

    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            habilidade[i][j] = 0;

            if (tipo == 'C') { // Cruz
                if (i == centro || j == centro) habilidade[i][j] = 1;
            }
            else if (tipo == 'O') { // Octaedro (losango)
                if (abs(i - centro) + abs(j - centro) <= centro) habilidade[i][j] = 1;
            }
            else if (tipo == 'N') { // Cone apontando para baixo
                if (i >= j - centro && i >= centro - j) habilidade[i][j] = 1;
            }
        }
    }
}

// Aplica a matriz de habilidade no tabuleiro com base no ponto de origem
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], int origemLinha, int origemColuna) {
    int i, j;
    int centro = TAMANHO_HABILIDADE / 2;

    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {

            int posLinha = origemLinha - centro + i;
            int posColuna = origemColuna - centro + j;

            if (posLinha >= 0 && posLinha < TAMANHO_TABULEIRO && posColuna >= 0 && posColuna < TAMANHO_TABULEIRO) {
                if (habilidade[i][j] == 1 && tabuleiro[posLinha][posColuna] == 0) {
                    tabuleiro[posLinha][posColuna] = 5; // Marca área de efeito
                }
            }
        }
    }
}

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

    // Inicializa o tabuleiro
    inicializarTabuleiro(tabuleiro);

    // Posiciona navios como nos desafios anteriores
    int sucesso = 1;
    sucesso &= posicionarNavio(tabuleiro, 1, 2, 'H');
    sucesso &= posicionarNavio(tabuleiro, 4, 5, 'V');
    sucesso &= posicionarNavio(tabuleiro, 6, 1, 'D');
    sucesso &= posicionarNavio(tabuleiro, 3, 8, 'd');

    if (!sucesso) {
        printf("Erro ao posicionar navios.\n");
        return 1;
    }

    // Simula uso da habilidade "Cone"
    construirHabilidade(habilidade, 'N');
    aplicarHabilidade(tabuleiro, habilidade, 2, 2);

    // Simula uso da habilidade "Cruz"
    construirHabilidade(habilidade, 'C');
    aplicarHabilidade(tabuleiro, habilidade, 7, 5);

    // Simula uso da habilidade "Octaedro"
    construirHabilidade(habilidade, 'O');
    aplicarHabilidade(tabuleiro, habilidade, 5, 5);

    // Exibe o tabuleiro final
    exibirTabuleiro(tabuleiro);

    return 0;
}
