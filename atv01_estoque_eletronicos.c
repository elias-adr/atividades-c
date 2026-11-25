#include <stdio.h>

#define QTD_PRODUTOS 3

typedef struct {
    int codigo;
    char nome[50];
    float preco;
} Produto;

int main() {
    Produto produtos[QTD_PRODUTOS];

    // a) Receber dados via teclado
    for (int i = 0; i < QTD_PRODUTOS; i++) {
        printf("--- Produto %d ---\n", i + 1);
        printf("Codigo: ");
        scanf("%d", &produtos[i].codigo);
        printf("Nome: ");
        scanf(" %49[^\n]", produtos[i].nome);
        printf("Preco: ");
        scanf("%f", &produtos[i].preco);
    }

    // b) Salvar em estoque.txt (um produto por linha)
    FILE *fp = fopen("estoque.txt", "w");
    if (fp == NULL) {
        printf("Erro ao criar estoque.txt\n");
        return 1;
    }

    for (int i = 0; i < QTD_PRODUTOS; i++) {
        fprintf(fp, "%d;%s;%.2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco);
    }
    fclose(fp);

    // c) Reabrir para leitura e exibir formatado
    fp = fopen("estoque.txt", "r");
    if (fp == NULL) {
        printf("Erro ao abrir estoque.txt para leitura\n");
        return 1;
    }

    printf("\n=== ESTOQUE CADASTRADO ===\n");
    Produto lido;
    while (fscanf(fp, "%d;%49[^;];%f\n", &lido.codigo, lido.nome, &lido.preco) == 3) {
        printf("Codigo: %-5d | Produto: %-25s | Preco: R$ %.2f\n",
               lido.codigo, lido.nome, lido.preco);
    }

    fclose(fp);
    return 0;
}
