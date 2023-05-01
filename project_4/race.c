// 4 components of deadlock

// -mutual exclusion:
// --when you hold a lock, someone else can't get to that code
// -hold and wait
// --when you hold one resource, and your waiting for another
// -no preemption
// --you can't interrupt a waiting thread
// -circular wait
// --dining philosophers, everyone has left for


    //---------------------------//
    //1
    if x == 12:
        x++;

    //1 solution:
    //The race occurs anytime a thread examines the value of x (the if condition), or adds to x (x++)
    //To fix, add a lock around the whole thing

    lock(m1)
    if x == 12;
        x++;
    unlock(m1)
    ////--------------------////


    //---------------------------//
    //2
    if x == 12:
        lock(m1)
        x++;
        unlock(m1)

    //2 solution
    //The race occurs anytime a thread examines the value of x (the if condition).
    //to fix, add a lock around the whole thing

    lock(m1)
    if x ==12;
        x++;
    unlock(m1)
    ////---------------------------////


    //---------------------------//
    //3
    if y not in hash:
        hash[y] = 12
    else
        hash[y]++

    //3 solution:
    //The race occurs anytime a thread examines the hash to find y, and also theoretically making y 12
    //if two threads act at the same time, instead of doing y=12, then increment, it could set y=12 twice
    //to fix, add a lock around the whole thing
    lock(m1)
    if y not in hash:
        hash[y] = 12
    else
        hash[y]++
    unlock(m1)
    ////---------------------------////


    //---------------------------//   
    //4
    x += 12;
    //4 solution:
    //There is no race condition with this one snippet of code, but could easily be a race if further code
    //relied upon the value of x. As two threads could incremenent at the same time.
    //to fix, add a lock around the whole thing
    lock(m1)
    x += 12;
    unlock(m1)
    ////---------------------------////

    
    //---------------------------//
    //5
    x = 0

    semaphore_init(value):
        x = value

    semaphore_signal():
        x++;

    semaphore_wait():
        while x == 0: //->1
            do nothing  # spinlock

        x--

    //5 solution:
    //The race condition occurs when two threads escape the while loop at once, when only one should.
    //ex: set X to 1, threads to 2. Two threads use the wait loop, escape out of the spinlock at the same time, 
    //and decrement down to -1.
    //The spinlock no longer applies as its below 0, and can cause further issues.
    //To fix, add a mutex around wait

    semaphore_wait():
    lock(m1)
        while x == 0: //->1
            do nothing  # spinlock

        x--
    unlock(m1)

    ////---------------------------////


    //PART 2//

    //---------------------------//
    //1
    function1():
        lock(m1) //->1
        lock(m2) 

        unlock(m2)
        unlock(m1)

    function2():
        lock(m2)//->1
        lock(m1)

        unlock(m1)
        unlock(m2)


    //1 solution:
    //in function 1, at ->1, lock two is freed, but is inacessible. 
    //This can create a deadlock for anyone attempting to acquire lock 2.
    //This can make function 2 inacessible to start

    //in function 2, at ->2, lock one is freed, but is inacessible.
    //This can create a deadlock for anyone attempting to acquire lock 2.
    //This can make function 2 inacessible to start

    //in both functions, at ->3, if two threads each grab the start of function 1 and 2 respectively,
    //they can create a deadlock that makes it impossible for the other to continue, and continue ad infinatum.
    //to fix:
    //aquire mutex's in the same order
   function1():
        lock(m1)
        lock(m2) 

        unlock(m1)
        unlock(m2)

    function2():
        lock(m1)
        lock(m2)

        unlock(m1)
        unlock(m2)
    ////---------------------------////


    //---------------------------//
    //2
    function1():
        lock(m1)//->1
        lock(m2)

        unlock(m2)
        unlock(m1)

    function2():
        lock(m1)
        lock(m2)
       
        unlock(m1) //-->1
        lock(m1)

        unlock(m2)
        unlock(m1)

    //problem 2 solutions:
    //At ->1, assume thread 1 had gotten to unlock step in function 2, 
    //and thread 2 grabs that lock before thread 1 can lock it again
    //this creates a deadlock where neither function can continue and thus deadlocks

    //to fix, aquire m2 before m1 OR remove mid unlock/lock statement.

    function1():
        lock(m2)
        lock(m1)

        unlock(m2)
        unlock(m1)

    function2():
        lock(m2)
        lock(m1)
       
        unlock(m1)
        lock(m1)

        unlock(m2)
        unlock(m1)
    ////---------------------------////


    //---------------------------//
    //3
    function1(m1, m2):  # Mutexes are passed in by caller//check out ordering by memory address, weeks current reading, related to this
        lock(m1)
        lock(m2)

        unlock(m2)
        unlock(m1)

    //problem 3 solutions:
    //A problem occurs with this function if two different threads are passed in a different order of mutex's.
    //If thread A gets (function1(L1, L2)) and thread B gets function1(L2,L1), they could theoretically deadlock at m2
    //To fix, you must sort the locks to ensure its always in the same order

    function1(m1,m2)
        if m1 > m2{
            lock(m1)
            lock(m2)
        }
        else{
            lock(m2)
            lock(m1)
        }
    ////---------------------------////
