#ifndef MEUBANCO_H
#define MEUBANCO_H

typedef struct {
  char numero[15];
  int tipo;
  float saldo;
  int tem_titulo;
  char historico[200];
} Conta;

typedef struct {
  int id;
  char nome[50];
  Conta contas[2];
  int qtd_contas;
  int emprestimo_realizado;
  int contemplado_sorteio;
  float premio_sorteio;
  char conta_premiada[15];
} Cliente;

typedef struct {
  Cliente *clientes;
  int total_clientes;
  int proximo_numero_conta;
  int aberto;
} Banco;

void inicializar_banco(Banco *banco);
void liberar_banco(Banco *banco);
void salvar_banco(Banco *banco);
void carregar_banco(Banco *banco);

void cadastrar_cliente(Banco *banco);
void abrir_conta_cliente(Banco *banco);
void movimentar_saldo(Banco *banco);
void solicitar_emprestimo(Banco *banco);
void adquirir_titulo_capitalizacao(Banco *banco);
void verificar_alertas_acesso(Banco *banco);

void consultar_historico(Banco *banco);
void efetuar_sorteio_fibonacci(Banco *banco);

#endif
