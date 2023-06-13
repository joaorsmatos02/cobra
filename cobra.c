#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

enum dir {
    cima, 
    baixo, 
    esquerda, 
    direita
}direcao;

struct bloco {
    int x;
    int y;
};

HANDLE semaforo;
struct bloco* cobra;
char tabuleiro[9][18];
enum dir fila[100];
int elem_fila;
int ponteiro_fila;
int tamanho;

DWORD WINAPI thread_interacao(LPVOID arg) {
    int dir;
    int seta;
    while(tamanho != 0) {
        dir = getch();
        switch(dir) {
            case 224: // seta
                seta = getch();
                switch(seta) {
                    case 72:
                        ReleaseSemaphore(semaforo, 1, NULL);
                        direcao = cima;
                        break;
                    case 80:
                        ReleaseSemaphore(semaforo, 1, NULL);
                        direcao = baixo;
                        break;
                    case 75:
                        ReleaseSemaphore(semaforo, 1, NULL);
                        direcao = esquerda;
                        break;
                    case 77:
                        ReleaseSemaphore(semaforo, 1, NULL);
                        direcao = direita;
                        break;
                }
                break;
            case 119:
                ReleaseSemaphore(semaforo, 1, NULL);
                direcao = cima;
                break;
            case 115:
                ReleaseSemaphore(semaforo, 1, NULL);
                direcao = baixo;
                break;
            case 97:
                ReleaseSemaphore(semaforo, 1, NULL);
                direcao = esquerda;
                break;
            case 100:
                ReleaseSemaphore(semaforo, 1, NULL);
                direcao = direita;
                break;
        }
    }
}

void imprime_tempo() {
    time_t atual;
    time(&atual);
    struct tm aulas;
    aulas.tm_year = 122;
    aulas.tm_mon = 8;
    aulas.tm_mday = 20;
    aulas.tm_hour = 0;
    aulas.tm_min = 0;
    aulas.tm_sec = 0;
    time_t tempoaulas = mktime(&aulas);
    int diferenca = difftime(tempoaulas, atual);
    if (diferenca <= 0)
        printf("Voltamos a RURS!");
    else {
        srand(atual);
        int num = rand() % 4;
        switch(num){
            case 0:
                printf("Faltam %d segundos para voltarmos resi!\n", diferenca);
                break;
            case 1:
                printf("Faltam %d minutos para voltarmos resi!\n", diferenca/60);
                break;
            case 2:
                printf("Faltam %d horas para voltarmos resi!\n", (diferenca/60)/60);
                break;
            default:
                printf("Faltam %d dias para voltarmos resi!\n", ((diferenca/60)/60)/24);
        }
    }
}

void encerra_jogo() {
    printf("Perdeu! Tamanho atingido: %d\n", tamanho);
    imprime_tempo();
    tamanho = 0;
    free(cobra);
    getch(); // esperar ate primir tecla
    exit(0);
}

void imprime_tabuleiro() {
    system("cls");
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 18; j++)
            printf("%c", tabuleiro[i][j]);
        printf("\n");
    }
}

void gera_ponto() {
    int x;
    int y;
    srand(time(NULL));
    do {
        x = rand() % 9;
        y = rand() % 18;
    } while (tabuleiro[x][y] != '.');
    tabuleiro[x][y] = '*';
}

void move_cobra() {
    switch (direcao) {
        case cima:
            cobra->x--;
            fila[elem_fila] = cima;
            break;
        case baixo:
            cobra->x++;
            fila[elem_fila] = baixo;
            break;
        case esquerda:
            cobra->y--;
            fila[elem_fila] = esquerda;
            break;
        case direita:
            cobra->y++;
            fila[elem_fila] = direita;
            break;
    }
    elem_fila = (elem_fila + 1) % 100;
    // verificar se pos e valida
    if(tabuleiro[cobra->x][cobra->y] == 'X' || cobra->x < 0 || cobra->x > 8 || cobra->y < 0 || cobra->y > 17)
        encerra_jogo();
    else if(!(tabuleiro[cobra->x][cobra->y] == '*')) { // se nao cresce
        tabuleiro[(cobra+1)->x][(cobra+1)->y] = '.';
        if(fila[ponteiro_fila] == cima)
            (cobra+1)->x--;
        else if(fila[ponteiro_fila] == baixo)
            (cobra+1)->x++;
        else if(fila[ponteiro_fila] == esquerda)
            (cobra+1)->y--;
        else if (fila[ponteiro_fila] == direita)
            (cobra+1)->y++;
        ponteiro_fila = (ponteiro_fila + 1) % 100;
    } else if (tabuleiro[cobra->x][cobra->y] == '*'){ // se cresce
        tamanho++;
        gera_ponto();
    }
    tabuleiro[cobra->x][cobra->y] = 'X'; // posicionar cabeca
}

void executa_jogo() {
    gera_ponto();
    imprime_tabuleiro();
    WaitForSingleObject(semaforo, INFINITE);
    while(1) {
        Sleep(300);
        move_cobra();
        imprime_tabuleiro();
    }
}

int main() {  
    semaforo = CreateSemaphore(NULL, 0, 1, NULL);
    HANDLE thread = CreateThread(NULL, 0, thread_interacao, NULL, 0, NULL);
    cobra = malloc(2*sizeof(struct bloco));
    tamanho = 1;
    elem_fila = 0;
    ponteiro_fila = 0;
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 18; j++)
            tabuleiro[i][j] = '.';
    tabuleiro[4][9] = 'X';
    cobra->x = 4;
    cobra->y = 9;
    (cobra+1)->x = 4;
    (cobra+1)->y = 9;
    executa_jogo();
    return 0;
}