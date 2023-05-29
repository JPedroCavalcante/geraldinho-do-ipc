#include "libs.h"

#define MAX_MSG_SIZE 100

typedef struct {
    int id;
    char message[MAX_MSG_SIZE];
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

int main() {
    key_t key = generateKey();
    // Acessamos o segmento de memoria compartilhada
    int shmid = shmget(key, sizeof(Message),0);
    // Valida se houve um erro no acesso:
    if (shmid == -1) {
        perror("Erro ao criar o segmento de memoria compartilhada");
        exit(1);
    }

    // Associa o segmento de memoria compartilhada ao processo
    Message *shared_mem = (Message *)shmat(shmid, NULL, 0);
    // Valida se houve um erro:
    if (shared_mem == (void *)-1) {
        perror("Erro ao anexar o segmento de memoria compartilhada");
        exit(1);
    }

    printf("Cliente iniciado\n");

    // Inicializa o id
    int client_id = getpid();

    printf("Cliente %d conectado ao servidor!\n", client_id);

    while (1) {
        //menu:
        printf("O que deseja fazer?\n");
        printf("1 - Enviar mensagem\n");
        printf("2 - Ver mensagem\n");
        printf("Qualquer tecla - Sair\n");
        printf("Digite sua opcao: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("Digite a mensagem: ");
            //Apagar a mensagem atual do servidor
            memset(shared_mem->message, 0, MAX_MSG_SIZE);
            //Ler a mensagem do cliente
            char trash[MAX_MSG_SIZE];
            fgets(trash, MAX_MSG_SIZE, stdin);
            char msg[MAX_MSG_SIZE];
            fgets(msg, MAX_MSG_SIZE, stdin);
            //Remover o \n do final da string
            if(msg[strlen(msg) - 1] == '\n') msg[strlen(msg) - 1] = '\0';

            //Copiar a mensagem para a memoria compartilhada
            strcpy(shared_mem->message, msg);
            //Enviar a mensagem para o servidor
            shared_mem->id = client_id;
            printf("Mensagem enviada para o servidor!\n");
            continue;
        }

        else if (opcao == 2) {
            printf("----------------------------------------\n");
            printf("|                                       |\n");
            printf("| -> Mensagem recebida do servidor: %s\n", shared_mem->message);
            printf("|                                       |\n");
            printf("----------------------------------------\n");
            continue;
        }
        else {
            printf("Cliente %d desconectado do servidor!\n", client_id);
            shmdt(shared_mem) == 0 ? printf("Cliente desassociado do processo com sucesso!\n") : perror("Erro ao desassociar o segmento de memoria compartilhada do processo");
            break;
        }
    }
    return 0;
}