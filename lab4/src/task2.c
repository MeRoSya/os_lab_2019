#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    //Родитель не считывает код завершения нигде, 
    //поэтому дочерний процесс станет зомби
    if (pid > 0) {
        printf("I'm doing things\n");
        sleep(10);
    } else {
        printf("None");
        exit(0);
    }
    return 0;
}