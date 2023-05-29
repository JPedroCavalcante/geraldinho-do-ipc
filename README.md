# geraldinho-do-ipc
Script feito para realizar comunicação entre processos através do nosso secretário geraldinho, graduado em administração pela UECE com pós graduação em IPC(Interprocces Communication) realizada na universidade de Stanford.

gcc -c funcoes.c -o funcoes.o   
gcc server.c funcoes.o -o server && gcc client.c funcoes.o -o client
