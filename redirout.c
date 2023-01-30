#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("Not enough arguments. Run again with more arguments\n");
        exit(0);
    }

    char *filename = argv[1];

    int fd;
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char *words[128];
    char *word;
    word = argv[2];
    words[0] = word;
    int i = 3;
    while (word != NULL) {
        word = argv[i];
        words[i-2] = word;
        i++;
    }
    words[i] = NULL;

    int pfd[2];
    pipe(pfd);

    int pid = fork();
    if (pid == 0) {
        //child
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(words[0],words);
    }
    //parent
    close(pfd[1]);
    int count = 2048;
    char buf[2048];
    while(count > 0) {
        count = read(pfd[0], buf, sizeof buf);
        write(fd, buf, count);
    }
    close(pfd[0]);
    close(fd);
}