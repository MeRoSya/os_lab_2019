#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

void func1();
void func2();

pthread_mutex_t mutex1, mutex2;

int main(){
    pthread_t thread1, thread2;
    pthread_create( &thread1, NULL, (void*)func1, NULL );
    pthread_create( &thread2, NULL, (void*)func2, NULL );
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}

void func1(){
    pthread_mutex_lock(&mutex2);    //Захват 2 мьютекса 1 тредом
    sleep(5);
    pthread_mutex_lock(&mutex1);    //Неудачная попытка захвата 1 мьютекса 1 тредом
    printf("Thread1\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
}

void func2(){
    pthread_mutex_lock(&mutex1);    //Захват 1 мьютекса 2 тредом
    pthread_mutex_lock(&mutex2);    //Неудачная попытка захвата 2 мьютекса 2 тредом
    printf("Thread2\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}