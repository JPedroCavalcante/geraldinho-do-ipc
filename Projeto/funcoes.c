#include <stdio.h>
#include <sys/ipc.h>
#include <string.h>
#include "funcoes.h"

/* Função padrao para gerar uma chave padrao para o compartilhado usando a função ftkok
para gerar uma chave unica para aquele arquivo.*/
key_t generateKey() {
    return ftok("shared_memory_key", 1) // ftok() = retorna uma chave unica para o arquivo
}