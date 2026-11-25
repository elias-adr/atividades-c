#include "Meubanco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Cliente *buscar_cliente_por_id(Banco *banco, int id) {
  for (int i = 0; i < banco->total_clientes; i++) {
    if (banco->clientes[i].id == id) {
      return &banco->clientes[i];
    }
  }
  return NULL;
}

Conta *buscar_conta_por_tipo(Cliente *cliente, int tipo) {
  for (int i = 0; i < cliente->qtd_contas; i++) {
    if (cliente->contas[i].tipo == tipo) {
      return &cliente->contas[i];
    }
  }
  return NULL;
}

float calcular_saldo_total_cliente(Cliente *cliente) {
  float total = 0.0;
  for (int i = 0; i < cliente->qtd_contas; i++) {
    total += cliente->contas[i].saldo;
  }
  return total;
}

unsigned long long calcular_fibonacci(int n) {
  if (n <= 0)
    return 0;
  if (n == 1 || n == 2)
    return 1;
  unsigned long long a = 1, b = 1;
  for (int i = 3; i <= n; i++) {
    unsigned long long prox = a + b;
    a = b;
    b = prox;
  }
  return b;
}

void inicializar_banco(Banco *banco) {
  banco->clientes = NULL;
  banco->total_clientes = 0;
  banco->proximo_numero_conta = 1000;
  banco->aberto = 0;
  carregar_banco(banco);
}

void liberar_banco(Banco *banco) {
  if (banco->clientes != NULL) {
    free(banco->clientes);
    banco->clientes = NULL;
  }
  banco->total_clientes = 0;
}

void salvar_banco(Banco *banco) {
  FILE *fp = fopen("banco.bin", "wb");
  if (fp == NULL) {
    printf("Erro ao abrir arquivo banco.bin!\n");
    return;
  }

  fwrite(&banco->total_clientes, sizeof(int), 1, fp);
  fwrite(&banco->proximo_numero_conta, sizeof(int), 1, fp);
  if (banco->total_clientes > 0) {
    fwrite(banco->clientes, sizeof(Cliente), banco->total_clientes, fp);
  }
  fclose(fp);
}

void carregar_banco(Banco *banco) {
  FILE *fp = fopen("banco.bin", "rb");
  if (fp == NULL) {
    return;
  }

  int total = 0, prox_num = 1000;
  if (fread(&total, sizeof(int), 1, fp) == 1 &&
      fread(&prox_num, sizeof(int), 1, fp) == 1 && total > 0) {
    Cliente *temp = (Cliente *)malloc(total * sizeof(Cliente));
    if (temp != NULL) {
      if (fread(temp, sizeof(Cliente), total, fp) == (size_t)total) {
        banco->clientes = temp;
        banco->total_clientes = total;
        banco->proximo_numero_conta = prox_num;
      } else {
        free(temp);
      }
    }
  }
  fclose(fp);
}

static const char *nome_tipo_conta(int tipo) {
  switch (tipo) {
  case 1:
    return "Conta Corrente";
  case 2:
    return "Conta Salario";
  case 3:
    return "Conta Poupanca";
  default:
    return "Conta";
  }
}

void verificar_alertas_acesso(Banco *banco) {
  for (int i = 0; i < banco->total_clientes; i++) {
    for (int j = 0; j < banco->clientes[i].qtd_contas; j++) {
      Conta c = banco->clientes[i].contas[j];
      if (c.saldo < 0.0) {
        printf("[ALERTA] Cliente %s (Conta %s - %s) esta com saldo negativo: "
               "R$ %.2f\n",
               banco->clientes[i].nome, c.numero, nome_tipo_conta(c.tipo),
               c.saldo);
      }
    }
  }
}

int adicionar_conta_interna(Banco *banco, Cliente *cliente, int tipo) {
  if (tipo < 1 || tipo > 3) {
    printf("Tipo de conta invalido.\n");
    return 0;
  }

  if (buscar_conta_por_tipo(cliente, tipo) != NULL) {
    printf("O cliente ja possui uma conta deste tipo.\n");
    return 0;
  }

  Conta *corrente = buscar_conta_por_tipo(cliente, 1);
  Conta *salario = buscar_conta_por_tipo(cliente, 2);

  if ((tipo == 1 && salario != NULL) || (tipo == 2 && corrente != NULL)) {
    printf("Operacao negada: cliente nao pode possuir conta-salario e conta "
           "corrente simultaneamente.\n");
    return 0;
  }

  if (cliente->qtd_contas >= 2) {
    printf("Limite de contas atingido para este cliente.\n");
    return 0;
  }

  Conta *nova = &cliente->contas[cliente->qtd_contas];
  sprintf(nova->numero, "%d-%02d", banco->proximo_numero_conta, tipo);
  banco->proximo_numero_conta++;
  nova->tipo = tipo;
  nova->saldo = 0.0;
  nova->tem_titulo = 0;
  strcpy(nova->historico, "Conta aberta com saldo R$ 0.00");

  cliente->qtd_contas++;
  printf("Conta %s criada com sucesso (Saldo inicial: R$ 0.00).\n",
         nova->numero);
  return 1;
}

void cadastrar_cliente(Banco *banco) {
  Cliente *temp = (Cliente *)realloc(
      banco->clientes, (banco->total_clientes + 1) * sizeof(Cliente));
  if (temp == NULL) {
    printf("Erro ao alocar memoria!\n");
    return;
  }
  banco->clientes = temp;

  Cliente *novo = &banco->clientes[banco->total_clientes];
  novo->id = banco->total_clientes + 1;
  novo->qtd_contas = 0;
  novo->emprestimo_realizado = 0;
  novo->contemplado_sorteio = 0;
  novo->premio_sorteio = 0.0;
  strcpy(novo->conta_premiada, "");

  printf("Nome do cliente: ");
  scanf(" %49[^\n]", novo->nome);

  int tipo = 0;
  printf("Tipo de conta inicial (1 - Corrente | 2 - Salario | 3 - Poupanca): ");
  scanf("%d", &tipo);

  if (adicionar_conta_interna(banco, novo, tipo) == 1) {
    banco->total_clientes++;
    salvar_banco(banco);
    printf("Cliente %s cadastrado com ID %d.\n", novo->nome, novo->id);
  }
}

void abrir_conta_cliente(Banco *banco) {
  int id, tipo;
  printf("ID do cliente: ");
  scanf("%d", &id);

  Cliente *cliente = buscar_cliente_por_id(banco, id);
  if (cliente == NULL) {
    printf("Cliente nao encontrado.\n");
    return;
  }

  printf("Tipo da nova conta (1 - Corrente | 2 - Salario | 3 - Poupanca): ");
  scanf("%d", &tipo);

  if (adicionar_conta_interna(banco, cliente, tipo) == 1) {
    salvar_banco(banco);
  }
}

void movimentar_saldo(Banco *banco) {
  int id, tipo;
  float valor;

  printf("ID do cliente: ");
  scanf("%d", &id);
  Cliente *cliente = buscar_cliente_por_id(banco, id);
  if (cliente == NULL) {
    printf("Cliente nao encontrado.\n");
    return;
  }

  printf("Tipo da conta (1 - Corrente | 2 - Salario | 3 - Poupanca): ");
  scanf("%d", &tipo);
  Conta *conta = buscar_conta_por_tipo(cliente, tipo);
  if (conta == NULL) {
    printf("O cliente nao possui este tipo de conta.\n");
    return;
  }

  printf("Valor (+ deposito / - saque): ");
  scanf("%f", &valor);

  conta->saldo += valor;
  sprintf(conta->historico, "Movimentacao: %.2f | Saldo: R$ %.2f", valor,
          conta->saldo);

  printf("Operacao realizada! Conta %s | Saldo atual: R$ %.2f\n", conta->numero,
         conta->saldo);
  if (conta->saldo < 0.0) {
    printf("[ALERTA] Atencao: sua %s entrou em saldo negativo!\n",
           nome_tipo_conta(conta->tipo));
  }

  salvar_banco(banco);
}

void solicitar_emprestimo(Banco *banco) {
  int id;
  printf("ID do cliente: ");
  scanf("%d", &id);

  Cliente *cliente = buscar_cliente_por_id(banco, id);
  if (cliente == NULL) {
    printf("Cliente nao encontrado.\n");
    return;
  }

  if (cliente->emprestimo_realizado == 1) {
    printf("Emprestimo negado: limite de 1 emprestimo por cliente ja "
           "utilizado.\n");
    return;
  }

  Conta *corrente = buscar_conta_por_tipo(cliente, 1);
  Conta *poupanca = buscar_conta_por_tipo(cliente, 3);

  if (corrente == NULL || poupanca == NULL || poupanca->saldo <= 0.0) {
    printf("Emprestimo negado: exige Conta Corrente e saldo positivo na Conta "
           "Poupanca.\n");
    return;
  }

  float saldo_total = calcular_saldo_total_cliente(cliente);
  float limite = 0.25 * (10.0 * saldo_total);

  if (limite <= 0.0) {
    printf("Emprestimo negado: saldo total insuficiente para gerar limite.\n");
    return;
  }

  float valor_solicitado;
  printf("Limite disponivel: R$ %.2f. Valor desejado: ", limite);
  scanf("%f", &valor_solicitado);

  if (valor_solicitado <= 0.0 || valor_solicitado > limite) {
    printf("Valor invalido ou acima do limite permitido.\n");
    return;
  }

  corrente->saldo += valor_solicitado;
  cliente->emprestimo_realizado = 1;
  sprintf(corrente->historico, "Emprestimo concedido: R$ %.2f",
          valor_solicitado);

  printf("Emprestimo de R$ %.2f creditado na conta %s!\n", valor_solicitado,
         corrente->numero);
  salvar_banco(banco);
}

void adquirir_titulo_capitalizacao(Banco *banco) {
  int id;
  printf("ID do cliente: ");
  scanf("%d", &id);

  Cliente *cliente = buscar_cliente_por_id(banco, id);
  if (cliente == NULL) {
    printf("Cliente nao encontrado.\n");
    return;
  }

  Conta *salario = buscar_conta_por_tipo(cliente, 2);
  if (salario == NULL) {
    printf("Negado: cliente nao possui Conta Salario.\n");
    return;
  }

  if (salario->tem_titulo == 1) {
    printf("O cliente ja possui titulo de capitalizacao ativo.\n");
    return;
  }

  if (salario->saldo < 1000.0 || salario->saldo > 1500.0) {
    printf("Negado: saldo da Conta Salario (R$ %.2f) deve estar entre R$ "
           "1.000,00 e R$ 1.500,00.\n",
           salario->saldo);
    return;
  }

  salario->tem_titulo = 1;
  strcpy(salario->historico, "Titulo de capitalizacao adquirido.");
  printf("Titulo de capitalizacao adquirido com sucesso para %s (Conta %s)!\n",
         cliente->nome, salario->numero);
  salvar_banco(banco);
}

void consultar_historico(Banco *banco) {
  float saldo_total_banco = 0.0;
  for (int i = 0; i < banco->total_clientes; i++) {
    saldo_total_banco += calcular_saldo_total_cliente(&banco->clientes[i]);
  }

  printf("\n== Saldo Total do banco ==\n");
  printf("%d | R$ %.2f\n", banco->total_clientes, saldo_total_banco);

  printf("Clientes Aptos a Emprestimos =\n");
  for (int i = 0; i < banco->total_clientes; i++) {
    Cliente *cli = &banco->clientes[i];
    Conta *corrente = buscar_conta_por_tipo(cli, 1);
    Conta *poupanca = buscar_conta_por_tipo(cli, 3);

    if (cli->emprestimo_realizado == 0 && corrente != NULL &&
        poupanca != NULL && poupanca->saldo > 0.0) {
      float limite = 0.25 * (10.0 * calcular_saldo_total_cliente(cli));
      if (limite > 0.0) {
        printf("%s %s|%.2f\n", cli->nome, corrente->numero, limite);
      }
    }
  }

  printf("== Clientes com Saldo Negativo ==\n");
  for (int i = 0; i < banco->total_clientes; i++) {
    for (int j = 0; j < banco->clientes[i].qtd_contas; j++) {
      Conta c = banco->clientes[i].contas[j];
      if (c.saldo < 0.0) {
        printf("%s %s (%s)/%.2f\n", banco->clientes[i].nome, c.numero,
               nome_tipo_conta(c.tipo), c.saldo);
      }
    }
  }

  printf("== Clientes Aptos a Compra de Titulos ==\n");
  for (int i = 0; i < banco->total_clientes; i++) {
    Conta *salario = buscar_conta_por_tipo(&banco->clientes[i], 2);
    if (salario != NULL && salario->saldo >= 1000.0 &&
        salario->saldo <= 1500.0) {
      printf("%s|%s/%.2f\n", banco->clientes[i].nome, salario->numero,
             salario->saldo);
    }
  }

  printf("= Clientes Contemplado no Sorteio =\n");
  for (int i = 0; i < banco->total_clientes; i++) {
    if (banco->clientes[i].contemplado_sorteio == 1) {
      printf("%s %s|%.2f\n", banco->clientes[i].nome,
             banco->clientes[i].conta_premiada,
             banco->clientes[i].premio_sorteio);
    }
  }
}

void efetuar_sorteio_fibonacci(Banco *banco) {
  if (banco->total_clientes == 0) {
    printf("Nenhum cliente cadastrado.\n");
    return;
  }

  int *elegiveis = (int *)malloc(banco->total_clientes * sizeof(int));
  if (elegiveis == NULL) {
    printf("Erro ao alocar memoria!\n");
    return;
  }

  int qtd_elegiveis = 0;
  for (int i = 0; i < banco->total_clientes; i++) {
    for (int j = 0; j < banco->clientes[i].qtd_contas; j++) {
      if (banco->clientes[i].contas[j].tem_titulo == 1) {
        elegiveis[qtd_elegiveis] = i;
        qtd_elegiveis++;
        break;
      }
    }
  }

  if (qtd_elegiveis == 0) {
    printf(
        "Nenhum cliente apto (com titulo de capitalizacao) para o sorteio.\n");
    free(elegiveis);
    return;
  }

  for (int i = 0; i < qtd_elegiveis - 1; i++) {
    for (int j = 0; j < qtd_elegiveis - 1 - i; j++) {
      float s1 = calcular_saldo_total_cliente(&banco->clientes[elegiveis[j]]);
      float s2 =
          calcular_saldo_total_cliente(&banco->clientes[elegiveis[j + 1]]);
      if (s1 < s2) {
        int tmp = elegiveis[j];
        elegiveis[j] = elegiveis[j + 1];
        elegiveis[j + 1] = tmp;
      }
    }
  }

  int participantes = qtd_elegiveis;
  if (participantes > 5) {
    participantes = 5;
  }

  int semente;
  printf("Sorteio entre os %d maiores saldos com titulo. Informe o termo N de "
         "Fibonacci: ",
         participantes);
  scanf("%d", &semente);

  unsigned long long fib = calcular_fibonacci(semente);
  int indice_top = (int)(fib % (unsigned long long)participantes);
  Cliente *vencedor = &banco->clientes[elegiveis[indice_top]];

  for (int j = 0; j < vencedor->qtd_contas; j++) {
    if (vencedor->contas[j].tem_titulo == 1) {
      float premio = vencedor->contas[j].saldo * 10.0;
      if (premio < 0.0) {
        premio = 0.0;
      }
      vencedor->contas[j].saldo += premio;
      vencedor->contemplado_sorteio = 1;
      vencedor->premio_sorteio = premio;
      strcpy(vencedor->conta_premiada, vencedor->contas[j].numero);
      sprintf(vencedor->contas[j].historico,
              "Contemplado no sorteio! Premio: R$ %.2f", premio);

      printf("\nVENCEDOR DO SORTEIO: %s | Conta: %s | Premio: R$ %.2f\n",
             vencedor->nome, vencedor->conta_premiada, premio);
      break;
    }
  }

  salvar_banco(banco);
  free(elegiveis);
}
