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

        printf("Opa, parece que tem uma mensagem em nossa memória compartilhada, deseja visualiza-lá?[S/N] :");
        scanf("%c", &option);

        switch (option) {
            case 'S':
                printf("%s\n", buff);
                break;

            case 'N':
                printf("Tudo bem, tenha um ótimo dia, até mais!!");
                break;

            default:
                printf("Opção incorreta informada!");
                break;
        }

    } else {
        printf("A memória compartilhada está vazia, deseja inserir algo nela?");
    }
    printf("Saindo da memória, bye bye!!");
}