#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char cargo[30];
    float salario;
} Funcionario;

int main() {
    // a) Alocar dinamicamente o registro original
    Funcionario *original = (Funcionario *) malloc(sizeof(Funcionario));
    // b) Alocar dinamicamente o buffer de backup
    Funcionario *backup = (Funcionario *) malloc(sizeof(Funcionario));

    if (original == NULL || backup == NULL) {
        printf("Erro ao alocar memoria!\n");
        free(original);
        free(backup);
        return 1;
    }

    printf("ID do funcionario: ");
    scanf("%d", &original->id);
    printf("Cargo: ");
    scanf(" %29[^\n]", original->cargo);
    printf("Salario: ");
    scanf("%f", &original->salario);

    // c) Espelhar dados usando memcpy()
    memcpy(backup, original, sizeof(Funcionario));

    // d) Salvar buffer de backup em arquivo binario
    FILE *fp = fopen("backup_rh.bin", "wb");
    if (fp == NULL) {
        printf("Erro ao abrir backup_rh.bin!\n");
        free(original);
        free(backup);
        return 1;
    }

    fwrite(backup, sizeof(Funcionario), 1, fp);
    fclose(fp);

    printf("Backup gravado com sucesso em backup_rh.bin!\n");

    free(original);
    free(backup);
    return 0;
}
