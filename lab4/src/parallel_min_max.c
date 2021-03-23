#include <signal.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

void signal_handler(int signal){
    
    kill(0,SIGKILL);
}
void signal_handler_child(int signal){
    printf("I'm %d my parent is %d\nSIGKILL recieved\n",getpid(),getppid());
    kill(0, SIGKILL);
}
int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;
  int timeout = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (array_size <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);
            if (timeout < 0) {
                printf("timeout is not a negative number\n");
                return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  FILE* tempf;
  int** pipefd;
  if (with_files)
  {
      tempf = fopen("temp.txt", "w");
  }
  else {
      pipefd = (int**)malloc(sizeof(int*)*(pnum+1));
      
      }

    if (timeout>=0){
        signal(SIGALRM, signal_handler);
        signal(SIGTERM, SIG_IGN);
            alarm(timeout);
        }
    
    sleep(2);
  for (int i = 0; i < pnum; i++) {

    if (!with_files)
    {
        pipefd[i]=(int*)malloc(sizeof(int)*2);
        
        if (pipe(pipefd[i])<0)
        {
            printf("Error while making pipe");
            return 1;
        }
    }
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        if(timeout>=0){
        signal(SIGTERM, signal_handler_child);
        }
        sleep(3);
        // parallel somehow
        struct MinMax temp;
        if (i != pnum - 1)
        {
            temp = GetMinMax(array, i*array_size/pnum, (i+1) * array_size/pnum);
        }
        else 
        {
            temp = GetMinMax(array, i*array_size/pnum, array_size/pnum);
        }
        if (with_files) {
          // use files here
          fwrite(&temp.max, sizeof(int), 1, tempf);
          fwrite(&temp.min, sizeof(int), 1, tempf);
        } else {
          // use pipe here
          write(pipefd[i][1],&temp.min, sizeof(int));
          write(pipefd[i][1],&temp.max, sizeof(int));
          close(pipefd[i][1]);
          
        }
        return 0;
      }
        

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  while (active_child_processes > 0) {
    int state;
    waitpid(-1,&state,WNOHANG);
    active_child_processes -= 1;
  }

  
    
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
      fclose(tempf);
      tempf=fopen("temp.txt", "r");
      fread(&max, sizeof(int), 1, tempf);
      fread(&min, sizeof(int), 1, tempf);
      
    } else {
      // read from pipes
      read(pipefd[i][0],&min,sizeof(int));
      
      read(pipefd[i][0],&max,sizeof(int));    
      
      close(pipefd[i][0]);    
      
      free(pipefd[i]) ;
      
    }
    
    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  remove("temp.txt");
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec-5) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
  
  

  for(int i=0; i<array_size; i++){
      printf("%d\n", array[i]);
  }
  
  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
