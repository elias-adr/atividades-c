#include <stdio.h>

#define TAMANHO 10

void ordenar_crescente(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void exibir_vetor(const char *titulo, int vetor[], int tamanho) {
    printf("\n%s: ", titulo);
    if (tamanho == 0) {
        printf("Nenhum valor encontrado.");
    } else {
        for (int i = 0; i < tamanho; i++) {
            printf("%d ", vetor[i]);
        }
    }
    printf("\n");
}

int main() {
    int valores[TAMANHO];
    int positivos_pares[TAMANHO];
    int negativos_impares[TAMANHO];
    int qtd_pos_pares = 0;
    int qtd_neg_impares = 0;

    for (int i = 0; i < TAMANHO; i++) {
        printf("Digite o %do valor inteiro: ", i + 1);
        scanf("%d", &valores[i]);

        if (valores[i] > 0 && valores[i] % 2 == 0) {
            positivos_pares[qtd_pos_pares++] = valores[i];
        } else if (valores[i] < 0 && valores[i] % 2 != 0) {
            negativos_impares[qtd_neg_impares++] = valores[i];
        }
    }

    ordenar_crescente(positivos_pares, qtd_pos_pares);
    ordenar_crescente(negativos_impares, qtd_neg_impares);

    exibir_vetor("Positivos e Pares (ordem crescente)", positivos_pares, qtd_pos_pares);
    exibir_vetor("Negativos e Impares (ordem crescente)", negativos_impares, qtd_neg_impares);

    return 0;
}
