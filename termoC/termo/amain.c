#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jogo.h"
#include "rlutil.h"

int main()
{
    char palavra_secreta[MAX_LETRAS + 1];
    char tentativa[100];
    int o, max, chances;
    int ganhou;

    status stats;
    carregarStatus(&stats);

    do
    {
        system("cls");
        chances = MAX_TENTATIVAS;
        ganhou = 0;
        o = -1;

        printf("=== TERMO EM C ===\n");

printf("\nEscolha um modo de jogo:\n");
printf("1 - Facil (4 Letras)\n");
printf("2 - Normal (5 Letras)\n");
printf("3 - Medio (6 Letras)\n");
printf("4 - Dificil (7 Letras)\n");
printf("5 - Ver status\n");
printf("0 - Sair\n\n");

printf("Opcao: ");
        scanf("%d", &o);

        while (getchar() != '\n')
            ;

        if (o == 0)
        {
            msleep(250);
            printf("\nEncerrando...");
            msleep(1500);
            break;
        }

        switch (o)
        {
        case 1:
            max = 4;
            break;
        case 2:
            max = 5;
            break;
        case 3:
            max = 6;
            break;
        case 4:
            max = 7;
            break;
        case 5:
            system("cls");

            mostrarStatus(&stats);

            printf("Pressione ENTER para voltar...");
            getchar();

            continue;
        default:
            printf("\033[31mErro: Opcao Invalida.\033[0m\n\n");
            msleep(1500);
            continue;
        }

        const char *arquivo = obter_txt(max);

        if (!sortear_palavra(palavra_secreta, max))
        {
            printf("\033[31mErro: Nao foi possivel carregar ou ler o arquivo '%s'.\033[0m", arquivo);
            msleep(1500);
            return 1;
        }

        printf("\nUma palavra de %d letras foi sorteada. Voce tem %d chances!\n\n", max, chances);

        msleep(1500);
        system("cls");

        while (chances > 0 && !ganhou)
        {
            printf("Tentativa [%d/%d] -> Digite uma palavra: ", (MAX_TENTATIVAS - chances + 1), MAX_TENTATIVAS);
            if (fgets(tentativa, sizeof(tentativa), stdin) == NULL)
                break;

            tentativa[strcspn(tentativa, "\r\n")] = 0;
            for (int i = 0; tentativa[i]; i++)
                tentativa[i] = toupper((unsigned char)tentativa[i]);

            // validação de 5-7 letras
            if (!validar_palavra(tentativa, max))
            {
                printf("\033[31mErro: A palavra deve conter exatamente %d letras (sem acentos ou numeros).\033[0m\n\n", max);
                msleep(1500);
                continue;
            }

            if (!palavra_existe_no_arquivo(tentativa, max))
            {
                printf("\033[31mErro: Essa palavra nao existe no banco de dados: %s\033[0m\n\n", arquivo);
                msleep(1500);
                continue;
            }

            ganhou = avaliar_tentativa(tentativa, palavra_secreta, max);
            chances--;
        }

        if (ganhou)
        {
            registrarVitoria(&stats);
            printf("\033[32mParabens! Voce acertou a palavra secreta!\033[0m\n\n");
        }
        else
        {
            registrarDerrota(&stats);
            printf("\033[31mFim de jogo! Voce esgotou suas chances. A palavra era: %s\033[0m\n\n", palavra_secreta);
        }

        msleep(3000);

    } while (o != 0);

    return 0;
}