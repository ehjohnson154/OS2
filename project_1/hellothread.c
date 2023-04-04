#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>



//For multiple arguments, internet recommends struct?//
struct thread_args {
    int tally;
    char * thread_name;
};


void *run_func(void *arg){
    struct thread_args *args = arg;
    // char * name = arg;
    // static int num = 0;
    char * name = args->thread_name;
    int tally = args->tally;

    for (int i = 0; i < tally; i++) {
    printf("%s: %d\n",name, i);
    sleep(0);
    }

    return NULL;
}

int main(void){

    pthread_t t1;
    pthread_t t2;

    struct thread_args args1;
    args1.tally = 5;
    args1.thread_name = "Thread 1";
    struct thread_args args2;
    args2.tally = 5;
    args2.thread_name = "Thread 2";


    printf("Launching threads\n");

    pthread_create(&t1, NULL, run_func, &args1);
    pthread_create(&t2, NULL, run_func, &args2);


    pthread_join(t1, NULL);   
    pthread_join(t2, NULL);   

    printf("threads complete!\n");

}

