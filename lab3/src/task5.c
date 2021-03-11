#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }
  pid_t pid = fork();
  if (pid==-1){
      printf("Fork failed!");
      return 1;
  } else if (pid==0){
      execv("/projects/os_lab_2019/lab3/src/sequential_min_max", argv);
  } else {
      wait(NULL);
      printf("Success\n");
  }
}