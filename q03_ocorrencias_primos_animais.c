#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO 15

int eh_primo(int numero) {
  if (numero <= 1)
    return 0;
  for (int i = 2; i * i <= numero; i++) {
    if (numero % i == 0)
      return 0;
  }
  return 1;
}

int main() {
  int numeros[TAMANHO];
  int ja_processado[TAMANHO] = {0};

  for (int i = 0; i < TAMANHO; i++) {
    printf("Digite o %do numero inteiro: ", i + 1);
    scanf("%d", &numeros[i]);
  }

  // a) Identificar numeros que aparecem mais de uma vez e contar ocorrencias
  int ocorrencias[TAMANHO];
  int primeira_posicao[TAMANHO];
  int qtd_repetidos = 0;
  int soma_ocorrencias_primos_impares = 0;

  printf("\n--- Numeros que aparecem mais de uma vez ---\n");
  for (int i = 0; i < TAMANHO; i++) {
    if (ja_processado[i])
      continue;

    int cont = 0;
    for (int j = 0; j < TAMANHO; j++) {
      if (numeros[j] == numeros[i]) {
        cont++;
        ja_processado[j] = 1;
      }
    }

    if (cont > 1) {
      ocorrencias[qtd_repetidos] = cont;
      primeira_posicao[qtd_repetidos] = i;
      qtd_repetidos++;

      printf("Numero %d -> %d ocorrencias (1a posicao: %d)\n", numeros[i], cont,
             i);

      // b) Primos com quantidade impar de ocorrencias
      if (eh_primo(numeros[i]) && cont % 2 != 0) {
        soma_ocorrencias_primos_impares += cont;
      }
    }
  }

  // Cadastro dos animais conforme a soma das ocorrencias dos primos impares
  char (*animais)[50] = NULL;
  if (soma_ocorrencias_primos_impares > 0) {
    animais = malloc(soma_ocorrencias_primos_impares * sizeof(*animais));
    if (animais == NULL) {
      printf("Erro ao alocar memoria!\n");
      return 1;
    }
    printf("\nSoma das ocorrencias (primos impares): %d\n",
           soma_ocorrencias_primos_impares);
    for (int i = 0; i < soma_ocorrencias_primos_impares; i++) {
      printf("Digite o nome do %do animal: ", i + 1);
      scanf(" %49[^\n]", animais[i]);
    }
  } else {
    printf("\nNenhum primo com ocorrencia impar encontrado. Nenhum animal "
           "cadastrado.\n");
  }

  // c) e d) Correspondencia pela posicao no vetor para ocorrencias pares
  printf("\n--- Resultado das Correspondencias ---\n");
  int item_saida = 1;
  for (int i = 0; i < qtd_repetidos; i++) {
    if (ocorrencias[i] % 2 == 0) {
      int pos = primeira_posicao[i];
      if (animais != NULL && pos < soma_ocorrencias_primos_impares) {
        printf("%d. %s.\n", item_saida++, animais[pos]);
      } else {
        printf("%d. Nao existe correspondencia.\n", item_saida++);
      }
    }
  }

  if (item_saida == 1) {
    printf("Nenhum numero repetido com quantidade par de ocorrencias.\n");
  }

  free(animais);
  return 0;
}
