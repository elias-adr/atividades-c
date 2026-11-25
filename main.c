#include "Meubanco.h"
#include <stdio.h>


void menu_operacoes_banco(Banco *banco) {
  int opcao = -1;
  while (opcao != 0) {
    printf("\n--- OPERACOES DO BANCO (ABERTO) ---\n");
    printf("1. Cadastrar novo cliente\n");
    printf("2. Abrir nova conta para cliente existente\n");
    printf("3. Deposito / Saque\n");
    printf("4. Solicitar emprestimo\n");
    printf("5. Adquirir titulo de capitalizacao\n");
    printf("0. Voltar ao menu principal\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      cadastrar_cliente(banco);
      break;
    case 2:
      abrir_conta_cliente(banco);
      break;
    case 3:
      movimentar_saldo(banco);
      break;
    case 4:
      solicitar_emprestimo(banco);
      break;
    case 5:
      adquirir_titulo_capitalizacao(banco);
      break;
    case 0:
      break;
    default:
      printf("Opcao invalida.\n");
    }
  }
}

int main() {
  Banco banco;
  inicializar_banco(&banco);

  int opcao = 0;
  do {
    printf("\n====================================\n");
    if (banco.aberto == 1) {
      printf("       VISAO DO BANCO (ABERTO)\n");
    } else {
      printf("       VISAO DO BANCO (FECHADO)\n");
    }
    printf("====================================\n");
    printf("1. Abrir o banco\n");
    printf("2. Consultar historico\n");
    printf("3. Efetuar sorteio\n");
    printf("4. Sair\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      banco.aberto = 1;
      printf("\nBanco aberto com sucesso!\n");
      verificar_alertas_acesso(&banco);
      menu_operacoes_banco(&banco);
      break;

    case 2:
      if (banco.aberto == 0) {
        printf("Acesso negado: abra o banco antes de consultar o historico.\n");
      } else {
        verificar_alertas_acesso(&banco);
        consultar_historico(&banco);
      }
      break;

    case 3:
      if (banco.aberto == 0) {
        printf("Acesso negado: abra o banco antes de efetuar o sorteio.\n");
      } else {
        efetuar_sorteio_fibonacci(&banco);
      }
      break;

    case 4:
      salvar_banco(&banco);
      printf("Encerrando sistema bancario...\n");
      break;

    default:
      printf("Opcao invalida.\n");
    }
  } while (opcao != 4);

  liberar_banco(&banco);
  return 0;
}
