#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for sleep()



int val = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *waiter(void *args){
    (void)args;
    pthread_mutex_lock(&mutex);

    while(val < 4){
        printf("waiter: sleeping because %d < 4\n", val);
        pthread_cond_wait(&cond, &mutex);
        printf("waiter: waking up to see if %d >= 4\n", val);
        //TODO: wait
    }
    int my_val = val;
    pthread_mutex_unlock(&mutex);
    printf("Waiter: all done! val is &d\n",my_val);

    return NULL;
}

int main(void){

    pthread_t t;
    pthread_create(&t, NULL, waiter, NULL);


    for (int i = 0; i <5; i++){
        val = 1;
        //TODO signal the other thread to wake up
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    
    pthread_join(t,NULL);


}