#include "libs.h"

#define MAX_MSG_SIZE 100
#define MAX_FILE_SIZE 1000

typedef struct {
    int client_id;
    char message[MAX_MSG_SIZE];
    char file_name[MAX_MSG_SIZE];
    char file_content[MAX_MSG_SIZE];
    int file_size;
} Message;


int main() {
    key_t key = generateKey();
    // Acessamos o segmento de memoria compartilhada
    int shmid = shmget(key, sizeof(Message),0);
    //shmget() = retorna o id do segmento de memoria compartilhada

    // Valida se houve um erro no acesso:
    if (shmid == -1) {
        perror("Erro ao acessar o segmento de memoria compartilhada");
        exit(1);
    }

    // Associa o segmento de memoria compartilhada ao processo
    Message *shared_mem = (Message *)shmat(shmid, NULL, 0);
    //shared_mem = ponteiro para o segmento de memoria compartilhada
    //shmat() = associa o segmento de memoria compartilhada ao processo

    // Valida se houve um erro:
    if (shared_mem == (void *)-1) {
        perror("Erro ao anexar o segmento de memoria compartilhada");
        exit(1);
    }

    printf("Cliente iniciado\n");

    // Inicializa o id
    int client_id = getpid();
    // getpid() = retorna o id do processo atual

    printf("Cliente %d conectado ao servidor!\n", client_id);

    while (1) {
        //menu:
        printf("\nO que deseja fazer?\n");
        printf("1. Enviar mensagem\n");
        printf("2. Ver mensagem\n");
        printf("3. Enviar arquivo\n");
        printf("4. Ver arquivo\n");
        printf("5. Sair\n");
        printf("Digite sua opcao: ");

        int opcao;
        scanf("%d", &opcao);
        char trash[MAX_MSG_SIZE];
        fgets(trash, MAX_MSG_SIZE, stdin);

        if (opcao == 1) {
            printf("Digite a mensagem: ");
            //Apagar a mensagem atual do servidor
            memset(shared_mem->message, 0, MAX_MSG_SIZE);
            //Ler a mensagem do cliente
            char msg[MAX_MSG_SIZE];
            fgets(msg, MAX_MSG_SIZE, stdin);
            //Remover o \n do final da string
            if(msg[strlen(msg) - 1] == '\n') msg[strlen(msg) - 1] = '\0';

            //Copiar a mensagem para a memoria compartilhada
            strcpy(shared_mem->message, msg);
            //Enviar a mensagem para o servidor
            shared_mem->client_id = client_id;
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
        else if(opcao == 3){
            // Apagar arquivo atual do servidor:
            memset(shared_mem->file_name, 0, MAX_MSG_SIZE);
            memset(shared_mem->file_content, 0, MAX_FILE_SIZE);
            // memset() = preenche um bloco de memoria com um valor especifico
            shared_mem->file_size = 0;

            // Dados do arquivo:
            char filename[MAX_MSG_SIZE];
            char file_content[MAX_FILE_SIZE];
            int file_size;

            printf("Digite o nome do arquivo: ");
            fgets(filename, MAX_MSG_SIZE, stdin);
            //Remover o \n do final da string
            if(filename[strlen(filename) - 1] == '\n') filename[strlen(filename) - 1] = '\0';
            FILE *file = fopen(filename, "r");
            if(file == NULL){
                printf("Arquivo nao encontrado!\n");
                continue;
            }
            strcpy(shared_mem->file_name, filename);

            fseek(file, 0, SEEK_END);
            // fseek() = posiciona o ponteiro do arquivo
            // ftell() = retorna o tamanho do arquivo
            shared_mem->file_size = ftell(file);
            // Usamos o fseek(file,0,SEEK_END) para posicionar o ponteiro do arquivo no final do arquivo
            // e usamos o ftell(file) para retornar o tamanho do arquivo

            fseek(file, 0, SEEK_SET);
            // fseek() = posiciona o ponteiro do arquivo
            // SEEK_SET = inicio do arquivo
            // USamos o fseek(file,0,SEEK_SET) para posicionar o ponteiro do arquivo no inicio do arquivo
            // para que possamos ler o arquivo
            fread(file_content, shared_mem->file_size, 1, file);
            // fread() = le o arquivo
            strcpy(shared_mem->file_content, file_content);
            fclose(file);
            shared_mem->client_id = client_id;
            continue;
        }
        else if (opcao == 4){
            printf("----------------------------------------\n");
            printf("|                                       |\n");
            printf("| -> Arquivo recebido do servidor: %s\n", shared_mem->file_name);
            printf("| -> Conteudo do arquivo: %s\n", shared_mem->file_content);
            printf("|                                       |\n");
            printf("----------------------------------------\n");
            printf("Baixar o arquivo do servidor ?\n");
            printf("1. Sim\n");
            printf("2. Nao\n");
            printf("Digite sua opcao: ");
            int opcao2;
            scanf("%d", &opcao2);
            char trash[MAX_MSG_SIZE];
            fgets(trash, MAX_MSG_SIZE, stdin);
            if(opcao2 == 1){
                FILE *file = fopen("baixado.txt", "w");
                if(file == NULL){
                    printf("Erro ao criar o arquivo!\n");
                    continue;
                }
                fwrite(shared_mem->file_content, shared_mem->file_size, 1, file);
                fclose(file);
                printf("Arquivo baixado com sucesso!\n");
            }
            continue;
        }
        else if (opcao == 5){
            printf("Cliente %d desconectado do servidor!\n", client_id);
            shmdt(shared_mem) == 0 ? printf("Cliente desassociado do processo com sucesso!\n") : perror("Erro ao desassociar o segmento de memoria compartilhada do processo");
            //shmdt() = desassocia o segmento de memoria compartilhada do processo
            break;
        }
    }
    return 0;
}