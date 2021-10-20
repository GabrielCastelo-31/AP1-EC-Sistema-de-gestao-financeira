#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_CLIENTES=100;
const int MAX_CONTAS=200;
const int MAX_TRANS=1000;

// Estrutura de endereço dos clientes
// estrutura dos dados dos clientes
struct cadastro_cliente {
  int codigo,telefone,CPF_CNPJ;
  char nome[50];
} cad_cliente[MAX_CLIENTES],*cliente;

/*Função sair encerra o programa*/
void sair () {
  exit(0);
}
/*void cad_clientes() {
  printf("Informe os dados do cliente\n");
  printf("CPF/CNPJ: \n"); 
  for(int i = 0;i<MAX_CLIENTES;i++)
  scanf("%d",cliente[i].CPF_CNPJ);
  }
}*/

// Protótipo gerenciamento de clientes
void ger_clientes();
// Protótipo menu principal 
char menu(void);
  
int main(void) {
  char letra_menu;
  
  while((letra_menu = menu()) != 's' || letra_menu !='S'){
    //Se digitar C-Gerenciar Clientes
    if(letra_menu == 'C'||letra_menu == 'c'){
      /*Abre menu de gerenciamento de clintes*/
      ger_clientes(); 
    } 
    // Se digitar T- Gerenciar Contas 
    else if(letra_menu == 'T'||letra_menu == 't'){
      /*Abre menu de gerenciamento de contas*/
    } 
    // Se digitar S-sair
    else if (letra_menu == 'S'||letra_menu == 's'){
      /*Encerra o programa*/
      sair();
    } 
  }
  
  printf("teste de saída: Int main");
	return 0;
} /*Fim main*/

// Menu principal para Selecionar uma opção
char menu(void) {
  char letra_menu;
  printf("=============== Bem vindo! =================\n");
  printf("Digite um comando para prosseguir:\nC – Gerenciar Clientes\nT – Gerenciar Contas\nS – Sair\n");
  scanf("%c",&letra_menu);
  return letra_menu;
} /*Fim Menu*/
void ger_clientes(void) {
  char letra_menu;
  printf("============ Gerenciar Clientes ============\n");
  printf("Digite um comando para prosseguir:\n");
  printf("%s\n%s\n%s\n%s\n%s\n%s\n",
  "C – Cadastrar um cliente","L – Listar todos os clientes cadastrados",
  "B – Buscar cliente já cadastrado","A – Atualizar um cliente cadastrado",
  "E – Excluir um cliente cadastrado","S – Sair");
  
  scanf("%s",&letra_menu); // recebe o comando
 
 // Se for C-Cadastrar Cliente
  if (letra_menu == 'C' || letra_menu == 'c'){
  }
  //Se for L-Listar clientes
  if (letra_menu == 'L' || letra_menu == 'l'){
  }
  // Se for B-Buscar cliente
  if (letra_menu == 'B' || letra_menu == 'b'){
  }
  //Se for A-Atualizar cliente
  if (letra_menu == 'A' || letra_menu == 'a'){
  }
  //Se for E-Excluir cliente
  if (letra_menu == 'E' || letra_menu == 'e'){
  }
  //Se for S-Sair do programa
  if (letra_menu == 'S' || letra_menu == 's'){
    sair();
  }
} /*Fim ger_clientes*/
