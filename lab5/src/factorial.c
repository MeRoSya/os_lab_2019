#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <getopt.h>

struct FactArgs {
  long long current_value;
  int current_n;
  int it_num;
};
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
void ThreadFact(struct FactArgs* args) {
  for(int i=0; i<args->it_num; i++){
        pthread_mutex_lock(&mut);
        args->current_n--;
        if(args->current_n==0){
            break;
        } else {
        args->current_value *=args->current_n;
        }
        pthread_mutex_unlock(&mut);
    }
}

int main(int argc, char **argv){
    int k = -1;
    int pnum = -1;
    int mod = -1;

    //Получение параметров
    static struct option long_options[] = {{"mod", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 1},
                                      {0, 0, 0, 0}};
    int c;
    while ((c = getopt_long_only(argc, argv, "k:", long_options, NULL)) != -1){
        switch (c) {

        case 0:
            {
                mod = atoi(optarg);
                if(mod<=0) {
                    printf("mod is a positive number\n");
                    return 1;
                }
            }
            break;

        case 1:
            {
                pnum = atoi(optarg);
                if (pnum<=0) {
                    printf("pnum is a positive number\n");
                    return 1;
                }
            }
            break;

        case 'k':
            {
                k=atoi(optarg);
                if(k<=0) {
                    printf("k is a positive number\n");
                    return 1;
                }
            }
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

    if (k == -1 || pnum == -1 || mod == -1) {
        printf("Usage: %s -k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
        return 1;
    }
    if (pnum>k){
        printf("pnum is greater than k\nSo in case not to start unnessesery threads, pnum will equal to k\n");
        pnum = k;
    }
    pthread_t threads[pnum];
    struct FactArgs args;
    args.it_num = k/pnum;
    args.current_n = k;
    args.current_value = k;

    for(int i=0; i<pnum; i++){
        if (pthread_create(&threads[i], NULL, (void*)ThreadFact, (void*)&args)) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Factorial = %lld\nFactorial mod = %lld\n", args.current_value, args.current_value%mod);
    return 0;
}
