#include "libs.h"

int main(){
    FILE *file = fopen("arquivo.txt", "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
}