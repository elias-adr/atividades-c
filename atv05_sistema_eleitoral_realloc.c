#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char titulo[12];
    int zona;
} Eleitor;

typedef struct {
    Eleitor eleitor;
    int numero_voto;
} Cedula;

// a) Adicionar votos dinamicamente usando realloc
Cedula *adicionar_voto(Cedula *urna, int *total_votos) {
    Cedula nova;
    printf("Titulo do eleitor: ");
    scanf(" %11s", nova.eleitor.titulo);
    printf("Zona eleitoral: ");
    scanf("%d", &nova.eleitor.zona);
    printf("Numero do voto: ");
    scanf("%d", &nova.numero_voto);

    Cedula *temp = (Cedula *) realloc(urna, (*total_votos + 1) * sizeof(Cedula));
    if (temp == NULL) {
        printf("Erro ao realocar memoria!\n");
        return urna;
    }

    temp[*total_votos] = nova;
    (*total_votos)++;
    return temp;
}

// c) Recuperacao rapida usando fseek e ftell
void recuperar_urnas(const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir %s.\n", nome_arquivo);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long tamanho_bytes = ftell(fp);
    rewind(fp);

    if (tamanho_bytes <= 0) {
        printf("Arquivo vazio.\n");
        fclose(fp);
        return;
    }

    int qtd_registros = tamanho_bytes / sizeof(Cedula);
    Cedula *recuperadas = (Cedula *) malloc(tamanho_bytes);
    if (recuperadas == NULL) {
        printf("Erro ao alocar memoria para recuperacao.\n");
        fclose(fp);
        return;
    }

    fread(recuperadas, sizeof(Cedula), qtd_registros, fp);
    fclose(fp);

    printf("\n=== VOTOS RECUPERADOS (%d registros) ===\n", qtd_registros);
    for (int i = 0; i < qtd_registros; i++) {
        printf("Titulo: %-12s | Zona: %-4d | Voto: %d\n",
               recuperadas[i].eleitor.titulo,
               recuperadas[i].eleitor.zona,
               recuperadas[i].numero_voto);
    }

    free(recuperadas);
}

int main() {
    Cedula *urna = NULL;
    int total_votos = 0;
    char continuar;

    do {
        urna = adicionar_voto(urna, &total_votos);
        printf("Registrar outro voto? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    // b) Salvar lista completa em urnas.bin
    FILE *fp = fopen("urnas.bin", "wb");
    if (fp != NULL) {
        fwrite(urna, sizeof(Cedula), total_votos, fp);
        fclose(fp);
    }

    free(urna);

    // Recuperar tudo de uma vez
    recuperar_urnas("urnas.bin");

    return 0;
}
