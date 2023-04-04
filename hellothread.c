#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



void *run_func(void *arg){
    char * name = arg;
    for (int i = 0; i < 5; i++) {
    printf("%s: %d\n",name, i);
    }

    return NULL;
}

int main(void){

    pthread_t t1;
    pthread_t t2;

    printf("Threads running!\n");

    //for (int i = 0; i < 5; i++) {

    pthread_create(&t1, NULL, run_func, "Thread 1");
    pthread_create(&t2, NULL, run_func, "thread 2");
    //}

    pthread_join(t1, NULL);   
    pthread_join(t2, NULL);   

    printf("threads complete!\n");

}