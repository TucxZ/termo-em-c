#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "jogo.h"

const char *obter_txt(int max)
{
    switch (max)
    {
    case 4:
        return "dados/quatro_letras.txt";
    case 5:
        return "dados/cinco_letras.txt";
    case 6:
        return "dados/seis_letras.txt";
    case 7:
        return "dados/sete_letras.txt";
    default:
        return NULL;
    }
}

// Remove quebras de linha e muda as palavras para maiúsculas
void limpar_e_maiuscula(char *str)
{
    str[strcspn(str, "\r\n")] = 0;
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int sortear_palavra(char *palavra_secreta, int max)
{
    const char *arquivo = obter_txt(max);
    FILE *file = fopen(arquivo, "r");
    if (!file)
        return 0;

    char linha[100];
    int linhas = 0;

    // Conta quantas palavras existem

    while (fgets(linha, sizeof(linha), file))
    {
        linhas++;
    }

    if (linhas == 0)
    {
        fclose(file);
        return 0;
    }

    srand(time(NULL));
    int linha_sorteada = rand() % linhas;

    fseek(file, 0, SEEK_SET);
    int atual = 0;
    while (fgets(linha, sizeof(linha), file))
    {
        if (atual == linha_sorteada)
        {
            limpar_e_maiuscula(linha);
            strncpy(palavra_secreta, linha, max);
            palavra_secreta[max] = '\0';
            break;
        }
        atual++;
    }

    fclose(file);
    return 1;
}

int validar_palavra(const char *palavra, int max)
{
    if (strlen(palavra) != max)
    {
        return 0;
    }
    for (int i = 0; i < max; i++)
    {
        if (!isalpha((unsigned char)palavra[i]))
        {
            return 0;
        }
    }
    return 1;
}

int palavra_existe_no_arquivo(const char *palavra, int max)
{
    const char *arquivo = obter_txt(max);
    FILE *file = fopen(arquivo, "r");
    if (!file)
        return 0;

    char linha[100];
    int encontrada = 0;

    while (fgets(linha, sizeof(linha), file))
    {
        limpar_e_maiuscula(linha);
        if (strcmp(linha, palavra) == 0)
        {
            encontrada = 1;
            break;
        }
    }

    fclose(file);
    return encontrada;
}

int avaliar_tentativa(const char *tentativa, const char *palavra_secreta, int max)
{
    int acertos = 0;
    // Vetor para controlar quais letras da palavra secreta já foram "usadas" no feedback amarelo
    int usada_na_secreta[MAX_LETRAS] = {0};
    int status_letras[MAX_LETRAS] = {0}; // 2 = Verde, 1 = Amarelo, 0 = Cinza

    // verifica letras verdes lugar certo!!
    for (int i = 0; i < max; i++)
    {
        if (tentativa[i] == palavra_secreta[i])
        {
            status_letras[i] = 2;
            usada_na_secreta[i] = 1;
            acertos++;
        }
    }

    // Passo 2: Verificar Letras Amarelas Lugar errado!!
    for (int i = 0; i < max; i++)
    {
        if (status_letras[i] == 2)
            continue;

        for (int j = 0; j < max; j++)
        {
            if (!usada_na_secreta[j] && tentativa[i] == palavra_secreta[j])
            {
                status_letras[i] = 1;
                usada_na_secreta[j] = 1;
                break;
            }
        }
    }

    // resultado colorido no terminal
    for (int i = 0; i < max; i++)
    {
        if (status_letras[i] == 2)
        {
            printf("\033[42;30m %c \033[0m", tentativa[i]); // Fundo Verde, Letra Preta
        }
        else if (status_letras[i] == 1)
        {
            printf("\033[43;30m %c \033[0m", tentativa[i]); // Fundo Amarelo, Letra Preta
        }
        else
        {
            printf("\033[40;37m %c \033[0m", tentativa[i]); // Fundo Cinza/Preto, Letra Branca
        }
    }
    printf("\n\n");

    return (acertos == max);
}

/*Funções status*/
void carregarStatus(status *e){
    FILE *arquivo = fopen("dados/historico.dat", "rb");

    if(arquivo == NULL){
        e ->partidas=0;
        e ->vitorias=0;
        e ->derrotas=0;
        return;
    }

    fread(e, sizeof(status), 1, arquivo);
    fclose(arquivo);
}

void salvarStatus(status *e){
    FILE *arquivo = fopen("dados/historico.dat", "wb");

    if(arquivo == NULL){
        return;

        fwrite(e, sizeof(status), 1, arquivo);
        fclose(arquivo);
    }
}

void registrarDerrota(status *e){
    e ->partidas++;
    e ->derrotas++;

    salvarStatus(e);
}

void registrarVitoria(status *e){
    e ->partidas++;
    e ->vitorias++;

    salvarStatus(e);
}

void mostrarStatus(status *e){
    printf("\n====================\n");
    printf("   ESTATISTICAS\n");
    printf("====================\n");

    printf("Partidas jogadas: %d\n", e->partidas);
    printf("Vitorias: %d\n", e->vitorias);
    printf("Derrotas: %d\n", e->derrotas);

    printf("====================\n\n");
}
