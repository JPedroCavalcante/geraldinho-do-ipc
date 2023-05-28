//Importando bibliotecas usadas no projeto
#include "libs.h"

#define MAX_MSG_SIZE 100

typedef struct {
    int id;
    char msg[MAX_MSG_SIZE];
} Message;

void receiveMessage(int shmid) {
    printf("Aguardando mensagens...\n");
    Message *message = (Message *) shmat(shmid, 0, 0);
    if (message == (void *) -1) {
        perror("Erro ao associar o segmento de memoria compartilhada ao processo");
        exit(1);
    }
    while (1) {
        if (message->id != -1) {
            printf("Cliente %d: %s\n", message->id, message->msg);
            message->id = -1;
        }
        sleep(1);
    }
    printf("Cliente %d desconectado do servidor!\n", message->id);
}

int main(){

    // Chama a função de geração de key padrao do projeto
    key_t key = generateKey();

    // Cria o segmento de memoria compartilhada
    int shmid = shmget(key, sizeof(Message), 0666 | IPC_CREAT);
    // Valida se houve um erro:
    shmid == -1 ? (perror("Erro ao criar o segmento de memoria compartilhada"), exit(1)) : printf("Segmento de memoria compartilhada criado com sucesso!\n");

    // Associa o segmento de memoria compartilhada ao processo
    Message *message = (Message *) shmat(shmid, 0, 0);
    // Valida se houve um erro:
    message == (void *) -1 ? (perror("Erro ao associar o segmento de memoria compartilhada ao processo"), exit(1)) : printf("Segmento de memoria compartilhada associado ao processo com sucesso!\n");

    int max_clientes;
    printf("Seja bem vindo ao nosso servidor de mensagens!\n");
    printf("Digite quantos clientes deseja criar: ");
    scanf("%d", &max_clientes);


    // Inicializa o id
    message->id = -1;


    int i;
    for(i = 0; i < max_clientes; i++){
        // Inicializa o processo
        int pid = fork();
        if (pid < 0) {
            perror("Erro ao criar o processo fork");
            exit(1);
        } else if (pid == 0) {
            // Processo filho
            execl("./client", "client", NULL);
            perror("execl");
            exit(1);
        }
    }

    // Mostra todas as mensagens recebidas
    receiveMessage(shmid);

    // Desassocia o segmento de memoria compartilhada do processo
    if (shmdt(message) < 0) {
        perror("Erro ao desassociar o segmento de memoria compartilhada do processo");
        exit(1);
    }

    // Remove o segmento de memoria compartilhada
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        perror("Erro ao remover o segmento de memoria compartilhada");
        exit(1);
    }
    return 0;
}