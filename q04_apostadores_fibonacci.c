#include <stdio.h>

#define TOTAL_APOSTADORES 5

// b) Soma dos N primeiros termos da sequencia de Fibonacci (1, 1, 2, 3, 5...)
long long soma_fibonacci(int termos) {
    if (termos <= 0) return 0;
    if (termos == 1) return 1;

    long long anterior = 1, atual = 1, soma = 2;
    for (int i = 3; i <= termos; i++) {
        long long proximo = anterior + atual;
        soma += proximo;
        anterior = atual;
        atual = proximo;
    }
    return soma;
}

// d) Verifica se um numero e primo
int eh_primo(long long numero) {
    if (numero <= 1) return 0;
    for (long long i = 2; i * i <= numero; i++) {
        if (numero % i == 0) return 0;
    }
    return 1;
}

int main() {
    char nomes[TOTAL_APOSTADORES][50];
    long long resultados[TOTAL_APOSTADORES];
    int premiado[TOTAL_APOSTADORES] = {0};

    // a) Cadastro dos apostadores e calculo dos resultados
    for (int i = 0; i < TOTAL_APOSTADORES; i++) {
        int valor = 0;
        printf("Nome do %do apostador: ", i + 1);
        scanf(" %49[^\n]", nomes[i]);

        do {
            printf("Valor inteiro positivo para %s: ", nomes[i]);
            scanf("%d", &valor);
        } while (valor <= 0);

        resultados[i] = soma_fibonacci(valor);
    }

    // e) 1o lugar: maior resultado primo
    int idx_primeiro = -1;
    for (int i = 0; i < TOTAL_APOSTADORES; i++) {
        if (eh_primo(resultados[i])) {
            if (idx_primeiro == -1 || resultados[i] > resultados[idx_primeiro]) {
                idx_primeiro = i;
            }
        }
    }
    if (idx_primeiro != -1) premiado[idx_primeiro] = 1;

    // f) 2o lugar: maior resultado par entre os nao premiados
    int idx_segundo = -1;
    for (int i = 0; i < TOTAL_APOSTADORES; i++) {
        if (!premiado[i] && resultados[i] % 2 == 0) {
            if (idx_segundo == -1 || resultados[i] > resultados[idx_segundo]) {
                idx_segundo = i;
            }
        }
    }
    if (idx_segundo != -1) premiado[idx_segundo] = 1;

    // g) 3o lugar: maior resultado acima da media entre os nao premiados
    double soma_total = 0.0;
    for (int i = 0; i < TOTAL_APOSTADORES; i++) {
        soma_total += resultados[i];
    }
    double media = soma_total / TOTAL_APOSTADORES;

    int idx_terceiro = -1;
    for (int i = 0; i < TOTAL_APOSTADORES; i++) {
        if (!premiado[i] && resultados[i] > media) {
            if (idx_terceiro == -1 || resultados[i] > resultados[idx_terceiro]) {
                idx_terceiro = i;
            }
        }
    }
    if (idx_terceiro != -1) premiado[idx_terceiro] = 1;

    // j) Apresentar somente os nomes dos vencedores e colocacoes
    printf("\n=== VENCEDORES ===\n");
    if (idx_primeiro != -1)
        printf("1o Lugar: %s\n", nomes[idx_primeiro]);
    else
        printf("1o Lugar: Sem vencedor\n");

    if (idx_segundo != -1)
        printf("2o Lugar: %s\n", nomes[idx_segundo]);
    else
        printf("2o Lugar: Sem vencedor\n");

    if (idx_terceiro != -1)
        printf("3o Lugar: %s\n", nomes[idx_terceiro]);
    else
        printf("3o Lugar: Sem vencedor\n");

    return 0;
}
