#include "libs.h"

#define MAX_MSG_SIZE 100

typedef struct {
    int id;
    char msg[MAX_MSG_SIZE];
} Message;

int menu(){
    printf("O que deseja fazer?\n");
    printf("1 - Enviar mensagem\n");
    printf("2 - Listar mensagens\n");
    printf("3 - Sair\n");
    printf("Digite sua opcao: ");
    int opcao;
    scanf("%d", &opcao);
    return opcao;
}

void listaMensagens(){
    // Listar as mensagem no espaço de memoria:

}

int main() {
    key_t key = generateKey();
    // Acessamos o segmento de memoria compartilhada
    int shmid = shmget(key, sizeof(Message),0);
    // Valida se houve um erro no acesso:
    shmid == -1 ? perror("Erro ao acessar o segmento de memoria compartilhada"), exit(1)
    :
    printf("Segmento de memoria compartilhada acessado com sucesso!\n");

    // Associa o segmento de memoria compartilhada ao processo
    Message *shared_mem = (Message *) shmat(shmid, 0, 0);
    // Valida se houve um erro:
    shared_mem == (void *) -1 ? perror("Erro ao associar o segmento de memoria compartilhada ao processo"), exit(1)
    :
    printf("Segmento de memoria compartilhada associado ao processo com sucesso!\n");

    // Inicializa o id
    int client_id = getpid();
    shared_mem->id = client_id;

    printf("Cliente %d conectado ao servidor!\n", client_id);

    while (1) {
        //menu:
        int opcao = menu();
        if (opcao == 3) {
            printf("Cliente %d desconectado do servidor!\n", client_id);
            break;
        }
        if (opcao == 2) {
            printf("Mensagens:\n");
            listaMensagens();
            continue;
        }
        if (opcao == 1) {
            printf("Digite a mensagem: ");
            char trash[100];
            fgets(trash, 100, stdin);
            fgets(shared_mem->msg, MAX_MSG_SIZE, stdin);
            printf("Mensagem: %s", shared_mem->msg);
            printf("Mensagem enviada com sucesso!\n");
            size_t len = strlen(shared_mem->msg);
            if(len > 0 && shared_mem->msg[len-1] == '\n') {
                shared_mem->msg[len-1] = '\0';
            }
            while (shared_mem->id != -1)
            {
                sleep(1);
            }
            continue;
        }
    }
    shmdt(shared_mem) == 0 ? printf("Segmento de memoria compartilhada desassociado do processo com sucesso!\n") : perror("Erro ao desassociar o segmento de memoria compartilhada do processo");

    printf("%d", shmid);
    return 0;
}