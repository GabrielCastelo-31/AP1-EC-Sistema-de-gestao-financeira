/*
Aluno: Gabriel Henrique Castelo Costa
Curso: Engenharia de Computação
Professores:  Ronaldo Martins da Costa e Guilherme Silva Marques
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#pragma warning(disable : 4996)

const int MAX_CLIENTES = 100;
const int MAX_CONTAS = 200;
const int MAX_TRANS = 1000;

/* Estrutura de endereço dos clientes*/
struct endereco
{
    int CEP, numero;
    char estado[3], cidade[20], rua[20];
};
typedef struct endereco endereco;

/* Estrutura dos dados dos clientes*/
struct cad_cliente
{
    int codigo;
    char nome[40], CPF[12], CNPJ[15], telefone[13]; /*CPF tem 11 numeros e CNPJ tem 14*/
    endereco end;
};
typedef struct cad_cliente cad_cliente;

/*Estrutura de conta*/
struct CONTA
{
    int agencia;
    int num_conta;
    double saldo;
    cad_cliente cliente;
};
typedef struct CONTA CONTA;

/*Estrutura de transferencia*/
typedef struct RegTransferencia
{
    double valor;
    char descricao[100];
    int dia,mes,ano;
    CONTA conta;
}RegTransferencia;

/*Função sair encerra o programa*/
void sair()
{
    exit(0);
}

/*Função para manter mensagens na tela*/
void mysleep()
{
    /*Funciona em LINUX e WINDOWS*/
#ifdef _WIN32
    Sleep(3000);
#else
    sleep(3);
#endif
} /*Fim mysleep*/

/*Função para limpar a tela*/
void clear()
{
    /*Funciona em LINUX e WINDOWS*/
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
} /*Fim clear*/

char menu(); /* Protótipo MENU PRINCIPAL */

void Sair_Ou_Menu(); /*Protótipo para sair do aplicativo ou voltar para o menu principal*/

/*Protótipos de Gerenciamento DE CLIENTE*/
void GerenciarCLIENTES();                                                                                   /* Protótipo MENU DE CLIENTES */
void CadastroCLIENTE();                                                                                    /*Protótipo de CADASTRO DE CLIENTES*/
int RecebeDadosPessoaFISICA(FILE* cli, cad_cliente cliente, endereco end);                                 /*Protótipo recebe dados Pessoa FISICA*/
int RecebeDadosPessoaJURIDICA(FILE* cli, cad_cliente cliente, endereco end);                               /*Prototipo recebe dados pessoa JURIDICA*/
int VerificaCodigo(int);                                                                                   /*Protipo para VERIFICAR CODIGO*/
int VerificaCadastro(FILE* cli, cad_cliente cliente, int, char[]);                                         /*Prototipo para VERIFICAR EXISTENCIA DE CADASTRO*/
void ListarCLIENTES();                                                                                     /*Prototipo pra LISTAR CLIENTES*/
void BuscaCLIENTE();                                                                                       /*Prototipo para BUSCAR CLIENTES*/
void AtualizaCLIENTE();                                                                                    /*Prototipo para ATUALIZAR CLIENTES*/
int AtualizaDadosCLIENTE(FILE* cli, cad_cliente cliente, endereco end, int, char[12], char[15], char[40]); /*Protitpo para RECEBER DADOS A SEREM ATUALIZADOS */
void ExcluiCLIENTE();
void ImprimeDadosCLIENTE(cad_cliente cliente, endereco end);

/*Protótipos de Gerenciamento de CONTAS*/
void GerenciarCONTAS();
void CadastroCONTA();
int VerificaCONTA(int, int, FILE* acct, CONTA conta);
void ImprimeDadosCONTA(CONTA conta);
void ListaContaCliente();
void ListaTodasContas();
int VerificaSaldo(CONTA conta, int valor);
void Saque();
void Deposito();
void Transferencia();
void Extrato();
void Debito(CONTA conta, double valor, char descricao[100]);
void Credito(CONTA conta, double valor, char descricao[100]);

/*Função principal*/
int main(void)
{
    /*Permite uso de acentos da lingua portguesa*/
    setlocale(LC_ALL, "Portuguese");

    /*Incializa as structs  como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "","", end };
    char op_menu = ' ';
    FILE* cli;
    /*Laço para uso dos menus*/
    do
    {
        clear();
        op_menu = menu();
        setbuf(stdin, NULL);

        /*Abre menu de gerenciamento de clintes*/
        if (op_menu == 'C' || op_menu == 'c')
        {
            GerenciarCLIENTES();
            continue;
        }
        /*Abre menu de gerenciamento de contas*/
        else if (op_menu == 'T' || op_menu == 't')
        {
            GerenciarCONTAS();
            continue;
        }
        /*Encerra o programa*/
        else if (op_menu == 'S' || op_menu == 's')
        {
            break;
        }
        /*Se digitar um comando inválido*/
        else if (((op_menu != 'S' && op_menu != 's') && (op_menu != 'T' && op_menu != 't') && (op_menu != 'C' && op_menu != 'c')))
        {
            clear();
            printf("Comando inválido!\n");
            mysleep();
            continue;
        }
    } while (1);

    //printf("teste de saída: int main");
    sair();
    return 0;
}
/*Fim main*/

/*Menu PRINCIPAL*/
char menu(void)
{
    char op_menu = ' ';
    printf("=============== Bem vindo! =================\n");
    printf("Digite um comando para prosseguir:\n");
    printf("C – Gerenciar Clientes\nT – Gerenciar Contas\nS – Sair\n");
    //printf("Comando:");
    setbuf(stdin, NULL);
    scanf(" %c", &op_menu);
    clear();
    return op_menu;
} /*Fim Menu*/
/*Fim menu*/

void Sair_Ou_Menu()
{
    char op;
    clear();
    printf("Deseja sair do aplicativo ou voltar para o menu principal?\n");
    printf("%s\n%s\n", "Sair do aplicativo - S", "Voltar para o menu principal - M");
    setbuf(stdin, NULL);
    scanf(" %c", &op);
    clear();
    /*Opção S = Encerra o programa*/
    if (op == 'S' || op == 's')
    {
        sair();
    }
    /*Opção M = Voltar para o menu principal*/
    else if (op == 'M' || op == 'm')
    {
    }
    /*Comando inválido*/
    else
    {
        printf("Comando inválido!\n");
        mysleep();
        clear();
        Sair_Ou_Menu();
    }
}
/*Fim Sair_Ou_Menu*/

/*INÍCIO FUNÇÕES DE CLIENTE*/

/*Menu DE CELINTES*/
void GerenciarCLIENTES()
{
    char op;
    do {
        printf("============ Gerenciar Clientes ============\n");
        printf("Digite um comando para prosseguir:\n");
        printf("%s\n%s\n%s\n%s\n%s\n%s\n",
            "C – Cadastrar um cliente", "L – Listar todos os clientes cadastrados",
            "B – Buscar cliente já cadastrado", "A – Atualizar um cliente cadastrado",
            "E – Excluir um cliente cadastrado", "S – Sair");
        setbuf(stdin, NULL);
        scanf(" %c", &op); // recebe o comando
        clear();
        switch (op)
        {
        case 'C':
        case 'c':
            CadastroCLIENTE();
            break;

        case 'L':
        case 'l':
            ListarCLIENTES();
            break;

        case 'B':
        case 'b':
            BuscaCLIENTE();

            break;

        case 'A':
        case 'a':
            AtualizaCLIENTE();
            break;

        case 'E':
        case 'e':
            ExcluiCLIENTE();
            break;

        case 'S':
        case 's':
            Sair_Ou_Menu();
            break;

        default:
            clear();
            printf("Comando inválido!\n");
            mysleep();
            clear();
            //GerenciarCLIENTES();
        }
    } while ((op != 'C' && op != 'c') && (op != 'L' && op != 'l')
        && (op != 'A' && op != 'a') && (op != 'B' && op != 'b')
        && (op != 'E' && op != 'e') && (op != 'S' && op != 's'));
} /*Fim ger_clientes*/

void CadastroCLIENTE()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "","", end };

    int cont_cliente = 0;       /*Conta quantiade clientes*/
    int codigo = 0;             /*Código a ser recibido*/
    int verCAD = 0, verCOD = 0; /*Controle da função VerificaCadastro e VerificaCodigo*/
    char op = ' ';              /*Opção para continuar cadastro*/
    char CPF[11], CNPJ[14];     /*CPF e CNPJ a serem cadastrados*/

    /*Abre arquivo para atualização e leitura em modo binário*/
    FILE* cli = fopen("clientes.dat", "rb+");

    if (cli == NULL) /*Se arquivo não existir, cria um*/
    {
        cli = fopen("clientes.dat", "wb+");

        /*Fecha caso não seja possível abrir o arquivo*/
        if (cli == NULL)
        {
            printf("Não foi possível abrir o arquivo");
            exit(1);
        }
        else {/*Incialiaza o arquivo com 100 posições em branco*/
            for (int i = 0; i < MAX_CLIENTES; i++)
            {
                fwrite(&cliente, sizeof(cad_cliente), 1, cli);
            }
            /*Fecha o arquivo para salvar as posições*/
            fclose(cli);
            /*Abre novamente para leitura e gravação*/
            cli = fopen("clientes.dat", "rb+");
        }
    }
    /*Laço para cadastro*/
    do
    {
        /*Garante que as structs serão passadas vazias para as funções de cadastro*/
        endereco end = { 0, 0, "", "", "" };
        cad_cliente cliente = { 0, "", "", "","", end };

        clear();

        /*Escolher entre pessoa física ou jurídica*/
        printf("=============== Cadastrar Cliente =================\n");
        printf("Deseja cadastrar Pessoa Física ou Pessoa Júrdica?\n");
        printf("%s\n%s\n%s\n", "Pessoa FÍSICA - F", "Pessoa JURÍDICA - J", "Sair - S");
        setbuf(stdin, NULL);
        scanf(" %c", &op);
        clear();

        if (op == 'F' || op == 'f') /* Opção Cadastrar Pessoa FÍSICA*/
        {
            /*Limpa a opção para evitar erros*/
            op = ' ';

            /*Laço para cadastro do cliente*/
            do
            {
                verCOD = RecebeDadosPessoaFISICA(cli, cliente, end);
                verCAD = verCOD;
                if (verCOD == 1) /*Caso código seja inválido, pede para digitar de novo*/
                {
                    //printf("verCOD == 1, Codigo inválido\n");
                    mysleep();
                    clear();
                    continue;
                }
                else /*Caso código seja válido, prossegue com o cadastro*/
                {
                    if (verCAD == 2) /*Cliente já existente. */
                    {
                        //printf("verCAD == 2. Cliente já existente!\n");
                        mysleep();
                        break;
                    }
                    else if (verCAD == 0) /*Retorno de RecebeDadosPessoa = 0, cadastro realizado */
                    {
                        printf("Cadastro realizado com sucesso!\n");
                        mysleep();
                        clear();
                        break;
                    }
                }
            } while (1); /*Fim do.. while*/
        }/*Fim cadastar pessoa física*/

        else if (op == 'J' || op == 'j') /*Opção cadastrar Pessoa JURÍDICA*/
        {
            /*Limpa a opção para evitar erros*/
            op = ' ';

            /*Laço para cadastro do cliente*/
            do
            {
                verCOD = RecebeDadosPessoaJURIDICA(cli, cliente, end);
                verCAD = verCOD;
                if (verCOD == 1) /*Caso código seja inválido, pede para digitar de novo*/
                {
                    //printf("verCOD == 1, Codigo inválido\n");
                    mysleep();
                    clear();
                    continue;
                }
                else /*Caso código seja válido, prossegue com o cadastro*/
                {
                    if (verCAD == 2) /*Cliente já existente. */
                    {
                        //printf("verCAD == 2. Cliente já existente!\n");
                        mysleep();
                        break;
                    }
                    else if (verCAD == 0) /*Retorno de RecebeDadosPessoa = 0, cadastro realizado */
                    {
                        printf("Cadastro realizado com sucesso!\n");
                        mysleep();
                        clear();
                        break;
                    }
                }
            } while (1); /*Fim do.. while*/
        }/*Fim cadastrar pessoa jurídica*/

        else if (op == 'S' || op == 's') /*Opção SAIR*/
        {
            Sair_Ou_Menu();
        }
        /*Incrementa a quantidade de clientes cadastrados
            Máximo permitido = 100*/
        if ((op != 'S' && op != 's') && (verCOD != 1 && verCAD != 2))
        {
            cont_cliente++;
        }
    } while ((op != 'S' && op != 's') && (cont_cliente >= 0 && cont_cliente <= MAX_CLIENTES)); /*Fim do...while*/

    /*Quantidade máxima de clientes a serem cadastrados*/
    if (cont_cliente == MAX_CLIENTES)
    {
        printf("Quantidade máxima de clientes atingida!\n");
    }

    fclose(cli); /*Fecha o arquivo e salva os dados cadastrados*/
}
/*Fim cadastro_CLIENTE*/

/*Verifica se o código do cliente está entre 1 e 100*/
int VerificaCodigo(int codigo)
{
    if (codigo < 1 || codigo > MAX_CLIENTES)
    {
        clear();
        printf("Código inválido! Digite um valor entre 1 e %d.\n", MAX_CLIENTES);
        mysleep();
        clear();
        return 1; /* 1 Representa código INVÁLIDO*/
    }
    else
    {
        return 0; /* 0 Representa código VÁLIDO*/
    }
}
/*Fim VerificaCodigo*/

/*Verifica se o cliente já possui cadastro*/
int VerificaCadastro(FILE* cli, cad_cliente cliente, int codigo, char CPF_CNPJ[15])
{
    /*Posiciona o ponteiro na posição do arquivo referente ao código*/
    fseek(cli, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Lê os dados existentes no arquivo clientes.dat nessa posição*/
    fread(&cliente, sizeof(cad_cliente), 1, cli);

    /*Se houver algum cliente com o mesmo código, CPF ou CNPJ ação inválida*/
    if (cliente.codigo != 0 || (strcmp(cliente.CPF, CPF_CNPJ) == 0) || (strcmp(cliente.CNPJ, CPF_CNPJ) == 0))
    {
        clear();
        printf("Cliente já cadastrado! Por favor, verifique os dados e tente novamente.\n");
        mysleep();
        clear();
        return 1; /* 1 Representa cliente já cadastrado*/
    }
    else /*Verifica se existe algum outro cliente com o CPF ou CNPJ já cadastrado*/
    {
        rewind(cli);
        while (!feof(cli)) /*Varre todo o arquivo comparando o CPF ou CNPJ*/
        {
            fread(&cliente, sizeof(cad_cliente), 1, cli);

            if ((strcmp(cliente.CPF, CPF_CNPJ) == 0) || (strcmp(cliente.CNPJ, CPF_CNPJ) == 0))
            {
                clear();
                /*Se o CPF existir*/
                if ((strcmp(cliente.CPF, CPF_CNPJ) == 0))
                {
                    printf("CPF já cadastrado! Por favor, verifique os dados e tente novamente.\n");
                }
                /*Se o CNPJ existir*/
                else if ((strcmp(cliente.CNPJ, CPF_CNPJ) == 0))
                {
                    printf("CNPJ já cadastrado! Por favor, verifique os dados e tente novamente.\n");
                }
                mysleep();
                clear();
                return 1; /* 1 Representa cliente já cadastrado*/
                break;
            }
        }

    }

    return 0; /* 0 representa que o cadastro PODE PROSSEGUIR*/
}
/*Fim VerificaCadastro*/

int RecebeDadosPessoaFISICA(FILE* cli, cad_cliente cliente, endereco end)
{
    int codigo, verCOD, verCAD;
    char CPF[12];

    printf("=============== Cadastrar Pessoa FÍSICA =================\n");
    printf("%s\n", "Digite o código do cliente (ENTRE 1 E 100):");
    setbuf(stdin, NULL);
    scanf("%d", &codigo);
    verCOD = VerificaCodigo(codigo); /*Recebe o código e verifica se codigo esta entre 1 e 100*/

    if (verCOD != 0) /*Caso código seja inválido, retorna e pede para tentar novamente*/
    {
        return 1;
    }
    else /*Caso código seja válido, prossegue com o cadastro*/
    {
        /*Recebe o CPF do cliente*/
        printf("%s\n%s\n", "Formato CPF: SOMENTE NÚMEROS!", "Digite o CPF do cliente:");
        setbuf(stdin, NULL);
        scanf(" %12[^\n]", CPF);

        /*Verifica a exitencia de cliente com mesmo codigo ou cpf*/
        verCAD = VerificaCadastro(cli, cliente, codigo, CPF);

        if (verCAD != 0)/*Retorno de VerificaCadastro == 1: Cliente já existente*/
        {
            return 2;
        }
        else if (verCAD == 0)  /*Continua com o cadastro*/
        {
            /*Atribui os valores de codigo e CPF recebidos ao cliente*/
            cliente.codigo = codigo;
            strcpy(cliente.CPF, CPF);

            /*Recebe o nome do cliente*/
            printf("%s\n", "Digite o nome do cliente:");
            setbuf(stdin, NULL);
            scanf(" %40[^\n]", cliente.nome);

            /*Recebe o numero de telefone*/
            printf("%s\n", "Digite o numero de telefone:");
            setbuf(stdin, NULL);
            scanf(" %11[^\n]", cliente.telefone);

            /*Recebe o endereço*/

            /*Recebe o CEP*/
            printf("%s\n", "CEP: SOMENTE NÚMEROS");
            setbuf(stdin, NULL);
            scanf(" %d", &cliente.end.CEP);

            /*Recebe o estado*/
            printf("%s\n", "Estado (No Formato: SP,GO):");
            setbuf(stdin, NULL);
            scanf(" %3[^\n]", cliente.end.estado);

            /*Recebe o município*/
            printf("%s\n", "Município:");
            setbuf(stdin, NULL);
            scanf(" %20[^\n]", cliente.end.cidade);

            /*Recebe a rua*/
            printf("%s\n", "Rua:");
            setbuf(stdin, NULL);
            scanf(" %20[^\n]", cliente.end.rua);

            /*Recebe o número*/
            printf("Número:\n");
            setbuf(stdin, NULL);
            scanf(" %d", &cliente.end.numero);

            clear();
            /*Fim da entrada de dados*/
            /*Posiciona o ponteiro do arquivo na posição referente ao código*/
            fseek(cli, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);

            /*Grava os dados no arquivo clientes.dat*/
            fwrite(&cliente, sizeof(cad_cliente), 1, cli);
        }
    }
    return 0;
}
/*Fim RecebeDadosPessoaFISICA*/

int RecebeDadosPessoaJURIDICA(FILE* cli, cad_cliente cliente, endereco end)
{
    int codigo, verCOD, verCAD;
    char CNPJ[15];

    printf("=============== Cadastrar Pessoa JURÍDICA =================\n");
    printf("%s\n", "Digite o código do cliente (ENTRE 1 E 100):");
    setbuf(stdin, NULL);
    scanf("%d", &codigo);
    verCOD = VerificaCodigo(codigo); /*Recebe o código e verifica se codigo esta entre 1 e 100*/

    if (verCOD != 0) /*Caso código seja inválido, retorna e pede para tentar novamente*/
    {
        return 1;
    }
    else /*Caso código seja válido, prossegue com o cadastro*/
    {
        /*Recebe o CNPJ do cliente*/
        printf("%s\n%s\n", "Formato CNPJ: SOMENTE NÚMEROS!", "Digite o CNPJ do cliente:");
        setbuf(stdin, NULL);
        scanf(" %15[^\n]", CNPJ);

        /*Verifica a exitencia de cliente com mesmo codigo ou cpf*/
        verCAD = VerificaCadastro(cli, cliente, codigo, CNPJ);

        if (verCAD != 0)/*Retorno de VerificaCadastro == 1: Cliente já existente*/
        {
            return 2;
        }
        else if (verCAD == 0)  /*Continua com o cadastro*/
        {
            /*Atribui os valores de codigo e CNPJ recebidos ao cliente*/
            cliente.codigo = codigo;
            strcpy(cliente.CNPJ, CNPJ);

            /*Recebe o nome do cliente*/
            printf("%s\n", "Digite o nome do cliente:");
            setbuf(stdin, NULL);
            scanf(" %40[^\n]", cliente.nome);

            /*Recebe o numero de telefone*/
            printf("%s\n", "Digite o numero de telefone:");
            setbuf(stdin, NULL);
            scanf(" %11[^\n]", cliente.telefone);

            /*Recebe o endereço*/

            /*Recebe o CEP*/
            printf("%s\n", "CEP: SOMENTE NÚMEROS");
            setbuf(stdin, NULL);
            scanf(" %d", &cliente.end.CEP);

            /*Recebe o estado*/
            printf("%s\n", "Estado (No Formato: SP,GO):");
            setbuf(stdin, NULL);
            scanf(" %3[^\n]", cliente.end.estado);

            /*Recebe o município*/
            printf("%s\n", "Município:");
            setbuf(stdin, NULL);
            scanf(" %20[^\n]", cliente.end.cidade);

            /*Recebe a rua*/
            printf("%s\n", "Rua:");
            setbuf(stdin, NULL);
            scanf(" %20[^\n]", cliente.end.rua);

            /*Recebe o número*/
            printf("Número:\n");
            setbuf(stdin, NULL);
            scanf(" %d", &cliente.end.numero);

            clear();
            /*Fim da entrada de dados*/

            /*Posiciona o ponteiro do arquivo na posição referente ao código*/
            fseek(cli, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);

            /*Grava os dados no arquivo clientes.dat*/
            fwrite(&cliente, sizeof(cad_cliente), 1, cli);

        }
    }
    return 0;
}
/*Fim RecebeDadosPessoaJURIDICA*/

void ListarCLIENTES()
{
    int cont_cliente = 0;
    /*Inicialização das structs em branco*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };

    FILE* cli;
    cli = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }

    rewind(cli);

    /*Lê os dados de todos os clientes no arquivo*/
    while (!feof(cli))
    {
        fread(&cliente, sizeof(cad_cliente), 1, cli);

        /*Se existe cliente cadastrado*/
        if (cliente.codigo != 0)
        {
            /*Há clientes cadastrados*/
            cont_cliente = 1;
            /*Imprime por ordem de código*/
            ImprimeDadosCLIENTE(cliente, end);
            printf("\n");

        }
    }
    if (cont_cliente == 1)
    {
        printf("\nPressione ENTER para continuar\n");
        setbuf(stdin, NULL);
        getchar();
        clear();
    }
    else if (cont_cliente != 1)
    {
        printf("Nenhum cliente cadastrado!\n");
        mysleep();
        clear();
    }
}
/*Fim lista cliente*/

void BuscaCLIENTE()
{
    /*Inicialização das structs como  vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };

    /*Váriaveis*/
    int codigo = 0, cont_cliente = 0, cont_busca = 0;
    char busca[40] = "\0";

    /*Abre o arquivo com os dados dos clientes*/
    FILE* cli;
    cli = fopen("clientes.dat", "rb");
    if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1); /*Caso não seja possível abrir o arquivo, sai do programa*/
    }
    /*Permite buscar por nome, codigo ou CPF/CNPJ*/
    printf("============ Buscar Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código,CPF/CNJP ou nome do cliente:");
    setbuf(stdin, NULL);
    scanf(" %40[^\n]", busca);
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(cli); /*Posiciona o ponteiro no inicio do arquivo*/

    while (!feof(cli))
    {
        /*Lê os dados que estão no arquivo de clientes*/
        fread(&cliente, sizeof(cad_cliente), 1, cli);

        /*Compara a ocorrencia com o que foi digitado SOMENTE SE CADASTRO EXISTIR*/
        if (cliente.codigo != 0)
        {
            /*Existe cliente cadastrado*/
            cont_cliente = 1;

            if ((codigo == cliente.codigo) || ((strcmp(busca, cliente.nome) == 0)) || (strcmp(busca, cliente.CPF) == 0) || (strcmp(busca, cliente.CNPJ) == 0))
            {
                /*Cliente foi encontrado*/
                cont_busca = 1;
                clear();
                /*Imprime os dados do cliente na tela*/
                ImprimeDadosCLIENTE(cliente, end);
                printf("\nPressione ENTER para continuar\n");
                setbuf(stdin, NULL);
                getchar();
                clear();
                break;
            }
        }
    }

    /*Se cont_cliente for 0, então nenhum cliente foi cadastrado ainda*/
    if (cont_cliente != 1)
    {
        printf("%s\n", "Nenhum cliente cadastrado!");
        mysleep();
        clear();
    }
    /*Se cont_busca for 0, então o cliente não foi encontrado*/
    else if (cont_busca != 1)
    {
        printf("Cliente não encontrado. Verifique os dados e tente novamente.\n");
        mysleep();
        clear();
    }
    fclose(cli);
}
/*Fim BuscaCLIENTE*/

void AtualizaCLIENTE()
{
    /*Inicialização das structs como  vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };

    char busca[14] = "\0"; /*Variavel para achar cliente*/
    char nome[40] = "", CPF[12] = "", CNPJ[15] = ""; /*Para armazenar o nome, e o CPF/CNPJ cliente*/
    int cont_cliente = 0, cont_busca = 0, codigo = 0;
    int verATT = 0; /*verATT verifica o retorno de AtualizaDados*/

    FILE* cli;
    cli = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }
    printf("============ Atualizar Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código ou CPF/CNJP do cliente:");
    setbuf(stdin, NULL);
    scanf(" %15[^\n]", busca); /*Permite buscar por codigo ou CPF/CNPJ*/
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(cli);
    while (!feof(cli))
    {
        /*Lê os dados do cliente do arquvio clientes.dat*/
        fread(&cliente, sizeof(cad_cliente), 1, cli);

        /*Compara o que foi digitado com o lido no arquivo SOMENTE SE CADASTRO EXISTIR*/
        if (cliente.codigo != 0)
        {
            /*Existe cliente cadastrado*/
            cont_cliente = 1;

            /*Se codigo ou CPF/CNPJ forem iguais aos lidos pelo arquivo*/
            if ((codigo == cliente.codigo) || (strcmp(busca, cliente.CPF) == 0) || (strcmp(busca, cliente.CNPJ) == 0))
            {
                /*Cliente encontrado*/
                cont_busca = 1;

                codigo = cliente.codigo; /*Variavel codigo igual ao codigo do cliente lido no arquivo*/

                /*Copia o Nome, o CNPJ e o CPF lidos no arquivo para as variaveis CPF e CNPJ*/
                strcpy(nome, cliente.nome);
                strcpy(CPF, cliente.CPF);
                strcpy(CNPJ, cliente.CNPJ);

                /*Passa as structs vazias para atualizar corretamente
                    O CODIGO do cliente CONTINUA O MESMO*/
                endereco end = { 0, 0, "", "", "" };
                cad_cliente cliente = { codigo, "", "", "","", end };

                verATT = AtualizaDadosCLIENTE(cli, cliente, end, codigo, CPF, CNPJ, nome);

                if (verATT == 0)
                {
                    printf("Atualização de cliente realizada com sucesso!\n");
                    mysleep();
                    clear();
                    fclose(cli);
                    break;
                }
                else if (verATT == 1);
                {
                    printf("Erro ao atualizar os dados do cliente!\n");
                    mysleep();
                    clear();
                    exit(1);
                }
            }
        }
    }
    /*Nenhum cliente cadastrado*/
    if (cont_cliente != 1)
    {
        printf("%s\n", "Nenhum cliente cadastrado!");
        mysleep();
        clear();
    }
    /* Cliente não foi encontrado*/
    else if (cont_busca != 1)
    {
        printf("Cliente não encontrado. Verifique os dados e tente novamente.\n");
        mysleep();
        clear();
    }
}
/*Fim AtualizaCLIENTE*/


int AtualizaDadosCLIENTE(FILE* cli, cad_cliente cliente, endereco end, int codigo, char CPF[12], char CNPJ[15], char nome[40]) /*Não é permitido atualizar o código do cliente*/
{
    printf("=============== Atualizar Cliente =================\n");
    printf("Nome do cliente a ser atualizado: %s\n\n", nome);

    /*Se o CPF existe, não é pessoa jurídica*/
    if (strcmp(CPF, "") != 0)
    {
        /*Recebe o CPF do cliente*/
        printf("%s\n%s\n", "Formato CPF: SOMENTE NÚMEROS!", "Digite o CPF do cliente:");
        setbuf(stdin, NULL);
        scanf(" %12[^\n]", CPF);
    }

    /*Se CNPJ existe, não é pessoa física*/
    else if (strcmp(CNPJ, "") != 0)
    {
        /*Recebe o CNPJ do cliente*/
        printf("%s\n%s\n", "Formato CNPJ: SOMENTE NÚMEROS!", "Digite o CNPJ do cliente:");
        setbuf(stdin, NULL);
        scanf(" %15[^\n]", CNPJ);
    }

    /*Posiciona o ponteiro de acordo com o código do cliente */
    fseek(cli, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Exclui os dados do cliente no arquivo para atualizar completamente. O CODIGO CONTINUA O MESMO!*/
    fwrite(&cliente, sizeof(cad_cliente), 1, cli);

    /*Atribui o CPF lido à struct cliente*/
    if (strcmp(CPF, "") != 0) {
        strcpy(cliente.CPF, CPF);
    }
    /*Atribui o CNPJ lido à struct cliente*/
    else if (strcmp(CNPJ, "") != 0) {
        strcpy(cliente.CNPJ, CNPJ);
    }

    /*Recebe o nome do cliente*/
    printf("%s\n", "Digite o nome do cliente:");
    setbuf(stdin, NULL);
    fgets(cliente.nome, sizeof cliente.nome, stdin);
    //scanf(" %40[^\n]", cliente.nome);

    /*Recebe o numero de telefone*/
    printf("%s\n", "Digite o numero de telefone:");
    setbuf(stdin, NULL);
    scanf(" %11[^\n]", cliente.telefone);

    /*Recebe o endereço*/

    /*Recebe o CEP*/
    printf("%s\n", "CEP: SOMENTE NÚMEROS");
    setbuf(stdin, NULL);
    scanf(" %d", &cliente.end.CEP);

    /*Recebe o estado*/
    printf("%s\n", "Estado (No Formato: SP,GO):");
    setbuf(stdin, NULL);
    scanf(" %3[^\n]", cliente.end.estado);

    /*Recebe o município*/
    printf("%s\n", "Município:");
    setbuf(stdin, NULL);
    scanf(" %20[^\n]", cliente.end.cidade);

    /*Recebe a rua*/
    printf("%s\n", "Rua:");
    setbuf(stdin, NULL);
    scanf(" %20[^\n]", cliente.end.rua);

    /*Recebe o número*/
    printf("Número:\n");
    setbuf(stdin, NULL);
    scanf(" %d", &cliente.end.numero);

    clear();
    /*Fim da entrada de dados*/

    /*Posiciona o ponteiro de acordo com o código do cliente */
    fseek(cli, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Grava os novos dados no arquivo*/
    fwrite(&cliente, sizeof(cad_cliente), 1, cli);

    return 0;
}
/*Fim AtualizaDadosCLIENTE*/

void ExcluiCLIENTE()
{
    /*Structs para receber os dados do arquivo*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };

    /*Structs em branco para gravar no arquivo*/
    endereco blank_end = { 0, 0, "", "", "" };
    cad_cliente blank_cliente = { 0, "", "", "", "", blank_end };


    char op = ' ';
    int codigo = 0, cont_cliente = 0, cont_busca = 0;
    char busca[15];

    FILE* cli;
    cli = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }

    printf("============ Excluir Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código ou CPF/CNJP do cliente:");
    setbuf(stdin, NULL);
    scanf(" %14[^\n]", busca); /*Permite excluir por codigo ou CPF/CNPJ*/
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(cli);

    while (!feof(cli))
    {
        /*Lê os dados do cliente do arquvio clientes.dat*/
        fread(&cliente, sizeof(cad_cliente), 1, cli);

        if (cliente.codigo != 0)
        {
            /*Existe Cliente Cadastrado*/
            cont_cliente = 1;

            /*Se codigo ou CPF/CNPJ forem iguais aos lidos pelo arquivo*/
            if ((codigo == cliente.codigo) || (strcmp(busca, cliente.CPF) == 0) || (strcmp(busca, cliente.CNPJ) == 0))
            {
                /*Cliente encontrado*/
                cont_busca = 1;
                do
                {
                    ImprimeDadosCLIENTE(cliente, end);

                    printf("%s\n", "Deseja prosseguir com a exclusão do cliente?");
                    printf("%s\n%s\n", "Sim- S", "Não - N");
                    setbuf(stdin, NULL);
                    scanf(" %c", &op);
                    clear();
                    if ((op != 'S' && op != 's') && (op != 'N' && op != 'n'))
                    {
                        printf("Comando Inválido!\n");
                        mysleep();
                        clear();
                        continue;
                    }
                    else break;
                } while (1);

                if (op == 'S' || op == 's')
                {
                    /*Posiciona o ponteiro na posição onde o cliente está cadastrado*/
                    fseek(cli, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);

                    /*Grava os dados da struct em branco no arquivo*/
                    fwrite(&blank_cliente, sizeof(cad_cliente), 1, cli);

                    printf("Exclusão realizada com sucesso!\n");
                    mysleep();
                    clear();
                    break;
                }
                if (op == 'N' || op == 'n')
                {
                    printf("Exclusão cancelada!");
                    mysleep();
                    clear();
                    break;
                }
            }
        }
    }
    /*Nenhum cliente cadastrado*/
    if (cont_cliente != 1)
    {
        printf("%s\n", "Nenhum cliente cadastrado!");
        mysleep();
        clear();
    }
    /* Cliente não foi encontrado*/
    else if (cont_busca != 1)
    {
        printf("Cliente não encontrado. Verifique os dados e tente novamente.\n");
        mysleep();
        clear();
    }
    fclose(cli);/*Fecha o arquivo*/
}
/*Fim ExcluiCLIENTE*/
void ImprimeDadosCLIENTE(cad_cliente cliente, endereco end)
{
    /*Imprime o nome*/
    printf("%s%s\n", "Nome: ", cliente.nome);

    /*Imprime o codigo*/
    printf("%s%d\n", "Código: ", cliente.codigo);

    /*Se CPF existir, imprime o CPF*/
    if (cliente.CPF[0] != '\0')
    {
        printf("%s%s\n", "CPF: ", cliente.CPF);
    }

    /*Se o CNPJ existir, imprime o CNPJ*/
    else if (cliente.CNPJ[0] != '\0')
    {
        printf("%s%s\n", "CNPJ: ", cliente.CNPJ);
    }

    /*Imprime o telefone*/
    printf("%s%s\n", "Telefone: ", cliente.telefone);

    /*Imprime o endereço*/
    printf("%s\n", "Endereço:");

    printf("  %s%s\n", "Estado: ", strupr(cliente.end.estado));    /*Estado*/
    printf("  %s%s\n", "Município: ", cliente.end.cidade); /*Município*/
    printf("  %s%d\n", "CEP: ", cliente.end.CEP);          /*CEP*/
    printf("  %s%s\n", "Rua: ", cliente.end.rua);          /*Rua*/
    printf("  %s%d\n", "Número: ", cliente.end.numero);    /*Numero*/
}
/*Fim ImprimeDadosCLIENTE*/
/******************FIM FUNÇÕES CLIENTES**************************/

/******************INÍCIO FUNÇÕES CONTA***************************/

void GerenciarCONTAS()
{
    char op;
    do {
        printf("============ Gerenciar Contas ============\n");
        printf("Digite um comando para prosseguir:\n");
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            "R –Listagem de todas as contas cadastradas.", "C –Cadastrar uma conta para um cliente.",
            "L –Listar todas as contas de um cliente.", "W –Realizar um saque em uma conta.",
            "D –Realizar um depósito em uma conta.", "T –Realizar transferência entre contas.",
            "E –Exibir extrato de uma conta.", "S –Sair");
        setbuf(stdin, NULL);
        scanf(" %c", &op);

        switch (op)
        {
        case 'R':
        case 'r':
            ListaTodasContas();
            break;

        case 'C':
        case 'c':
            CadastroCONTA();
            break;

        case 'L':
        case 'l':
            ListaContaCliente();
            break;

        case 'W':
        case 'w':
            Saque();
            break;
        case 'D':
        case 'd':
            Deposito();
            break;

        case 'T':
        case 't':
            Transferencia();
            break;

        case 'E':
        case 'e':
            Extrato();
            break;

        case 'S':
        case 's':
            Sair_Ou_Menu();
            break;

        default:
            clear();
            printf("Comando inválido!\n");
            mysleep();
            clear();
            //GerenciarCONTAS();
            break;
        }
    } while ((op != 'R' && op != 'r') && (op != 'C' && op != 'c')
        && (op != 'L' && op != 'l') && (op != 'W' && op != 'w')
        && (op != 'D' && op != 'd') && (op != 'T' && op != 't')
        && (op != 'E' && op != 'e') && (op != 'S' && op != 's'));
}
/*Fim GerenciaCONTAS*/

void CadastroCONTA()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    char busca[15] = "";
    int cont_cliente = 0, cont_busca = 0, codigo = 0, agencia = 0, account = 0, verCONTA = 0;

    FILE* acct, * cli;
    acct = fopen("contas.dat", "rb+");  /*Abre arquivo de contas*/
    cli = fopen("clientes.dat", "rb+"); /*Abre arquivos de clientes*/

    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        acct = fopen("contas.dat", "wb+");

        /*Sai do programa se não for possível criar o arquivo*/
        if (acct == NULL)
        {
            printf("Não foi possível abrir o arquivo.");
            exit(1);
        }
        /*Incializa o arquivo de contas com 200 posições em branco*/
        else
        {
            for (int i = 0; i < MAX_CONTAS; i++) fwrite(&conta, sizeof(CONTA), 1, acct);

            fclose(acct);
            acct = fopen("contas.dat", "rb+");
        }
    }
    /*Se não for possível abrir o arquivo de clientes, sai do programa*/
    else if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }

    clear();
    printf("============ Cadastrar Conta ============\n");
    printf("%s\n", "Informe o código, o CPF ou o CNPJ do cliente: ");
    setbuf(stdin, NULL);
    scanf(" %15[^\n]", busca);

    codigo = atoi(busca);
    rewind(acct);
    rewind(cli);
    while (!feof(cli))
    {
        /*Lê os dados do aqruivo de clientes*/
        fread(&cliente, sizeof(cad_cliente), 1, cli);

        /*Verifica se existe cliente cadastrado*/
        if (cliente.codigo != 0)
        {
            /*Existe cliente cadastrado*/
            cont_cliente = 1;

            /*Compara o dado de busca com o presente no arquivo*/
            if (codigo == cliente.codigo || (strcmp(busca, cliente.CPF) == 0) || (strcmp(busca, cliente.CNPJ) == 0))
            {
                /*Cliente encontrado*/
                cont_busca = 1;

                /*Pede o numero da agencia e conta. Repete caso numero da conta seja inválido*/
                do
                {
                    printf("%s\n", "Digite o numero da agencia:");
                    scanf(" %d", &agencia);
                    printf("%s\n", "Digite o número da conta (ENTRE 1 E 200):");
                    scanf(" %d", &account);
                    clear();

                    if (account < 1 || account > 200)
                    {
                        printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
                        mysleep();
                        clear();
                    }
                } while (account < 1 || account > 200);

                /*Verifica se conta já foi cadastrada*/
                verCONTA = VerificaCONTA(account, agencia, acct, conta);

                /*Existe conta cadastrada com os dados informados*/
                if (verCONTA == 1)
                {
                    break;
                }
                /*Conta pode ser cadastrada*/
                else if (verCONTA == 0)
                {
                    /*Grava os dados das contas no struct CONTA*/
                    conta.num_conta = account;
                    conta.agencia = agencia;
                    conta.saldo = 0.0;
                    conta.cliente = cliente;
                    /*Posiciona o ponteiro na posição de número de conta*/
                    fseek(acct, (conta.num_conta - 1) * sizeof(CONTA), SEEK_SET);
                    /*Escreve os dados no arquivo*/
                    fwrite(&conta, sizeof(CONTA), 1, acct);
                    printf("%s\n", "Cadastro de conta realizado com sucesso!\n");
                    mysleep();
                    clear();
                    break;
                }
            }
        }
    }
    fclose(acct);
    fclose(cli);
    /*Se cont_cliente for 0, então nenhum cliente foi cadastrado ainda*/
    if (cont_cliente != 1)
    {
        printf("%s\n", "Nenhum cliente cadastrado!");
        mysleep();
        clear();
    }
    /*Se cont_busca for 0, então o cliente não foi encontrado*/
    else if (cont_busca != 1)
    {
        printf("Cliente não encontrado. Verifique os dados e tente novamente.\n");
        mysleep();
        clear();
    }
}
/*Fim CadastroCONTA*/

int VerificaCONTA(int account, int agencia, FILE* acct, CONTA conta)
{
    //int verCONTA = 0;
    /*Verfica todo o arquvio em busca de conta já existente*/
    rewind(acct);
    while (!feof(acct))
    {
        fread(&conta, sizeof(CONTA), 1, acct);
        /*Se o numero da conta e agencia já foram cadastrados*/
        if (account == conta.num_conta && agencia == conta.agencia)
        {
            // verCONTA = 1;
            clear();
            printf("Conta já existente!");
            mysleep();
            clear();
            return 1; /*1 representa conta já cadastrada*/
            break;
        }
    }
    return 0;
}
/*Fim VerificaCONTA*/

void ImprimeDadosCONTA(CONTA conta)
{
    printf("%s%4d\n", "Numero da agencia: ", conta.agencia);
    printf("%s%d\n", "Numero da conta: ", conta.num_conta);
    printf("%s%s\n", "Cliente: ", conta.cliente.nome);
    /*Imprime CPF se for pessoa física ou CNPJ se for pessoa jurídica*/
    if ((strcmp(conta.cliente.CPF, "")) != 0)
    {
        printf("%s%s\n", "CPF: ", conta.cliente.CPF);
    }
    else if ((strcmp(conta.cliente.CNPJ, "")) != 0)
    {
        printf("%s%s\n", "CNPJ: ", conta.cliente.CNPJ);
    }
    printf("%s%0.2lf\n", "Saldo: ", conta.saldo);
}
/*Fim ImprimeDadosCONTA*/

void ListaContaCliente()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    int cont_conta = 0, cont_busca = 0;

    FILE* acct, * cli;
    acct = fopen("contas.dat", "rb+");  /*Abre arquivo de contas*/

    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.");
        mysleep();
        exit(1);
    }


    char busca[15] = "";
    int codigo = 0;
    clear();
    printf("============ Listar Contas de um cliente ============\n");
    printf("%s\n", "Infome o código, o CPF ou o CNPJ do cliente:");
    setbuf(stdin, NULL);
    scanf("%15[^\n]", busca);
    codigo = atoi(busca);
    clear();

    rewind(acct);
    while (!feof(acct))
    {
        /*Lê os dados do arquivo contas*/
        fread(&conta, sizeof(CONTA), 1, acct);

        /*Se existe alguma conta cadastrada*/
        if (conta.cliente.codigo != 0)
        {
            /*Existe alguma conta cadastrada*/
            cont_conta = 1;

            /*Compara o dado de busca com o lido pelo arquivo*/
            if ((codigo == conta.cliente.codigo) || (strcmp(busca, conta.cliente.CNPJ) == 0) || (strcmp(busca, conta.cliente.CPF) == 0))
            {
                /*Cliente encontrado*/
                cont_busca = 1;
                ImprimeDadosCONTA(conta);
                printf("\n");
            }
        }
    }
    if (cont_busca == 1)
    {
        printf("%s\n", "Pressione ENTER para continuar");
        setbuf(stdin, NULL);
        getchar();
    }
    else if (cont_conta != 1)
    {
        printf("%s\n", "Não há contas cadastradas!");
        mysleep();
        clear();
    }
    else if (cont_busca != 1)
    {
        printf("%s\n", "Cliente não possui conta cadastrada!");
        mysleep();
        clear();
    }
}
/*Fim ListaContaCliente*/

void ListaTodasContas()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    int cont_conta = 0, cont_busca = 0;

    FILE* acct, * cli;
    acct = fopen("contas.dat", "rb+");  /*Abre arquivo de contas*/
    cli = fopen("clientes.dat", "rb+"); /*Abre arquivos de clientes*/

    clear();
    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.");
        mysleep();
        exit(1);
    }
    /*Se não for possível abrir o arquivo de clientes, sai do programa*/
    else if (cli == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }

    printf("============ Listar Todas as Contas============\n");

    rewind(cli);
    rewind(acct);

    while (!feof(acct))
    {
        /*Lê dados do arquivo de contas*/
        fread(&conta, sizeof(CONTA), 1, acct);

        if (conta.cliente.codigo != 0)
        {
            /*Existe conta cadastrada*/
            cont_conta = 1;
            ImprimeDadosCONTA(conta);
            printf("\n");
        }
    }
    fclose(acct);
    fclose(cli);
    if (cont_conta == 1)
    {
        printf("%s\n", "Pressione ENTER para continuar");
        setbuf(stdin, NULL);
        getchar();
        clear();
    }
    else if (cont_conta != 1)
    {
        printf("%s\n", "Não há contas cadastradas!");
        mysleep();
        clear();
    }

}
/*Fim ListaTodasContas*/

void Saque()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    int agencia = 0, account = 0, cont_conta = 0, verVALOR = 0;
    double valor = 0.0, saldo = 0.0;
    char descricao[100] = "";
    FILE* acct;
    acct = fopen("contas.dat", "rb+");  /*Abre arquivo de contas*/
    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.");
        mysleep();
        exit(1);
    }
    clear();
    printf("============ Realizar Saque============\n");

    do
    {
        printf("%s\n", "Digite o numero da agencia:");
        scanf(" %d", &agencia);
        printf("%s\n", "Digite o número da conta (ENTRE 1 E 200):");
        scanf(" %d", &account);
        clear();
        if (account < 1 || account > 200)
        {
            printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
            mysleep();
            clear();
        }
    } while (account < 1 || account > 200);

    rewind(acct);
    while (!feof(acct))
    {
        /*Lê os dados do arquivo contas*/
        fread(&conta, sizeof(CONTA), 1, acct);
        if (conta.cliente.codigo != 0) {
            /*Se conta informada exisir*/
            if (agencia == conta.agencia && account == conta.num_conta)
            {
                /*Conta encontrada*/
                cont_conta = 1;
                /*Imprime os dados da conta*/
                ImprimeDadosCONTA(conta);
                printf("\n");
                do
                {
                    printf("%s\n", "Digite o valor que deseja sacar:");
                    scanf("%lf", &valor);
                    /*Verifica se saldo é menor que valor a ser sacado*/
                    verVALOR = VerificaSaldo(conta, valor);
                    /*Saldo é menor ou igual, pode prossguir*/
                    if (verVALOR == 0) break;

                    /*Deseja digitar outro valor*/
                    else if (verVALOR == 1) continue;

                    /*Deseja sair do menu de saque*/
                    else if (verVALOR == 2) break;
                } while (1);

                if (verVALOR == 0)
                {
                    printf("%s\n", "Adicione uma descrição ao saque:");
                    scanf(" %100[^\n]", descricao);
                    conta.saldo -= valor;

                    /*Posiciona o ponteiro na posição de número de conta*/
                    fseek(acct, (conta.num_conta - 1) * sizeof(CONTA), SEEK_SET);

                    /*Escreve os dados com o novo saldo no arquivo*/
                    fwrite(&conta, sizeof(CONTA), 1, acct);
                    printf("%s\n", "Saque realizado com sucesso!");
                    mysleep();
                    clear();
                    Debito(conta,valor,descricao);
                    break;
                }
                else if (verVALOR == 2) break;
            }
        }
    }
    fclose(acct);
    if (cont_conta != 1)
    {
        printf("%s\n", "Conta não encontrada!");
        mysleep();
        clear();
    }
}
/*Fim Saque*/

void Deposito()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    int agencia = 0, account = 0, cont_conta = 0, verVALOR = 0;
    double valor = 0.0, saldo = 0.0;
    char descricao[100] = "";
    FILE* acct;
    acct = fopen("contas.dat", "rb+"); /*Abre arquivo de contas*/

    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.");
        mysleep();
        exit(1);
    }
    clear();
    printf("============ Realizar Depósito============\n");

    do
    {
        printf("%s\n", "Digite o numero da agencia:");
        scanf(" %d", &agencia);
        printf("%s\n", "Digite o número da conta (ENTRE 1 E 200):");
        scanf(" %d", &account);
        clear();
        if (account < 1 || account > 200)
        {
            printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
            mysleep();
            clear();
        }
    } while (account < 1 || account > 200);
    rewind(acct);
    while (!feof(acct))
    {
        /*Lê os dados do arquivo contas*/
        fread(&conta, sizeof(CONTA), 1, acct);

        if (conta.cliente.codigo != 0)
        {
            /*Se conta informada exisir*/
            if (agencia == conta.agencia && account == conta.num_conta)
            {
                /*Conta encontrada*/
                cont_conta = 1;

                /*Imprime os dados da conta*/
                ImprimeDadosCONTA(conta);
                printf("\n");
                /*Recebe o valor do depósito*/
                printf("%s\n", "Digite o valor que deseja depositar:");
                scanf("%lf", &valor);
                /*Adiciona a descrição de deposito*/
                printf("%s\n", "Adicione uma descrição ao deposito:");
                scanf(" %100[^\n]", descricao);
                /*Atualiza o saldo do cliente*/
                conta.saldo += valor;
                /*Posiciona o ponteiro na posição de número de conta*/
                fseek(acct, (conta.num_conta - 1) * sizeof(CONTA), SEEK_SET);

                /*Escreve os dados com o novo saldo no arquivo*/
                fwrite(&conta, sizeof(CONTA), 1, acct);
                Credito(conta,valor,descricao);
                break;
            }
        }
    }
    fclose(acct);
    printf("\n%s\n", "Depósito realizado com sucesso!");
    mysleep();
    clear();
    //Credito(conta,valor,descricao);
    if (cont_conta != 1)
    {
        printf("%s\n", "Conta não encontrada!");
        mysleep();
        clear();
    }
}
/*Fim Deposito*/

void Transferencia()
{
    /*Incialização das structs como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };
    CONTA conta = { 0, 0, 0.0, cliente };

    /*Struct para armazenar dados temporários*/
    struct Account
    {
        int agencia;
        int num_conta;
        double saldo;
        cad_cliente cliente;
    }account_orig = { 0,0,0.0,cliente };

    struct Account2
    {
        int agencia;
        int num_conta;
        double saldo;
        cad_cliente cliente;
    }account_dest;

    int agencia = 0, account = 0, cont_conta = 0, verVALOR = 0;
    double valor = 0.0, saldo = 0.0;
    char descricao_credito[100] = "", descricao_debito[100] = "";

    FILE* acct;
    acct = fopen("contas.dat", "rb+"); /*Abre arquivo de contas*/

    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.");
        mysleep();
        exit(1);
    }

    clear();
    printf("============ Realizar Transferencia============\n");

    do
    {
        printf("%s\n", "Digite o numero da agencia da conta de ORIGEM:");
        scanf(" %d", &agencia);
        printf("%s\n", "Digite o número da conta de ORIGEM (ENTRE 1 E 200):");
        scanf(" %d", &account);
        clear();
        if (account < 1 || account > 200)
        {
            printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
            mysleep();
            clear();
        }
    } while (account < 1 || account > 200);

    rewind(acct);
    while (!feof(acct))
    {
        /*Lê os dados do arquivo contas*/
        fread(&conta, sizeof(CONTA), 1, acct);

        if (conta.cliente.codigo != 0)
        {
            /*Se conta informada exisir*/
            if (agencia == conta.agencia && account == conta.num_conta)
            {
                /*Conta encontrada*/
                cont_conta = 1;

                /*Armazena os dados de origem na struct temporária de origem*/
                account_orig.num_conta = conta.num_conta;
                account_orig.agencia = conta.agencia;
                account_orig.cliente = conta.cliente;
                account_orig.saldo = conta.saldo;

                /*Recebe os dados da conta de destino*/
                do
                {
                    /*Imprime os dados da conta de origem*/
                    printf("%s\n", "Conta de ORIGEM:");
                    ImprimeDadosCONTA(conta);
                    printf("\n");

                    /*Recebe os dados da conta de destino*/
                    printf("%s\n", "Digite o numero da agencia da conta de DESTINO:");
                    scanf(" %d", &agencia);
                    printf("%s\n", "Digite o número da conta de DESTINO (ENTRE 1 E 200):");
                    scanf(" %d", &account);

                    /*Se numero de conta é valido*/
                    if (account < 1 || account > 200)
                    {
                        printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
                        mysleep();
                        clear();
                    }
                } while (account < 1 || account > 200);


                /*Posiciona o ponteiro na conta informada*/
                fseek(acct, (account - 1) * sizeof(CONTA), SEEK_SET);
                /*Armazena os dados na struct temporária*/
                fread(&conta, sizeof(CONTA), 1, acct);

                /*Armazena os dados de destino na struct temporária de destino*/
                account_dest.num_conta = conta.num_conta;
                account_dest.agencia = conta.agencia;
                account_dest.cliente = conta.cliente;
                account_dest.saldo = conta.saldo;

                /*Imprime os dados da conta de destino*/
                printf("%s\n", "Conta de DESTINO:");
                ImprimeDadosCONTA(conta);
                printf("\n");

                /*Se conta informada existir*/
                if (account_orig.num_conta != 0)
                {
                    /*Armazena na struct de contas os dados da conta de origem*/
                    conta.num_conta = account_orig.num_conta;
                    conta.agencia = account_orig.agencia;
                    conta.cliente = account_orig.cliente;
                    conta.saldo = account_orig.saldo;

                    do
                    {
                        printf("%s\n", "Digite o valor que deseja transferir:");
                        scanf("%lf", &valor);

                        /*Verifica se saldo é menor que valor a ser transferido pela conta de origem*/
                        verVALOR = VerificaSaldo(conta, valor);

                        /*Saldo é menor ou igual, pode prossguir*/
                        if (verVALOR == 0)
                            break;

                        /*Deseja digitar outro valor*/
                        else if (verVALOR == 1)
                            continue;

                        /*Deseja sair do menu de saque*/
                        else if (verVALOR == 2)
                            break;
                    } while (1);

                    /*Saldo suficiente, pode prosseguir com a transferencia*/
                    if (verVALOR == 0)
                    {
                        /*Atualiza o saldo da conta de origem e da conta de destino*/
                        conta.saldo -= valor;
                        account_dest.saldo += valor;

                        /*Grava o novo saldo subtraído na conta de origem*/
                        fseek(acct, (conta.num_conta - 1) * sizeof(CONTA), SEEK_SET);
                        fwrite(&conta, sizeof(CONTA), 1, acct);
                        sprintf(descricao_debito,"Transferência para conta: {%4d}-{%d}",conta.agencia,conta.num_conta);

                        Debito(conta,valor,descricao_debito);

                        /*Armazena na struct de contas os dados de destino*/
                        conta.num_conta = account_dest.num_conta;
                        conta.agencia = account_dest.agencia;
                        conta.cliente = account_dest.cliente;
                        conta.saldo = account_dest.saldo;

                        /*Grava o novo saldo somado na conta de destino*/
                        fseek(acct, (account_dest.num_conta - 1) * sizeof(CONTA), SEEK_SET);
                        fwrite(&account_dest, sizeof(CONTA), 1, acct);
                        sprintf(descricao_credito,"Transferência de conta {%4d}-{%d}",conta.agencia,conta.num_conta);

                        Credito(conta,valor,descricao_credito);

                        printf("\n%s\n", "Transferencia realizada com suceesso!");
                        mysleep();
                        clear();
                        break;
                    }

                    /*Saldo insuficiente e deseja voltar para o menu*/
                    else if (verVALOR == 2) break;
                }

                /*Conta de destino não existe*/
                else
                {
                    printf("%s\n", "Conta de destino não encontrada! Verifique os dados e tente novamente");
                    mysleep();
                    clear();
                }
            }
        }
    }
    fclose(acct);
}
/*Fim Transferencia*/

int VerificaSaldo(CONTA conta, int valor)
{
    char op = ' ';
    if (valor > conta.saldo)
    {
        clear();
        printf("%s\n", "Saldo insuficiente para realização da operação!");
        mysleep();
        printf("%s\n", "Deseja digitar outro valor ou voltar ao menu principal?");
        printf("%s\n%s\n", "D - Digitar outro valor", " S- Voltar ao menu principal");
        scanf(" %c", &op);
        clear();
        if (op == 'D' || op == 'd')
        {
            return 1;
        }
        else if (op == 'S' || op == 's')
        {
            return 2;
        }

    }
    return 0;
}
/*Fim VerificaSaldo*/

void Debito(CONTA conta, double valor, char descricao[100])
{
    struct TempTrans
    {
        double valor;
        char descricao[100];
        int num_trans, dia, mes, ano;
        CONTA conta;
    } temp_trans;


    time_t segundos; // guarda os segundos desde 01/01/1970
    struct tm* data_atual;       // declara uma estrutura tm

    time(&segundos); // preenche a variável data_hora_segundos

    data_atual = localtime(&segundos); // preenche a estrutura data_atual 

    FILE* tran;
    tran = fopen("transacoes.dat", "rb+"); /*Abre arquivo de transferencias*/

    /*Se arquivo de transações não existir, cria o arquivo e incializa com 1000 posições*/
    if (tran == NULL)
    {
        tran = fopen("transacoes.dat", "wb+");

        /*Incialização das structs como vazias*/
        endereco end = { 0, 0, "", "", "" };
        cad_cliente cliente = { 0, "", "", "", "", end };
        CONTA conta = { 0, 0, 0.0, cliente };

        /*Inicialização da struct de transferencia*/
        RegTransferencia trans = {0.0,"",0,0,0,conta};

        /*Sai do programa se não for possível criar o arquivo*/
        if (tran == NULL)
        {
            printf("Não foi possível abrir o arquivo.");
            exit(1);
        }
        /*Incializa o arquivo de contas com 1000 posições em branco*/
        else
        {
            for (int i = 0; i < MAX_TRANS; i++) fwrite(&trans, sizeof(RegTransferencia), 1, tran);

            fclose(tran);
            tran = fopen("transacoes.dat", "rb+");
        }
    }
    
    /*Atribui os dados de debito a struct de transferencia*/
    RegTransferencia trans = {-valor,"",0,0,0,conta};

    strcpy(trans.descricao,descricao);
    trans.dia = data_atual->tm_mday;
    trans.mes = data_atual->tm_mon +1;
    trans.ano = data_atual->tm_year + 1900;

    int cont_trans=0;
    
    rewind(tran);
    while(!feof(tran)) 
    {
        /*Lê os dados do arquivo de transacoes e armazena na struct temporaria*/
        fread(&temp_trans,sizeof(struct TempTrans),1,tran);

        /*Se não houver transação cadastrada em tal posição, ira cadastrar uma nessa posição*/
        if(temp_trans.mes == 0) 
        {   cont_trans =1;
            fwrite(&trans,sizeof(RegTransferencia),1,tran);
            break;
        }
    }
    fclose(tran);
    if(cont_trans ==0)
    {
        printf("%s\n","Limite de transações antigido!");
        mysleep();
        clear();
    }
}
/*Fim debito*/

void Credito(CONTA conta, double valor, char descricao[100])
{
    struct TempTrans
    {
        double valor;
        char descricao[100];
        int num_trans, dia, mes, ano;
        CONTA conta;
    } temp_trans;


    time_t segundos; // guarda os segundos desde 01/01/1970
    struct tm* data_atual;       // declara uma estrutura tm

    time(&segundos); // preenche a variável data_hora_segundos

    data_atual = localtime(&segundos); // preenche a estrutura data_atual 

    FILE* tran;
    tran = fopen("transacoes.dat", "rb+"); /*Abre arquivo de transferencias*/

    /*Se arquivo de transações não existir, cria o arquivo e incializa com 1000 posições*/
    if (tran == NULL)
    {
        tran = fopen("transacoes.dat", "wb+");

        /*Incialização das structs como vazias*/
        endereco end = { 0, 0, "", "", "" };
        cad_cliente cliente = { 0, "", "", "", "", end };
        CONTA conta = { 0, 0, 0.0, cliente };

        /*Inicialização da struct de transferencia*/
        RegTransferencia trans = {0.0,"",0,0,0,conta};

        /*Sai do programa se não for possível criar o arquivo*/
        if (tran == NULL)
        {
            printf("Não foi possível abrir o arquivo.");
            exit(1);
        }
        /*Incializa o arquivo de contas com 1000 posições em branco*/
        else
        {
            for (int i = 0; i < MAX_TRANS; i++) fwrite(&trans, sizeof(RegTransferencia), 1, tran);

            fclose(tran);
            tran = fopen("transacoes.dat", "rb+");
        }
    }
    
    /*Atribui os dados de debito a struct de transferencia*/
    RegTransferencia trans = {valor,"",0,0,0,conta};

    strcpy(trans.descricao,descricao);
    trans.dia = data_atual->tm_mday;
    trans.mes = data_atual->tm_mon +1;
    trans.ano = data_atual->tm_year + 1900;

    int cont_trans=0;
    
    rewind(tran);
    while(!feof(tran)) 
    {
        /*Lê os dados do arquivo de transacoes e armazena na struct temporaria*/
        fread(&temp_trans,sizeof(struct TempTrans),1,tran);

        /*Se não houver transação cadastrada em tal posição, ira cadastrar uma nessa posição*/
        if(temp_trans.mes == 0) 
        {   cont_trans =1;
            fwrite(&trans,sizeof(RegTransferencia),1,tran);
            break;
        }
    }
    fclose(tran);
    if(cont_trans ==0)
    {
        printf("%s\n","Limite de transações antigido!");
        mysleep();
        clear();
    }
}
/*Fim Credito*/

void Extrato() 
{
    int account =0,agencia =0,cont_conta =0,cont_trans =0;
    
    FILE* acct,*tran;
    acct = fopen("contas.dat", "rb+"); /*Abre arquivo de contas*/
    tran = fopen("transacoes.dat","rb+");
    /*Se arquivo de contas não existir, cria o arquivo e incializa com 200 contas*/
    if (acct == NULL)
    {
        /*Sai do programa se não for possível abrir o arquivo*/
        printf("Não foi possível abrir o arquivo de transações. Arquivo não criado\n");
        mysleep();
        exit(1);
    }
    else if(tran == NULL) 
    {
        /*Sai do programa se não for possível criar o arquivo*/
        printf("Não foi possível abrir o arquivo de contas.\n");
        mysleep();
        exit(1);
    }

    /*Incialização das structs como vazias*/
    endereco end = {0, 0, "", "", ""};
    cad_cliente cliente = {0, "", "", "", "", end};
    CONTA conta = {0, 0, 0.0, cliente};

    /*Inicialização da struct de transferencia*/
    RegTransferencia trans = {0.0, "", 0, 0, 0, conta};

    clear();

    
    
    do
    {
        printf("============ Visualizar Extrato============\n");
        printf("%s\n", "Digite o numero da agencia:");
        scanf(" %d", &agencia);
        printf("%s\n", "Digite o número da conta (ENTRE 1 E 200):");
        scanf(" %d", &account);
        if (account < 1 || account > 200)
        {
            printf("Numero de conta inválido! Digite um número entre 1 e 200.\n");
            mysleep();
            clear();
        }
    } while (account < 1 || account > 200);
    
    rewind(acct);
    rewind(tran);
    while (!feof(acct))
    {
        /*Lê os dados do arquivo contas*/
        fread(&conta, sizeof(CONTA), 1, acct);

        if (conta.cliente.codigo != 0)
        {
            /*Se conta informada exisir*/
            if (agencia == conta.agencia && account == conta.num_conta)
            {
                /*Conta encontrada*/
                cont_conta = 1;

                /*Imprime os dados da conta*/
                ImprimeDadosCONTA(conta);
                printf("\n");
                break;
            }
        }
    }
    fclose(acct);
    rewind(tran);
    while (!feof(tran))
    {
        fread(&trans, sizeof(RegTransferencia), 1, tran);

        if (trans.conta.agencia == agencia && trans.conta.num_conta == account)
        {
            /*Cliente possui transações registradas*/
            cont_trans = 1;

            printf("%s %2d/%2d/%2d\n", "Data:", trans.dia, trans.mes, trans.ano);
            if (trans.valor < 0)
            {
                printf("%s\n", "Operação: DÉBITO");
            }
            else
                printf("%s %lf\n", "Operação: CRÉDITO");
            printf("%s %s\n", "Descrição:", trans.descricao);

            printf("%s %lf\n", "Valor:", trans.valor);
            break;
        }
    }

    fclose(tran);

    if(cont_trans ==1) 
    {
        printf("\nPressione ENTER para continuar\n");
        setbuf(stdin, NULL);
        getchar();
        clear();
    }
    else if (cont_conta != 1)
    {
        printf("%s\n", "Conta não encontrada!");
        mysleep();
        clear();
    }
    else if(cont_trans !=1) 
    {
        printf("%s\n","Conta não possui transações registradas!");
        mysleep();
        clear();
    }
}