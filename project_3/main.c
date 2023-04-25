#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h> 
#include <fcntl.h> 

#include "eventbuf.h"
#include "eventbuf.c"

struct eventbuf * buffer;
//sem_t *sem_open_temp("consumer_mutex", 0);
sem_t *producer_sem;
sem_t *consumer_sem;
sem_t *buffer_sem;

int producer_count;
int consumer_count;
int event_production_count;
int outstanding_event_count;


// struct producer_block {
//     int thread_id;
//     int epc;     // event production count
//     int oec;   // outstanding event count
// };
//producer_thread_id, event_production_count,outstanding_event_count


sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void * producing(void *arg){
 
    // int producer_number, int prod_count, int queue
    int * producer_number = arg;
    //printf("in producer thread!\n");
    //printf("%d producer number\n", *producer_number);

    for (int i=0;i<event_production_count;i++){
        sem_wait(producer_sem); //This is right
        int event_data = ((*producer_number * 100) + i);
        printf("P%d: adding event %d\n",*producer_number, event_data);
        sem_wait(buffer_sem);
        eventbuf_add(buffer, event_data);
        sem_post(buffer_sem);
        sem_post(consumer_sem);
        //printf("looped in producer thread!\n");

    //sem_post(consumer_mutex)
    }

    printf("P%d: exiting...\n",*producer_number);
    return NULL;
}

void * consuming(void *arg){
    //int consumer_number

    int * consumer_number = arg;

    while(1){
        sem_wait(consumer_sem);
        //lock
        //create semaphore lock with val 1 for non-thread safe buffer
        sem_wait(buffer_sem);
        if (eventbuf_empty(buffer)){
            sem_post(buffer_sem);
            //
            printf("C%d: exiting...\n",*consumer_number);
            sem_post(consumer_sem);
            //
            return NULL;
        }
        int event = eventbuf_get(buffer);
        sem_post(buffer_sem);
        printf("C%d: got event %d\n",*consumer_number, event);
        sem_post(producer_sem);
    }

}


//semaphore of 1 is free, when aquired it goes down to 0, no longer positive and now locked

int main(int argc, char *argv[]){

   // printf("start program!\n");

    if (argc != 5) {
        fprintf(stderr, "incorrect number of arguments!\n");
        exit(1);
    }

    producer_count = atoi(argv[1]);
    consumer_count = atoi(argv[2]);
    event_production_count = atoi(argv[3]);
    outstanding_event_count = atoi(argv[4]);

    buffer = eventbuf_create();
    consumer_sem = sem_open_temp("consumer_mutex", 0);
    producer_sem =sem_open_temp("producer_mutex", outstanding_event_count);
    buffer_sem = sem_open_temp("buffer_mutex", 1);


    //printf("grabbed data!\n");

    //eventbuf_create; 


    pthread_t *producer_thread = calloc(producer_count, sizeof *producer_thread);
    int *producer_thread_id = calloc(producer_count, sizeof *producer_thread_id);


    pthread_t *consumer_thread = calloc(consumer_count, sizeof *consumer_thread);
    int *consumer_thread_id = calloc(consumer_count, sizeof *consumer_thread_id);
    //printf("allocated threads!\n");

    //NEED TO DO:
    //Create producer threads
    for(int i = 0; i < producer_count; i++){
        //create producers
        producer_thread_id[i] = i;
        pthread_create(producer_thread + i, NULL, producing, 
        producer_thread_id + i);
        //printf("created producer threads!\n");
        //producer_thread_id, event_production_count,outstanding_event_count
    }

    //Create consumer threads

    for(int i = 0; i < consumer_count; i++){
        consumer_thread_id[i] = i;
        pthread_create(consumer_thread + i, NULL, consuming, 
        consumer_thread_id + i);
        //printf("created consumer threads!\n");
        //consumer_thread_id
        //create consumers
    }
    //Create events for each producer based on count
    //create stack of events
    //initiate events


    for (int i = 0; i < producer_count; i++){
        pthread_join(producer_thread[i], NULL);}
    
    sem_post(consumer_sem);

    for (int i = 0; i < consumer_count; i++){
        pthread_join(consumer_thread[i], NULL);}

    sem_close(consumer_sem);
    sem_close(producer_sem);

}