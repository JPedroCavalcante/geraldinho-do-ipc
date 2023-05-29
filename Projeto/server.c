//Importando bibliotecas usadas no projeto
#include "libs.h"

#define MAX_MSG_SIZE 100

typedef struct {
    int client_id;
    char message[MAX_MSG_SIZE];
} Message;

int main(){

    // Chama a função de geração de key padrao do projeto
    key_t key = generateKey();

    // Cria o segmento de memoria compartilhada
    int shmid = shmget(key, sizeof(Message), 0666 | IPC_CREAT);
    // Valida se houve um erro:
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

    // Servidor iniciado: 
    printf("Servidor iniciado!\n");
    char lido[MAX_MSG_SIZE];
    printf("\n%d\n", compareString(lido, shared_mem->message));

    while(1) {
        // Verificar se há uma nova mensagem na memória:
        if (shared_mem->client_id > 0) {
            printf("Mensagem recebida do Cliente %d: %s\n", shared_mem->client_id, shared_mem->message);

            // Limpar a mensagem na memória compartilhada
            shared_mem->client_id = 0;
            //memset(shared_mem->message, 0, MAX_MSG_SIZE);
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