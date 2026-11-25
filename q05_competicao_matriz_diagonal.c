#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PARTICIPANTES 4

unsigned long long fatorial(int n) {
    if (n <= 1) return 1;
    unsigned long long fat = 1;
    for (int i = 2; i <= n; i++) {
        fat *= i;
    }
    return fat;
}

unsigned long long potencia(unsigned long long base, int exp) {
    unsigned long long res = 1;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

// 4. Funcao para media ponderada (pesos 1, 2, 3...)
double calcular_media_ponderada(double valores[], int qtd) {
    double soma_ponderada = 0.0;
    int soma_pesos = 0;
    for (int j = 0; j < qtd; j++) {
        int peso = j + 1;
        soma_ponderada += valores[j] * peso;
        soma_pesos += peso;
    }
    return (soma_pesos > 0) ? (soma_ponderada / soma_pesos) : 0.0;
}

int main() {
    char nomes[PARTICIPANTES][50];
    int idades[PARTICIPANTES];
    unsigned long long ids[PARTICIPANTES];
    int qtd_valores = 0;

    // 1. Cadastro dos participantes
    for (int i = 0; i < PARTICIPANTES; i++) {
        printf("Nome do %do participante: ", i + 1);
        scanf(" %49[^\n]", nomes[i]);
        printf("Idade de %s: ", nomes[i]);
        scanf("%d", &idades[i]);
    }

    // 2. Quantidade de valores por participante e preenchimento da matriz
    do {
        printf("\nQuantidade de valores a registrar por participante: ");
        scanf("%d", &qtd_valores);
    } while (qtd_valores <= 0);

    double **matriz = malloc(PARTICIPANTES * sizeof(double *));
    for (int i = 0; i < PARTICIPANTES; i++) {
        matriz[i] = malloc(qtd_valores * sizeof(double));
    }

    double lados_retangulo[2];
    int qtd_nao_negativos = 0;

    for (int i = 0; i < PARTICIPANTES; i++) {
        printf("\n--- Valores para %s ---\n", nomes[i]);
        for (int j = 0; j < qtd_valores; j++) {
            printf("Valor %d: ", j + 1);
            scanf("%lf", &matriz[i][j]);

            // 5. Captura os dois primeiros valores nao negativos informados
            if (matriz[i][j] >= 0.0 && qtd_nao_negativos < 2) {
                lados_retangulo[qtd_nao_negativos++] = matriz[i][j];
            }
        }
    }

    // Calculo inicial dos IDs: fatorial(idade) / 2
    for (int i = 0; i < PARTICIPANTES; i++) {
        ids[i] = fatorial(idades[i]) / 2;
    }

    // Resolucao de duplicidade nos IDs
    for (int i = 0; i < PARTICIPANTES; i++) {
        int tentativas = 0;
        while (1) {
            int duplicado = 0;
            for (int k = 0; k < PARTICIPANTES; k++) {
                if (i != k && ids[i] == ids[k]) {
                    duplicado = 1;
                    break;
                }
            }
            if (!duplicado) break;

            tentativas++;
            unsigned long long base = fatorial(idades[i] / 2);
            // Aplica fatorial(idade/2)^qtd_valores e garante convergencia se idades forem iguais
            ids[i] = potencia(base, qtd_valores) + (tentativas - 1);
        }
    }

    // 3. Ordenacao alfabetica sincronizada (nome, ID e linha da matriz)
    for (int i = 0; i < PARTICIPANTES - 1; i++) {
        for (int j = 0; j < PARTICIPANTES - 1 - i; j++) {
            if (strcmp(nomes[j], nomes[j + 1]) > 0) {
                char temp_nome[50];
                strcpy(temp_nome, nomes[j]);
                strcpy(nomes[j], nomes[j + 1]);
                strcpy(nomes[j + 1], temp_nome);

                unsigned long long temp_id = ids[j];
                ids[j] = ids[j + 1];
                ids[j + 1] = temp_id;

                double *temp_linha = matriz[j];
                matriz[j] = matriz[j + 1];
                matriz[j + 1] = temp_linha;
            }
        }
    }

    // 5, 6 e 7. Diagonal do retangulo, classificacao e saida
    printf("\n=== CLASSIFICACAO FINAL ===\n");
    if (qtd_nao_negativos < 2) {
        printf("Nao foi possivel calcular a diagonal (menos de dois valores nao negativos).\n");
    } else {
        double diagonal = sqrt((lados_retangulo[0] * lados_retangulo[0]) +
                               (lados_retangulo[1] * lados_retangulo[1]));

        for (int i = 0; i < PARTICIPANTES; i++) {
            double media_p = calcular_media_ponderada(matriz[i], qtd_valores);
            const char *classificacao;

            if (fabs(media_p - diagonal) < 1e-6) {
                classificacao = "Sobre a diagonal";
            } else if (media_p > diagonal) {
                classificacao = "Acima da diagonal";
            } else {
                classificacao = "Abaixo da diagonal";
            }

            printf("%s (ID: %llu) - %s\n", nomes[i], ids[i], classificacao);
        }
    }

    for (int i = 0; i < PARTICIPANTES; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}
