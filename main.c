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
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

const int MAX_CLIENTES = 100;
const int MAX_CONTAS = 200;
const int MAX_TRANS = 1000;

/*ARRUMAR LIXO DE MEMÓRIA EM ALGUM LUGAR DO CADASTRO*/

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

struct Conta 
{
    char agencia[5];
    char conta[8];
    double saldo;
    cad_cliente cliente;
};

typedef struct Conta Conta;
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

/* Protótipo MENU PRINCIPAL */
char menu(void);

/*Protótipo para sair do aplicativo ou voltar para o menu principal*/
void Sair_Ou_Menu();

/*Protótipos de GERENCIAMENTO DE CLIENTE*/
void GerenciarCLIENTE();                                                                                    /* Protótipo MENU DE CLIENTES */
    void CadastroCLIENTE();                                                                                     /*Protótipo de CADASTRO DE CLIENTES*/
    int RecebeDadosPessoaFISICA(FILE *fpt, cad_cliente cliente, endereco end);                                 /*Protótipo recebe dados Pessoa FISICA*/
    int RecebeDadosPessoaJURIDICA(FILE *fpt, cad_cliente cliente, endereco end);                               /*Prototipo recebe dados pessoa JURIDICA*/
    int VerificaCodigo(int);                                                                                    /*Protipo para VERIFICAR CODIGO*/
    int VerificaCadastro(FILE *fpt, cad_cliente cliente, int, char[]);                                         /*Prototipo para VERIFICAR EXISTENCIA DE CADASTRO*/
    void ListarCLIENTES();                                                                                      /*Prototipo pra LISTAR CLIENTES*/
    void BuscaCLIENTE();                                                                                        /*Prototipo para BUSCAR CLIENTES*/
    void AtualizaCLIENTE();                                                                                     /*Prototipo para ATUALIZAR CLIENTES*/
    int AtualizaDadosCLIENTE(FILE *fpt, cad_cliente cliente, endereco end, int, char[12], char[15], char[40]); /*Protitpo para RECEBER DADOS A SEREM ATUALIZADOS */
    void ExcluiCLIENTE ();
    void ImprimeDadosCLIENTE(cad_cliente cliente, endereco end);

/*Resolver problema do menu de cadastro de PJ*/
/*Função principal*/
int main(void)
{
    /*Permite uso de acentos da lingua portguesa*/
    setlocale(LC_ALL, "Portuguese");

    /*Incializa as structs  como vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "","", end };
    char op_menu = ' ';
    FILE* fp;
    /*Laço para uso dos menus*/
    do
    {
        clear();
        op_menu = menu();
        setbuf(stdin, NULL);

        /*Abre menu de gerenciamento de clintes*/
        if (op_menu == 'C' || op_menu == 'c')
        {
            GerenciarCLIENTE();
            continue;
        }
        /*Abre menu de gerenciamento de contas*/
        else if (op_menu == 'T' || op_menu == 't')
        {
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
    scanf(" %c", &op);
    clear();
    /*Opção S = Encerra o programa*/
    if (op == 'S' || op == 's')
    {
        sair();
    }
    /*Opção M = Voltar para o menu principal*/
    else if (op == 'M' || op == 'm') 
    {}
    /*Comando inválido*/
    else 
    {
        printf("Comando inválido!\n");
        mysleep();
        clear();
        Sair_Ou_Menu();
    }
}
/*Menu DE CELINTES*/
void GerenciarCLIENTE()
{
    char op_cliente;
    printf("============ Gerenciar Clientes ============\n");
    printf("Digite um comando para prosseguir:\n");
    printf("%s\n%s\n%s\n%s\n%s\n%s\n",
        "C – Cadastrar um cliente", "L – Listar todos os clientes cadastrados",
        "B – Buscar cliente já cadastrado", "A – Atualizar um cliente cadastrado",
        "E – Excluir um cliente cadastrado", "S – Sair");
    setbuf(stdin, NULL);
    scanf(" %c", &op_cliente); // recebe o comando
    clear();
    switch (op_cliente)
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
        GerenciarCLIENTE();
    }
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
    FILE* fp = fopen("clientes.dat", "rb+");

    if (fp == NULL) /*Se arquivo não existir, cria um*/
    {
        fp = fopen("clientes.dat", "wb+");

        /*Fecha caso não seja possível abrir o arquivo*/
        if (fp == NULL)
        {
            printf("Não foi possível abrir o arquivo");
            exit(1);
        }
        else {/*Incialiaza o arquivo com 100 posições em branco*/
            for (int i = 0; i < 100; i++)
            {
                fwrite(&cliente, sizeof(cad_cliente), 1, fp);
            }
            /*Fecha o arquivo para salvar as posições*/
            fclose(fp);
            /*Abre novamente para leitura e gravação*/
            fp = fopen("clientes.dat", "rb+");
        }
    }
    /*Laço para cadastro*/
    do
    {
        /*Garante que as structs serão passadas vazias para as funções de cadastro*/
        endereco end = { 0, 0, "", "", "" };
        cad_cliente cliente = { 0, "", "", "","", end };

        clear();
        setbuf(stdin, NULL);

        /*Escolher entre pessoa física ou jurídica*/
        printf("=============== Cadastrar Cliente =================\n");
        printf("Deseja cadastrar Pessoa Física ou Pessoa Júrdica?\n");
        printf("%s\n%s\n%s\n", "Pessoa FÍSICA - F", "Pessoa JURÍDICA - J", "Sair - S");
        scanf(" %c", &op);
        clear();

        if (op == 'F' || op == 'f') /* Opção Cadastrar Pessoa FÍSICA*/
        {
            /*Limpa a opção para evitar erros*/
            op = ' ';

            /*Laço para cadastro do cliente*/
            do
            {
                verCOD = RecebeDadosPessoaFISICA(fp, cliente, end);
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
                verCOD = RecebeDadosPessoaJURIDICA(fp, cliente, end);
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
    } while ((op != 'S' && op != 's') && (cont_cliente >= 0 && cont_cliente <= 100)); /*Fim do...while*/

    /*Quantidade máxima de clientes a serem cadastrados*/
    if (cont_cliente == 100)
    {
        printf("Quantidade máxima de clientes atingida!\n");
    }

    fclose(fp); /*Fecha o arquivo e salva os dados cadastrados*/
}
/*Fim cadastro_CLIENTE*/

/*Verifica se o código do cliente está entre 1 e 100*/
int VerificaCodigo(int codigo)
{
    if (codigo < 1 || codigo > 100)
    {
        clear();
        printf("Código inválido! Digite um valor entre 1 e 100.\n");
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
int VerificaCadastro(FILE* fpt, cad_cliente cliente, int codigo, char CPF_CNPJ[15])
{
    /*Posiciona o ponteiro na posição do arquivo referente ao código*/
    fseek(fpt, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Lê os dados existentes no arquivo clientes.dat nessa posição*/
    fread(&cliente, sizeof(cad_cliente), 1, fpt);

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
        rewind(fpt);
        while (!feof(fpt)) /*Varre todo o arquivo comparando o CPF ou CNPJ*/
        {
            fread(&cliente, sizeof(cad_cliente), 1, fpt);

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

int RecebeDadosPessoaFISICA(FILE* fpt, cad_cliente cliente, endereco end)
{
    int codigo, verCOD, verCAD;
    char CPF[12];

    printf("=============== Cadastrar Pessoa FÍSICA =================\n");
    printf("%s\n", "Digite o código do cliente (ENTRE 1 E 100):");
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
        scanf(" %12[^\n]", CPF);

        /*Verifica a exitencia de cliente com mesmo codigo ou cpf*/
        verCAD = VerificaCadastro(fpt, cliente, codigo, CPF);

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
            scanf(" %40[^\n]", cliente.nome);

            /*Recebe o numero de telefone*/
            printf("%s\n", "Digite o numero de telefone:");
            scanf(" %11[^\n]", cliente.telefone);

            /*Recebe o endereço*/

            /*Recebe o CEP*/
            printf("%s\n", "CEP: SOMENTE NÚMEROS");
            //printf("%s\n","CEP:");
            scanf(" %d", &cliente.end.CEP);

            /*Recebe o estado*/
            printf("%s\n", "Estado (No Formato: SP,GO):");
            scanf(" %3[^\n]", cliente.end.estado);
            toupper(cliente.end.estado[1]);
            toupper(cliente.end.estado[2]);

            /*Recebe o município*/
            printf("%s\n", "Município:");
            scanf(" %20[^\n]", cliente.end.cidade);

            /*Recebe a rua*/
            printf("%s\n", "Rua:");
            scanf(" %20[^\n]", cliente.end.rua);

            /*Recebe o número*/
            printf("Número:\n");
            scanf(" %d", &cliente.end.numero);

            clear();
            /*Fim da entrada de dados*/
            /*Posiciona o ponteiro do arquivo na posição referente ao código*/
            fseek(fpt, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);

            /*Grava os dados no arquivo clientes.dat*/
            fwrite(&cliente, sizeof(cad_cliente), 1, fpt);
        }
    }
    return 0;
}
/*Fim RecebeDadosPessoaFISICA*/

int RecebeDadosPessoaJURIDICA(FILE* fpt, cad_cliente cliente, endereco end)
{
    int codigo, verCOD, verCAD;
    char CNPJ[15];

    printf("=============== Cadastrar Pessoa JURÍDICA =================\n");
    printf("%s\n", "Digite o código do cliente (ENTRE 1 E 100):");
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
        scanf(" %15[^\n]", CNPJ);

        /*Verifica a exitencia de cliente com mesmo codigo ou cpf*/
        verCAD = VerificaCadastro(fpt, cliente, codigo, CNPJ);

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
            scanf(" %40[^\n]", cliente.nome);

            /*Recebe o numero de telefone*/
            printf("%s\n", "Digite o numero de telefone:");
            scanf(" %11[^\n]", cliente.telefone);

            /*Recebe o endereço*/

            /*Recebe o CEP*/
            printf("%s\n", "CEP: SOMENTE NÚMEROS");
            //printf("%s\n","CEP:");
            scanf(" %d", &cliente.end.CEP);

            /*Recebe o estado*/
            printf("%s\n", "Estado (No Formato: SP,GO):");
            scanf(" %3[^\n]", cliente.end.estado);
            toupper(cliente.end.estado[1]);
            toupper(cliente.end.estado[2]);

            /*Recebe o município*/
            printf("%s\n", "Município:");
            scanf(" %20[^\n]", cliente.end.cidade);

            /*Recebe a rua*/
            printf("%s\n", "Rua:");
            scanf(" %20[^\n]", cliente.end.rua);

            /*Recebe o número*/
            printf("Número:\n");
            scanf(" %d", &cliente.end.numero);

            clear();
            /*Fim da entrada de dados*/

            /*Posiciona o ponteiro do arquivo na posição referente ao código*/
            fseek(fpt, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);

            /*Grava os dados no arquivo clientes.dat*/
            fwrite(&cliente, sizeof(cad_cliente), 1, fpt);

        }
    }
    return 0;
}
/*Fim RecebeDadosPessoaJURIDICA*/

void ListarCLIENTES()
{
    int cont_cliente =0;
    /*Inicialização das structs em branco*/
    endereco end = {0, 0, "", "", ""};
    cad_cliente cliente = {0, "", "", "", "", end};

    FILE* fpt;
    fpt = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (fpt == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    } 
    
    rewind(fpt);

    /*Lê os dados de todos os clientes no arquivo*/
    while(!feof(fpt)) 
    {
        fread(&cliente, sizeof(cad_cliente), 1, fpt);

        /*Se existe cliente cadastrado*/
        if (cliente.codigo != 0)
        {
            /*Há clientes cadastrados*/
            cont_cliente=1;
            /*Imprime por ordem de código*/
            ImprimeDadosCLIENTE(cliente,end);
            printf("\n");
            
        }
    }
    if(cont_cliente == 1) 
    {
    printf("\nPressione ENTER para continuar\n");
    setbuf(stdin, NULL);
    getchar();
    clear();
    }
    else if(cont_cliente !=1) 
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
    FILE* fpt;
    fpt = fopen("clientes.dat", "rb");
    if (fpt == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1); /*Caso não seja possível abrir o arquivo, sai do programa*/
    }
    printf("============ Buscar Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código,CPF/CNJP ou nome do cliente:");
    /*Permite buscar por nome, codigo ou CPF/CNPJ*/
    scanf(" %40[^\n]", busca); 
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(fpt); /*Posiciona o ponteiro no inicio do arquivo*/

    while (!feof(fpt))
    {
        /*Lê os dados que estão no arquivo de clientes*/
        fread(&cliente, sizeof(cad_cliente), 1, fpt);

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
                ImprimeDadosCLIENTE(cliente,end);
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
    fclose(fpt);
}
/*Fim BuscaCLIENTE*/

void AtualizaCLIENTE()
{
    /*Inicialização das structs como  vazias*/
    endereco end = { 0, 0, "", "", "" };
    cad_cliente cliente = { 0, "", "", "", "", end };

    char busca[14] = "\0"; /*Variavel para achar cliente*/
    char nome[40] ="",CPF[12] = "", CNPJ[15] = ""; /*Para armazenar o nome, e o CPF/CNPJ cliente*/
    int cont_cliente = 0, cont_busca = 0, codigo = 0;
    int verATT = 0; /*verATT verfica o retorno de AtualizaDados*/

    FILE* fpt;
    fpt = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (fpt == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }
    printf("============ Atualizar Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código ou CPF/CNJP do cliente:");
    scanf(" %15[^\n]", busca); /*Permite buscar por codigo ou CPF/CNPJ*/
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(fpt);
    while (!feof(fpt))
    {
        /*Lê os dados do cliente do arquvio clientes.dat*/
        fread(&cliente, sizeof(cad_cliente), 1, fpt);

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
                strcpy(nome,cliente.nome);
                strcpy(CPF, cliente.CPF);
                strcpy(CNPJ, cliente.CNPJ);
    
                /*Passa as structs vazias para atualizar corretamente
                    O CODIGO do cliente CONTINUA O MESMO*/
                endereco end = { 0, 0, "", "", "" };
                cad_cliente cliente = { codigo, "", "", "","", end };

                verATT = AtualizaDadosCLIENTE(fpt, cliente, end, codigo, CPF, CNPJ,nome);

                if (verATT == 0)
                {
                    printf("Atualização de cliente realizada com sucesso!\n");
                    mysleep();
                    clear();
                    fclose(fpt);
                    break;
                }
                else if(verATT == 1);
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

/*Recebe a struct vazia, mas com os dados necessários armazenados em variáveis*/
int AtualizaDadosCLIENTE(FILE* fpt, cad_cliente cliente, endereco end, int codigo, char CPF[12], char CNPJ[15], char nome[40]) /*Não é permitido atualizar o código do cliente*/
{
    printf("=============== Atualizar Cliente =================\n");
    printf("Nome do cliente a ser atualizado: %s\n\n", nome);

    /*Se o CPF existe, não é pessoa jurídica*/
    if (strcmp(CPF, "") != 0)
    {
        /*Recebe o CPF do cliente*/
        printf("%s\n%s\n", "Formato CPF: SOMENTE NÚMEROS!", "Digite o CPF do cliente:");
        scanf(" %12[^\n]", CPF);
    }

    /*Se CNPJ existe, não é pessoa física*/
    else if (strcmp(CNPJ, "") != 0)
    {
        /*Recebe o CNPJ do cliente*/
        printf("%s\n%s\n", "Formato CNPJ: SOMENTE NÚMEROS!", "Digite o CNPJ do cliente:");
        scanf(" %15[^\n]", CNPJ);
    }

  /*Posiciona o ponteiro de acordo com o código do cliente */
    fseek(fpt, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Exclui os dados do cliente no arquivo para atualizar completamente. O CODIGO CONTINUA O MESMO!*/
    fwrite(&cliente, sizeof(cad_cliente), 1, fpt);

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
    scanf(" %40[^\n]", cliente.nome);

    /*Recebe o numero de telefone*/
    printf("%s\n", "Digite o numero de telefone:");
    scanf(" %11[^\n]", cliente.telefone);

    /*Recebe o endereço*/

    /*Recebe o CEP*/
    printf("%s\n", "CEP: SOMENTE NÚMEROS");
    scanf(" %d", &cliente.end.CEP);

    /*Recebe o estado*/
    printf("%s\n", "Estado (No Formato: SP,GO):");
    scanf(" %3[^\n]", cliente.end.estado);

    /*Recebe o município*/
    printf("%s\n", "Município:");
    scanf(" %20[^\n]", cliente.end.cidade);

    /*Recebe a rua*/
    printf("%s\n", "Rua:");
    scanf(" %20[^\n]", cliente.end.rua);

    /*Recebe o número*/
    printf("Número:\n");
    scanf(" %d", &cliente.end.numero);

    clear();
    /*Fim da entrada de dados*/

    /*Posiciona o ponteiro de acordo com o código do cliente */
    fseek(fpt, (codigo - 1) * sizeof(cad_cliente), SEEK_SET);

    /*Grava os novos dados no arquivo*/
    fwrite(&cliente, sizeof(cad_cliente), 1, fpt);

    return 0;
}
/*Fim AtualizaDadosCLIENTE*/

void ExcluiCLIENTE()
{
    /*Structs para receber os dados do arquivo*/
    endereco end = {0, 0, "", "", ""};
    cad_cliente cliente = {0, "", "", "", "", end};
    
    /*Structs em branco para gravar no arquivo*/
    endereco blank_end = {0, 0, "", "", ""};
    cad_cliente blank_cliente = {0, "", "", "", "", blank_end};
    

    char op =' ';
    int codigo=0,cont_cliente =0,cont_busca=0;
    char busca[15];

    FILE *fpt;
    fpt = fopen("clientes.dat", "rb+");
    /*Caso não seja possível abrir o arquivo, sai do programa*/
    if (fpt == NULL)
    {
        printf("%s\n", "Não foi possível abrir o arquivo de clientes.\nArquivo de Clientes não foi criado.");
        mysleep();
        exit(1);
    }
    
    printf("============ Excluir Cliente ============\n");
    printf("%s\n%s\n", "Formato CPF e CNPJ: SOMENTE NÚMEROS", "Digite o código ou CPF/CNJP do cliente:");
    scanf(" %15[^\n]", busca); /*Permite buscar por codigo ou CPF/CNPJ*/
    clear();

    codigo = atoi(busca); /*Transforma a string em int para verificar o codigo*/

    rewind(fpt);

    while(!feof(fpt)) 
    {
        /*Lê os dados do cliente do arquvio clientes.dat*/
        fread(&cliente, sizeof(cad_cliente), 1, fpt);

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
                ImprimeDadosCLIENTE(cliente,end);

                printf("%s\n","Deseja prosseguir com a exclusão do cliente?");
                printf("%s\n%s\n","Sim- S","Não - N");
                scanf(" %c",&op);
                clear();
                if ((op != 'S' && op != 's') && (op != 'N' && op != 'n'))
                {
                    printf("Comando Inválido!\n");
                    mysleep();
                    clear();
                    continue;
                }
                else break;
                } while(1);

                if(op== 'S' || op == 's') 
                {
                    /*Posiciona o ponteiro na posição onde o cliente está cadastrado*/
                    fseek(fpt, (cliente.codigo - 1) * sizeof(cad_cliente), SEEK_SET);
                    
                    /*Grava os dados da struct em branco no arquivo*/
                    fwrite (&blank_cliente, sizeof(cad_cliente), 1, fpt);
                    
                    printf("Exclusão realizada com sucesso!\n");
                    mysleep();
                    clear();
                    break;
                }
                if (op == 'N' || op == 'n' ) 
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
    fclose(fpt);/*Fecha o arquivo*/
}
/*Fim ExcluiCLIENTE*/
void ImprimeDadosCLIENTE (cad_cliente cliente, endereco end) 
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