#ifndef JOGO_H
#define JOGO_H

#define MAX_LETRAS 7
#define MAX_TENTATIVAS 10


/*Adição de status*/
typedef struct{
    int partidas;
    int vitorias;
    int derrotas;
}   status;

void carregarStatus(status *e);
void salvarStatus(status *e);
void registrarVitoria(status *e);
void registrarDerrota(status *e);
void mostrarStatus(status *e);
/*End status*/

const char *obter_txt(int max);

int sortear_palavra(char *palavra_secreta, int max);

int validar_palavra(const char *palavra, int max);

int palavra_existe_no_arquivo(const char *palavra, int max);

int avaliar_tentativa(const char *tentativa, const char *palavra_secreta, int max);

#endif