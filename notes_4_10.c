//locks//


//this wont work

int lock = 0;

void unlock(void){
    lock = 0;
}

void lock(void){
    //if someone holds the lock, we wait until its unlocked
    // while (lock == 1){
    //     ;//wait
    // }
    // lock = 1;

    //spins endlessly until it see's a 0 and is now free. 
    //Thread can now set it to 0, and go about its day
    while (test_and_set(&lock, 1) == 1){
        //yield(); //yield works great with 2 threads
        sleep_until_lock_is_released();
    }
}

//TEST AND SET (atomic exchange)//

//pseudocode (because theres nothing actually stopping interruption)

int test_and_set(int *old_ptr, int new_val){

    //this can't be interrupted between here 
    int old_val = *old_ptr;

    *old_ptr = new_val;
    //and here
    return old_val;
}

//improving spin locks//
//pthread locks("mutexes")

int lock = 0;

void lock*(void)
void unlock(void)

//---//

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);

///

void unlock(void){

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    /...
    pthread_mutex_lock(&lock);

    if (amount <= balance)
    
}
