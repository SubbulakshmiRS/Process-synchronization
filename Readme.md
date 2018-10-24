# *ASSIGNMENT 4*

## *QUESTION 1*
>1 semaphore of value 2 for the player 

>1 mutex for the refree 

Type of person entering the academy is found out by randomly chosen out of the values 0(refree) and 1(player) with calculated probabilities .

>Probablity of player = p/(p+r)

>Probability of refree = r/(p+r)

For each person entering , a thread is created the enterAcademy function is called . Type of the person is found and later according the type ,appropriate functions are called . In the meetOrganizer function , if the person is a player , the semaphore wait is called and if refree , the mutex lock is called .Once the 2 players and 1 refree is found , they enter the Court . Until the game has started the , all the remaining threads are waiting .

## *QUESTION 2*
> 2 condition waits and 1 mutex lock is used 

> Threads are created for each booth , from which threads are created for each voter and evm . 

>The struct booth consists of id(id), number of voters in the booth(voter), number of evms(evm) , number of voters still to be processed (dummy), the current evm which is being handled by the thread (cur_evm) , the number of voters who have entered the booth(vcount).

Each voter thread calls upon the voter_wait_for_evm function and waits to be proccessed . The evm threads call the polling_ready_evm function and gets into the while loop .The while loop will only break when the number of remaining voters to be proccessed is zero .Each evm when called , randomly allocates a number of slots and checks whether enough number of voters are there to be allocated to slots .If present , all are allocated and proccessed . If not , the current evm is dropped , mutex is unlocked and again by race condition , a evm is called .

> This code is loosely based on the producer-consumer thread problem .

## *EXECUTE CODE 1*
>gcc -pthread 1.c

>./a.out 

Enter value of n (representing the number of games to be played )

## *EXECUTE CODE 2*
>gcc -pthread 2.c

>./a.out 

Enter value of number of booths (n).

For the remaining n lines , enter 2 values representing the number of voters and number of evms of that booth.


## *EXECUTE CODE 3*
For the 3rd question , 3 files are submitted 
> The normal merge sort (merge-sort.c)

> Mergesort with processes (3-nonbonus.c)

> Mergesort with threads (3-bonus.c)

To execute the 3 files separately , uncomment the int main function description and compile (gcc -pthread) and execute (./a.out) .

For the performance report between the files , compile and execute the report.c file which makes comparison between the 3 methods of mergesort .
> gcc -pthread report.c
>./a.out

Enter values as needed .



