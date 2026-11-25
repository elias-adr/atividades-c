#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int registro;
    int horas_aula;
} Professor;

void carregar_e_exibir(const char *nome_arquivo, int qtd) {
    FILE *fp = fopen(nome_arquivo, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir %s para leitura.\n", nome_arquivo);
        return;
    }

    Professor *carregados = (Professor *) malloc(qtd * sizeof(Professor));
    if (carregados == NULL) {
        printf("Erro de memoria na leitura.\n");
        fclose(fp);
        return;
    }

    size_t lidos = fread(carregados, sizeof(Professor), qtd, fp);
    fclose(fp);

    printf("\n=== PROFESSORES CARREGADOS DO ARQUIVO ===\n");
    for (size_t i = 0; i < lidos; i++) {
        printf("Registro: %d | Horas/Aula: %d\n", carregados[i].registro, carregados[i].horas_aula);
    }

    free(carregados);
}

int main() {
    int n;
    printf("Quantos professores deseja cadastrar? ");
    scanf("%d", &n);

    if (n <= 0) return 0;

    size_t total_bytes = n * sizeof(Professor);

    // Alocar array dinamico de N professores
    Professor *lista = (Professor *) malloc(total_bytes);
    // Criar buffer bruto de bytes
    void *buffer_bruto = malloc(total_bytes);

    if (lista == NULL || buffer_bruto == NULL) {
        printf("Erro ao alocar memoria!\n");
        free(lista);
        free(buffer_bruto);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Professor %d - Registro: ", i + 1);
        scanf("%d", &lista[i].registro);
        printf("Professor %d - Horas/aula: ", i + 1);
        scanf("%d", &lista[i].horas_aula);
    }

    // Copiar array inteiro para o buffer bruto
    memcpy(buffer_bruto, lista, total_bytes);

    // Gravar buffer completo com uma unica chamada fwrite()
    FILE *fp = fopen("folha.dat", "wb");
    if (fp == NULL) {
        printf("Erro ao criar folha.dat!\n");
        free(lista);
        free(buffer_bruto);
        return 1;
    }

    fwrite(buffer_bruto, total_bytes, 1, fp);
    fclose(fp);

    free(lista);
    free(buffer_bruto);

    // Ler tudo de volta em um novo array dinamico
    carregar_e_exibir("folha.dat", n);

    return 0;
}
