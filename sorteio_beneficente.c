#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nome[50];
  float preco;
} Produto;

int contador_ocorrencias(int entrada[], int distintos[], int ocorrencias[]) {
  int qtd_distintos = 0;

  for (int i = 0; i < 12; i++) {
    int ja_contado = 0;
    for (int k = 0; k < qtd_distintos; k++) {
      if (distintos[k] == entrada[i]) {
        ja_contado = 1;
        break;
      }
    }

    if (ja_contado == 0) {
      int contagem = 0;
      for (int j = 0; j < 12; j++) {
        if (entrada[j] == entrada[i]) {
          contagem++;
        }
      }
      distintos[qtd_distintos] = entrada[i];
      ocorrencias[qtd_distintos] = contagem;
      qtd_distintos++;
    }
  }

  return qtd_distintos;
}

void ordenar_decrescente(int vetor[], int tamanho) {
  for (int i = 0; i < tamanho - 1; i++) {
    for (int j = 0; j < tamanho - 1 - i; j++) {
      if (vetor[j] < vetor[j + 1]) {
        int temp = vetor[j];
        vetor[j] = vetor[j + 1];
        vetor[j + 1] = temp;
      }
    }
  }
}

void ordenar_produtos_alfabetico(Produto produtos[], int tamanho) {
  for (int i = 0; i < tamanho - 1; i++) {
    for (int j = 0; j < tamanho - 1 - i; j++) {
      if (strcmp(produtos[j].nome, produtos[j + 1].nome) > 0) {
        Produto temp = produtos[j];
        produtos[j] = produtos[j + 1];
        produtos[j + 1] = temp;
      }
    }
  }
}

int main() {
  int sorteio[12];
  int distintos[12];
  int ocorrencias[12];
  int ocorrencia_par[12];
  int qtd_pares = 0;

  for (int i = 0; i < 12; i++) {
    printf("Digite o %do numero inteiro: ", i + 1);
    scanf("%d", &sorteio[i]);
  }

  int qtd_distintos = contador_ocorrencias(sorteio, distintos, ocorrencias);

  int total_produtos = 0;
  for (int i = 0; i < qtd_distintos; i++) {
    if (ocorrencias[i] % 2 == 0) {
      ocorrencia_par[qtd_pares] = distintos[i];
      qtd_pares++;
    } else {
      if (distintos[i] > 0) {
        total_produtos += distintos[i];
      }
    }
  }

  Produto *produtos = NULL;
  if (total_produtos > 0) {
    produtos = (Produto *)malloc(total_produtos * sizeof(Produto));
    if (produtos == NULL) {
      printf("Erro ao alocar memoria!\n");
      return 1;
    }
  }

  int indice_prod = 0;
  for (int i = 0; i < qtd_distintos; i++) {
    if (ocorrencias[i] % 2 != 0 && distintos[i] > 0) {
      printf("\nNumero %d teve %d ocorrencia(s) (impar). Cadastre %d "
             "produto(s):\n",
             distintos[i], ocorrencias[i], distintos[i]);

      for (int p = 0; p < distintos[i]; p++) {
        printf("  Nome do produto %d: ", p + 1);
        scanf(" %49[^\n]", produtos[indice_prod].nome);
        printf("  Preco unitario (R$): ");
        scanf("%f", &produtos[indice_prod].preco);
        indice_prod++;
      }
    }
  }

  ordenar_decrescente(ocorrencia_par, qtd_pares);
  if (total_produtos > 0) {
    ordenar_produtos_alfabetico(produtos, total_produtos);
  }

  printf("\n=== a) NUMEROS INFORMADOS ===\n");
  for (int i = 0; i < 12; i++) {
    printf("%d ", sorteio[i]);
  }
  printf("\n");

  printf("\n=== b) OCORRENCIAS POR NUMERO DISTINTO ===\n");
  for (int i = 0; i < qtd_distintos; i++) {
    printf("Numero %d: %d ocorrencia(s)\n", distintos[i], ocorrencias[i]);
  }

  printf("\n=== c) PRODUTOS PARA COMPRA (ORDEM ALFABETICA) ===\n");
  if (total_produtos == 0) {
    printf("Nenhum produto cadastrado.\n");
  } else {
    for (int i = 0; i < total_produtos; i++) {
      printf("- %s | R$ %.2f\n", produtos[i].nome, produtos[i].preco);
    }
  }

  printf("\n=== d) NUMEROS COM OCORRENCIA PAR (DECRESCENTE) ===\n");
  if (qtd_pares == 0) {
    printf("Nenhum numero com quantidade par de ocorrencias.\n");
  } else {
    for (int i = 0; i < qtd_pares; i++) {
      printf("%d ", ocorrencia_par[i]);
    }
    printf("\n");
  }

  if (produtos != NULL) {
    free(produtos);
    produtos = NULL;
  }

  return 0;
}
