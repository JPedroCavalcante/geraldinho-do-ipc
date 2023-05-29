//Importando bibliotecas usadas no projeto
#include "libs.h"

#define MAX_MSG_SIZE 100
#define MAX_FILE_SIZE 1000

typedef struct {
    int client_id;
    char message[MAX_MSG_SIZE];
    char file_name[MAX_MSG_SIZE];
    char file_content[MAX_FILE_SIZE];
    int file_size;
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

    while(1) {
        // Verificar se há uma alteração na memória:
        if (shared_mem->client_id > 0) {
            // Verificar se há uma mensagem na memória compartilhada
            if(strlen(shared_mem->message) > 0){
                printf("Mensagem recebida do Cliente %d: %s\n", shared_mem->client_id, shared_mem->message);
            }
            // Verificar se há um arquivo na memória compartilhada
            if(strlen(shared_mem->file_name) > 0){
                printf("Arquivo recebido do Cliente %d: %s\n", shared_mem->client_id, shared_mem->file_name);
                printf("Conteudo do arquivo: %s\n", shared_mem->file_content);
                printf("Tamanho do arquivo: %d\n", shared_mem->file_size);
            }

            // Reseta a validação da memória compartilhada
            // permitindo que o client acesse a ultima mensagem e so mude quando 
            // o client enviar uma nova mensagem
            // o que ira alterar o ID.
            // ----------------------------------------------------
            // Aplicando a mesma logica para os arquivos:
            // Nesse caso, o arquivo só será apagado quando o client enviar um novo arquivo
            shared_mem->client_id = 0;
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