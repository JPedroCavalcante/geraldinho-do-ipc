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
    Message *shared_mem = (Message *) shmat(shmid, 0, 0);
    // Valida se houve um erro:
    shared_mem == (void *) -1 ? (perror("Erro ao associar o segmento de memoria compartilhada ao processo"), exit(1)) : printf("Segmento de memoria compartilhada associado ao processo com sucesso!\n");

    // Servidor iniciado: 
    printf("%d\n", shmid);
    printf("%d\n", key);
    printf("Servidor iniciado!\n");

    while(1) {
        // Verificar se há uma nova mensagem na memória:
        printf("Mensagem recebida do Cliente %d: %s", shared_mem->id, shared_mem->msg);
        if (shared_mem->id > 0 && strlen(shared_mem->msg) > 0){
            printf("Mensagem recebida do Cliente %d: %s", shared_mem->id, shared_mem->msg);
            shared_mem->id = 0;
            memset(shared_mem->msg, 0, sizeof(shared_mem->msg));
        }
    }

    // Desassocia o segmento de memoria compartilhada do processo
    if (shmdt(shared_mem) < 0) {
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