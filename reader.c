#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    char *sharedMemory;
    int sharedMemoryId;
    int key = 704;
    char buff[256];
    unsigned long sizeMessage;
    char option = 0;

    sharedMemoryId = shmget(key, 256, 0);
    sharedMemory = shmat(sharedMemoryId, 0, 0);
    strcpy(buff, sharedMemory);
    sizeMessage = strlen(buff);
    if (sizeMessage > 0) {

        printf("Chefia, parece que tem uma mensagem em nossa memória compartilhada, deseja visualiza-lá? [S/N]: ");

        do {
            scanf("%c", &option);

            switch (option) {
                case 'S':
                    printf("%s\n", buff);
                    break;

                case 'N':
                    printf("Tudo bem, tenha um ótimo dia, até mais senhor!!");
                    break;

                default:
                    printf("Não conheço essa opção informada senhor, por gentileza informar uma opção correta: ");
                    break;
            }
        } while (option != 'S' && option != 'N');

    } else {
        printf("A memória compartilhada está vazia, deseja inserir algo nela senhor? [S/N]: ");

        do {
            scanf("%c", &option);

            switch (option) {
                case 'S':
                    printf("Qual mensagem deseja inserir algo senhor?\n");
                    scanf("%s", buff);
                    strcpy(sharedMemory, buff);

                    break;

                case 'N':
                    printf("Tudo bem, tenha um ótimo dia, até mais senhor!!");
                    break;

                default:
                    printf("Não conheço essa opção informada senhor, por gentileza informar uma opção correta: ");
                    break;
            }
        } while (option != 'S' && option != 'N');
    }
    shmdt(sharedMemory);
    printf("Estou me desligando e saindo da memória compartilhada para que eu deixe seus dados salvos senhor, tenha um bom dia!!\n");
}