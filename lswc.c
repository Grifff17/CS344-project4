#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

    char *filepath = argv[1];

    int pfd[2];
    pipe(pfd);

    int pid = fork();
    if (pid == 0) {
        //child
        dup2(pfd[0],0);
        close(pfd[1]);
        execlp("wc","wc","-l", NULL);
    }
    //parent
    dup2(pfd[1], 1);
    close(pfd[0]);
    execlp("ls","ls","-1a",filepath, NULL);
}