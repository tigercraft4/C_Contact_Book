/* 
 * File:   main.c
 * Author: Francisco Fernandes
 *
 * Created on 17 de Dezembro de 2017, 20:17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LETRAS 30 //define o num. máximo de letras que um nome ou empresa pode ter
#define MAX_CONTACTOS 30//define o num. máximo de contactos que a agenda pode ter

struct template {//define o template (modelo) da estrutura que armazena informação
    char nome [MAX_LETRAS];
    char sexo [10];
    int NIF;
    short int ano;
    short int mes;
    short int dia;
    short int peso;
    char empresa [MAX_LETRAS];
};
//as próximas linhas prototipam as funções usadas no programa
void cabecalho(void);
void MenuCriar(void);
struct template contactos[MAX_CONTACTOS]; //define variável contactos do tipo 'template'
int le_ficheiro(void);
void escreve_ficheiro(int k);
void modifica_ficheiro(int k);
void menu_modificar(int k);
void menu_ordenar(void);
void procura_contacto(void);
int contador_contactos;

FILE *ptr_ficheiro; //na linguagem C, os ficheiros devem ser referenciados por ponteiros,
char *ptr_nomes[MAX_LETRAS];
char *ptr_empresa[MAX_LETRAS];

int main(void) {
    int escolhaMenu1, escolha_contacto, i;

    contador_contactos = le_ficheiro(); //chama a função que cria (caso não exista) abre e lê o conteudo do ficheiro
    cabecalho(); //chama a função que imprime o cabeçalho do programa
    do {
        printf("------------------------------\n"
                "1 – Introduzir contactos\n"
                "2 – Modificar contactos\n"
                "3 – Visualizar contactos\n"
                "4 – Procurar contactos\n"
                "0-  Sair\n"
                "------------------------------\n"
                "Indique qual a sua opção:\n "
                "-->>___\b\b");
        scanf(" %d", &escolhaMenu1);
        getchar();

        switch (escolhaMenu1) {
            case 1://caso em que se deseja introduzir um novo contacto.
                MenuCriar();
                break; //o break sai do case e volta para o do while()
            case 2://caso pretenda modificar um contacto.
                printf("Indique o contacto que pretende modificar: ");
                scanf("%d", &escolha_contacto);
                getchar();
                if (escolha_contacto < 1 || escolha_contacto > contador_contactos) {
                    printf("A escolha não e valida tente novamente\n");
                    break;
                }
                printf("o contacto escolhido foi: %s", contactos[escolha_contacto - 1].nome);
                menu_modificar(escolha_contacto - 1);
                modifica_ficheiro(escolha_contacto - 1);
                break;
            case 3://caso pretenda visualizar contacto.
                menu_ordenar();
                break;
            case 4://caso pretendaprocurar contactos.
                procura_contacto();
                break;
            case 0://caso pretenda sair do programa.
                printf("Obrigado por usar a agenda maravilha!!\n");
                break;
            default://caso nao introduza uma escolha valida.
                printf("A escolha não e valida tente novamente\n");
                break;
        }

    } while (escolhaMenu1 != 0);

    return (0);
}

void cabecalho(void) {
    printf("----------------------------------\n"
            "Super agenda de Contactos v0.658\n"
            "Criada por Francisco Fernandes\n"
            "----------------------------------\n");
    /*
     * Imprime o cabacalho do programa
     */
}

void MenuCriar(void) {
    int i, dia_actual, mes_actual, ano_actual, dia_nascimento, mes_nascimento, ano_nascimento, dia_calculado, ano_calculado, mes_calculado;

    if (contador_contactos == MAX_CONTACTOS)
        printf("Numero maximo atingido");
        /*As próximas linhas só são executadas caso ainda seja possível criar um contacto.*/
    else {
        printf("Contacto n:%d\n", contador_contactos + 1);
        printf("------------------------------------------\n");
        printf("Introduza o nome: ");
        gets(contactos[contador_contactos].nome);
        if (contactos[contador_contactos].nome[0] >= 97)
            contactos[contador_contactos].nome [0] = contactos[contador_contactos].nome[0] - 32;

        for (i = 1; i < MAX_LETRAS; i++)
            if (contactos[contador_contactos].nome[i] == ' ')
                if (contactos[contador_contactos].nome[i + 1] >= 97)
                    contactos[contador_contactos].nome[i + 1] = contactos[contador_contactos].nome[i + 1] - 32;

        printf("Introduza o sexo: ");
        gets(contactos[contador_contactos].sexo);

        printf("Introduza empresa do %s: ", contactos[contador_contactos].nome);
        gets(contactos[contador_contactos].empresa);

        printf("Introduza a data de hoje\n");
        printf("Introduza o dia:\n");
        scanf("%d", &dia_actual);
        printf("Introduza o mes:\n");
        scanf("%d", &mes_actual);
        printf("Introduza o ano:\n");
        scanf("%d", &ano_actual);
        printf("\nItroduza a data do seu nascimento\n");
        printf("Inroduza o dia:\n");
        scanf("%d", &dia_nascimento);
        printf("Introduza o mes:\n");
        scanf("%d", &mes_nascimento);
        printf("Introduza o ano:\n");
        scanf("%d", &ano_nascimento);

        if (dia_nascimento > dia_actual) {
            mes_actual = mes_actual - 1;
            dia_actual = dia_actual + 30;
        }
        if (mes_nascimento > mes_actual) {
            ano_actual = ano_actual - 1;
            mes_actual = mes_actual + 12;
        }

        contactos[contador_contactos].dia = dia_actual - dia_nascimento;
        contactos[contador_contactos].mes = mes_actual - mes_nascimento;
        contactos[contador_contactos].ano = ano_actual - ano_nascimento;

        printf("Introduza o NIF: ");
        scanf(" %d", &contactos[contador_contactos].NIF);
        getchar();

        printf("Introduza o peso: ");
        scanf(" %d", &contactos[contador_contactos].peso);
        getchar();

        escreve_ficheiro(contador_contactos); //chama função que escreve (guarda) no ficheiro.
        contador_contactos = contador_contactos + 1; //incrementa o contador por forma a 'saltar' para a próxima
    }
}

int le_ficheiro(void) {
    /*Esta função verifica a existência do ficheiro, caso não exista cria-o e caso exista lê-o.*/
    int indice = 0;
    if ((ptr_ficheiro = fopen("agenda.dat", "a+b")) == NULL) {
        fputs("Nao e possivel abrir o ficheiro 'agenda.dat'\n", stderr);
        exit(1);
    }
    rewind(ptr_ficheiro); // vai para o inicio do ficheiro.
    while (indice < MAX_CONTACTOS && fread(&contactos[indice], sizeof (struct template), 1, ptr_ficheiro) == 1)
        indice++;
    fclose(ptr_ficheiro); //fecha ficheiro.
    return (indice);
}

void escreve_ficheiro(int k) {
    if ((ptr_ficheiro = fopen("agenda.dat", "a+b")) == NULL) {
        fputs("Nao e possivel abrir o ficheiro 'agenda.dat'\n", stderr);
        exit(1);
    }
    fwrite(&contactos[k], sizeof (struct template), 1, ptr_ficheiro); //escreve o conteudo
    fclose(ptr_ficheiro);
}

void modifica_ficheiro(int k) {
    if ((ptr_ficheiro = fopen("agenda.dat", "r+b")) == NULL) {
        fputs("Nao e possivel abrir o ficheiro 'agenda.dat'\n", stderr);
        exit(1);
    }
    rewind(ptr_ficheiro);
    fseek(ptr_ficheiro, sizeof (struct template)*k, SEEK_CUR); //coloca o ponteiro de escrita no devido local de edição
    fwrite(&contactos[k], sizeof (struct template), 1, ptr_ficheiro);
    fclose(ptr_ficheiro);
    /*  Esta função usa permissões de escrita ('r') diferentes das anteriores ('a').
       Isto permite escrever por cima (overwrite) dos conteúdos existentes.
     */
}

void menu_modificar(int k) {
    int escolhaMenuModificar, i, dia_actual, mes_actual, ano_actual, dia_nascimento, mes_nascimento, ano_nascimento, dia_calculado, ano_calculado, mes_calculado;

    do {
        printf("\n----Escolha a opcao----\n"
                "1- Modificar o nome\n"
                "2- Modificar o sexo\n"
                "3- Modificar a empresa\n"
                "4- Modificar a data de nascimento\n"
                "5- Modificar o NIF\n"
                "6- Modificar o peso\n"
                "0- voltar ao menu anterior\n\n"
                "-->>___\b\b");
        scanf(" %d", &escolhaMenuModificar);
        getchar();
        switch (escolhaMenuModificar) {
            case 1://caso deseje modificar o nome
                printf("indique qual e o novo  nome:: ");
                gets(contactos[k].nome);
                if (contactos[k].nome[0] >= 97)
                    contactos[k].nome [0] = contactos[k].nome[0] - 32;

                for (i = 1; i < MAX_LETRAS; i++)
                    if (contactos[k].nome[i] == ' ')
                        if (contactos[k].nome[i + 1] >= 97)
                            contactos[k].nome[i + 1] = contactos[k].nome[i + 1] - 32;
                break;
            case 2://caso deseje modificar o sexo
                printf("indique qual e o novo sexo: ");
                gets(contactos[k].sexo);
                modifica_ficheiro(k);
                break;
            case 3://caso deseje modificar a empresa
                printf("indique qual e a nova empresa: ");
                gets(contactos[k].empresa);
                break;
            case 4://caso deseje modificar o nome
                printf("Introduza a data de hoje\n");
                printf("Introduza o dia:\n");
                scanf("%d", &dia_actual);
                printf("Introduza o mes:\n");
                scanf("%d", &mes_actual);
                printf("Introduza o ano:\n");
                scanf("%d", &ano_actual);
                printf("\nItroduza a data do seu nascimento\n");
                printf("Inroduza o dia:\n");
                scanf("%d", &dia_nascimento);
                printf("Introduza o mes:\n");
                scanf("%d", &mes_nascimento);
                printf("Introduza o ano:\n");
                scanf("%d", &ano_nascimento);

                if (dia_nascimento > dia_actual) {
                    mes_actual = mes_actual - 1;
                    dia_actual = dia_actual + 30;
                }
                if (mes_nascimento > mes_actual) {
                    ano_actual = ano_actual - 1;
                    mes_actual = mes_actual + 12;
                }

                contactos[contador_contactos].dia = dia_actual - dia_nascimento;
                contactos[contador_contactos].mes = mes_actual - mes_nascimento;
                contactos[contador_contactos].ano = ano_actual - ano_nascimento;
                break;
            case 5://caso deseje modificar o NIF
                printf("indique qual e o novo NIF: ");
                scanf(" %d", &contactos[k].NIF);
                getchar();
                break;
            case 6://caso deseje modificar o peso
                printf("indique qual e o novo Peso: ");
                scanf(" %d", &contactos[k].peso);
                getchar();
                break;
            case 0://caso deseje voltar ao menu anterior
                printf("Voce obtou por voltar ao menu anterior:\n");
                break;
            default://caso escolha outra opcao
                printf("Opcao Invalida, Tente Novamente\n");
                break;
        }
    } while (escolhaMenuModificar != 0);
}

void menu_ordenar(void) {

    int i, j, index, escolhaOrdernar;

    char *temp;
    for (index = 0; index < MAX_CONTACTOS; index++) {//ciclo para atribuir ponteiros aos nomes
        ptr_nomes[index] = contactos[index].nome;
        ptr_empresa[index] = contactos[index].empresa;
    }
    printf("------------------------------\n"
            "Como pretende ordenar:\n"
            "1-Nome\n"
            "2-Empresa\n"
            "3-Ordem de introducao\n"
            "0-Voltar ao menu anterior\n"
            "------------------------------\n"
            "-->>___\b\b");
    scanf(" %d", &escolhaOrdernar);
    getchar();
    switch (escolhaOrdernar) {
        case 1://caso pretenda ordenar por nome

            for (i = 0; i < contador_contactos - 1; i++)
                for (j = i + 1; j < contador_contactos; j++)
                    if (strcmp(ptr_nomes[i], ptr_nomes[j]) > 0) {
                        temp = ptr_nomes[j];
                        ptr_nomes[j] = ptr_nomes[i];
                        ptr_nomes[i] = temp;
                    }
            printf("Eis a lista dos nomes ordenados:\n");
            for (i = 0; i < contador_contactos; i++) {//imprime lista de nomes organizada, recorrendo aos ponteiros
                printf("\n---»»");
                printf("%s\n", ptr_nomes[i]);

                printf("----------------------------------------\n");
            };
            for (index = 0; index < contador_contactos; index++) //após impressão, volta a colocar os ponteiros
                ptr_empresa[index] = contactos[index].empresa;
            break;

        case 2://caso pretenda ordenar por empresa
            for (i = 0; i < contador_contactos - 1; i++)
                for (j = i + 1; j < contador_contactos; j++)
                    if (strcmp(ptr_empresa[i], ptr_empresa[j]) > 0) {
                        temp = ptr_empresa[j];
                        ptr_empresa[j] = ptr_empresa[i];
                        ptr_empresa[i] = temp;
                    }
            printf("Eis a lista de empresas ordenadas:\n");
            printf("----------------------------------------\n");
            for (i = 0; i < contador_contactos; i++) {//imprime lista de nomes organizada, recorrendo aos ponteiros
                printf("\n---»»");
                printf("%s\n", ptr_empresa[i]);

                printf("----------------------------------------\n");
            };
            for (index = 0; index < contador_contactos; index++) //após impressão, volta a colocar os ponteiros
                ptr_empresa[index] = contactos[index].empresa;
            break;
        case 3://caso pretenda ordenar por ordem de intodução
            for (i = 0; i < contador_contactos; i++)//imprime lista de nomes organizada, por ordem de introduçao
                printf("---------- Contacto %d ----------\n"

                    "Nome: %-40s \n"
                    "Sexo:%-8s \n"
                    "Empresa:%-40d \n"
                    "Idade: %d anos %d meses %d dias\n"
                    "NIF:%-8d \n"
                    "Peso:%-10d \n", i + 1, contactos[i].nome, contactos[i].sexo, contactos[i].empresa, contactos[i].ano, contactos[i].mes, contactos[i].dia, contactos[i].NIF, contactos[i].peso);
            printf("--------------------------------\n");
            break;
        case 0://caso pretenda voltar ao mebu principal
            printf("Voce obtou por voltar ao menu anterior:\n");
            break;
        default://Caso o utilizador introduza outro caracter
            printf("Opcao Invalida, Tente Novamente\n");
            break;
    }

}
void procura_contacto(void) {
    int escolhaProcuraContacto, i, procura_ano, procura_peso, procura_NIF, procura_dia, procura_mes, contactos_encontrados = 0;
    char string_procura[MAX_LETRAS];
    do {
        printf("\n----Escolha a opcao----\n"
                "1- Procurar o nome\n"
                "2- Procurar o sexo\n"
                "3- Procurar a empresa\n"
                "4- Procurar o ano de nascimento\n"
                "5- Procurar o mes de nascimento\n"
                "6- Procurar o dia de nascimento\n"
                "7- Procurar o NIF\n"
                "8- Procurar o peso\n"
                "0- voltar ao menu anterior\n\n"
                "-->>___\b\b");
        scanf(" %d", &escolhaProcuraContacto);
        getchar();

        switch (escolhaProcuraContacto) {
            case 1://caso deseje procurar por nome
                contactos_encontrados = 0;
                printf("indique qual e o Nome a procurar:\n"
                        "-->>__\b\b ");
                gets(string_procura);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (strcmp(string_procura, contactos[i].nome) == 0) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Ano: %d \n", contactos[i].ano);
                        printf("NIF: %d \n", contactos[i].NIF);
                        printf("Peso: %d \n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 2://caso deseje procurar por empresa
                contactos_encontrados = 0;
                printf("indique qual e a empresa a procurar:\n"
                        "-->>__\b\b ");
                gets(string_procura);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (strcmp(string_procura, contactos[i].sexo) == 0) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Data de nascimento: %d-%d-%d\n", contactos[i].ano, contactos[i].mes, contactos[i].dia);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;

            case 3://caso deseje procurar por sexo
                contactos_encontrados = 0;
                printf("indique qual e o Sexo a procurar:\n"
                        "-->>__\b\b ");
                gets(string_procura);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (strcmp(string_procura, contactos[i].sexo) == 0) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Data de nascimento: %d-%d-%d\n", contactos[i].ano, contactos[i].mes, contactos[i].dia);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 4://caso deseje procurar por dia de nascimento
                contactos_encontrados = 0;
                printf("indique qual e o dia de nascimento a procurar:\n"
                        "-->>__\b\b ");
                scanf("%d", &procura_dia);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (procura_dia == contactos[i].dia) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Data de nascimento: %d-%d-%d\n", contactos[i].ano, contactos[i].mes, contactos[i].dia);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 5://caso deseje procurar por mes de nascimento
                contactos_encontrados = 0;
                printf("indique qual e o mes de nascimento a procurar:\n"
                        "-->>__\b\b ");
                scanf("%d", &procura_mes);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (procura_mes == contactos[i].ano) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Data de nascimento: %d-%d-%d\n", contactos[i].ano, contactos[i].mes, contactos[i].dia);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 6://caso deseje procurar por ano de nascimento
                contactos_encontrados = 0;
                printf("indique qual e o ano de nascimento a procurar:\n"
                        "-->>__\b\b ");
                scanf("%d", &procura_ano);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (procura_ano == contactos[i].ano) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Data de nascimento: %d-%d-%d\n", contactos[i].ano, contactos[i].mes, contactos[i].dia);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 7://caso deseje procurar por nif
                contactos_encontrados = 0;
                printf("indique qual e o NIF a procurar:\n"
                        "-->>__\b\b ");
                scanf("%d", &procura_NIF);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (procura_NIF == contactos[i].NIF) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Ano: %d\n", contactos[i].ano);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 8://caso deseje procurar por peso
                contactos_encontrados = 0;
                printf("indique qual e o peso a procurar:\n"
                        "-->>__\b\b ");
                scanf("%d", &procura_peso);
                printf("------------------------------\n");
                printf("------Resultados da pesquisa-------\n");
                printf("------------------------------\n");
                for (i = 0; i < MAX_CONTACTOS; i++)
                    if (procura_peso == contactos[i].peso) {
                        printf("Contacto n: %d\n ", i + 1);
                        printf("------------------------------\n");
                        printf("Nome: %s\n ", contactos[i].nome);
                        printf("Sexo: %s\n", contactos[i].sexo);
                        printf("Empresa: %s\n", contactos[i].empresa);
                        printf("Ano: %d\n", contactos[i].ano);
                        printf("NIF: %d\n", contactos[i].NIF);
                        printf("Peso: %d\n", contactos[i].peso);

                        printf("------------------------------\n");
                        contactos_encontrados++;
                    }
                if (contactos_encontrados == 0)
                    printf("Nao foram encontrados contactos\n");
                break;
            case 0:
                printf("Voce obtou por voltar ao menu anterior:\n");
                break;
            default:
                printf("Opcao Invalida, Tente Novamente");
                break;
        }
    } while (escolhaProcuraContacto != 0);

}