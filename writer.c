#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main() {
    char *sharedMemory;
    int key = 704; // key para gerar
    int sharedMemoryId;

    sharedMemoryId = shmget(key, 1024, IPC_CREAT|0666); // Gerar ID da memória compartilhada
    printf("o ID de sua memória compartilhada é %d\n", sharedMemoryId); // Exibe na tela o ID da memória compartilhada
    sharedMemory = shmat(sharedMemoryId, 0, 0);
    printf("Escreva a mensagem que deseja enviar para a memória compartilhada:\n");
    scanf("%s", sharedMemory);
    printf("Você enviou a seguinte mensagem para a memória compartilhada: %s\n", sharedMemory);
    shmdt(sharedMemory);
    printf("Estou me desligando e saindo da memória compartilhada para que eu deixe seus dados salvos senhor, tenha um bom dia!!");
}