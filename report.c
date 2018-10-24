#include<stdio.h>
#include <time.h>
#include <sys/shm.h> 
#include <pthread.h>
#include <sys/ipc.h> 

#include "merge-sort.c"
#include "3-bonus.c"
#include "3-nonbonus.c"

clock_t start1,start2,start3;
clock_t end1,end2,end3;
double t1,t2,t3;

int main()
{
    int shmid,length; 
    key_t key = IPC_PRIVATE; 
    int *shm_array; 

    printf("Enter No of elements of Array:\n"); 
    scanf("%d",&length); 

    size_t SHM_SIZE = sizeof(int)*length; 
   
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
    { 
        perror("shmget"); 
        _exit(1); 
    } 
   
    if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1) 
    { 
        perror("shmat"); 
        _exit(1); 
    } 

    printf("Enter values\n");
    for(int i =0;i<length;i++)
        scanf("%d",&shm_array[i]); 

    printf("\nPERFOMANCE REPORT\n\n");
    //Normal mergesort 
    start1 = clock();
    merge_sort1(shm_array,0,length-1);
    end1 = clock();
    t1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
    printf("Time taken for normal merge sort %f\n",t1);

    //Merge sort with processes 
    start2 = clock();
    mergeSort2(shm_array, 0, length-1);
    end2 = clock();
    t2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
    printf("Time taken for normal merge sort (with processes) %f\n",t2); 

    //Merge sort with threads 
    struct arg arg_cur;
    for(int i = 0;i<length;i++)
    {
        arg_cur.array[i] = shm_array[i];
    }

    arg_cur.x = 0 ;
    arg_cur.y = length - 1;

    start3 = clock();
    mergeSort3(&arg_cur);
    end3 = clock();
    t3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
    printf("Time taken for normal merge sort (with threads) %f\n",t3);  

}