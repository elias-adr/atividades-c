#include <stdio.h>

typedef struct {
    int conta_corrente;
    char nome[50];
    float saldo;
} Cliente;

int main() {
    int conta_busca;
    float valor_operacao;

    // a) Solicitar conta e valor (positivo para deposito, negativo para saque)
    printf("Numero da conta corrente: ");
    scanf("%d", &conta_busca);
    printf("Valor da operacao (+ deposito / - saque): ");
    scanf("%f", &valor_operacao);

    // b) Abrir arquivo binario em modo leitura/escrita ("rb+")
    FILE *fp = fopen("clientes.bin", "rb+");
    if (fp == NULL) {
        printf("Erro ao abrir clientes.bin (verifique se o arquivo existe).\n");
        return 1;
    }

    Cliente c;
    int encontrado = 0;

    // c) Ler registro por registro
    while (fread(&c, sizeof(Cliente), 1, fp) == 1) {
        if (c.conta_corrente == conta_busca) {
            c.saldo += valor_operacao;

            // d) Voltar o ponteiro do arquivo para o inicio deste registro e sobrescrever
            fseek(fp, -((long)sizeof(Cliente)), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, fp);

            printf("Saldo atualizado! Titular: %s | Novo saldo: R$ %.2f\n", c.nome, c.saldo);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Conta %d nao encontrada.\n", conta_busca);
    }

    fclose(fp);
    return 0;
}
