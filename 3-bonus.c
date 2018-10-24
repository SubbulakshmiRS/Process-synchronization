#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h>
#include<time.h>

struct arg{
    int array[100000];
    int x;
    int y;
};

void mergeSort3( struct arg * argument);
void selection_sort3(int arr[], int n); 
struct arg * merge3( struct arg arg1 ,struct arg arg2, struct arg * a) ;
void isSorted3(int arr[], int len) ;

void mergeSort3( struct arg * argument) 
{ 
    int length = sizeof((* argument).array)/sizeof((* argument).array[0]);
    int i, len=((* argument).y-(* argument).x+1); 

    if (len<=5) 
    { 
        selection_sort3((* argument).array+(* argument).x, len); 
        return; 
    } 
    struct arg arg1,arg2;

    arg1.x = (* argument).x;
    arg2.y = (* argument).y;
    arg1.y = (* argument).x + len/2 - 1;
    arg2.x = (* argument).x + len/2;

    for(int i = 0;i<length;i++)
    {
        arg1.array[i] = (* argument).array[i];
        arg2.array[i] = (* argument).array[i];
    }

    pthread_t thread1, thread2;
    int left_t, right_t;

    left_t = pthread_create( &thread1, NULL, mergeSort3, &arg1);
    if(left_t)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",left_t);
        exit(EXIT_FAILURE);
    }

    right_t = pthread_create( &thread2, NULL, mergeSort3 , &arg2);
    if(right_t)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",right_t);
        exit(EXIT_FAILURE);
    }

    pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

    /*isSorted3(arg1.array+arg1.x,arg1.y-arg1.x+1);
    isSorted3(arg2.array+arg2.x,arg2.y-arg2.x+1);
    */

    argument = merge3(arg1,arg2,argument); 
} 
  
void selection_sort3(int arr[] , int n)
{ 
    int i,j,min,temp;
    for(i=0;i<n-1;i++)
    {
        min=i;
        for(j=i+1;j<n;j++)
        {
            if(arr[min]>arr[j])
            min=j;
        }
        if(min!=i)
        {
            temp=arr[i];
            arr[i]=arr[min];
            arr[min]=temp;
        }
    }
}
  
struct arg * merge3( struct arg arg1 ,struct arg arg2, struct arg * a) 
{ 
    int i=arg1.x,j=arg2.x,k=arg1.x;
    while(i<=arg1.y&&j<=arg2.y)
    {
        if(arg1.array[i]<arg2.array[j])
        {
            a->array[k] = arg1.array[i];
            i++;
            k++;
        }
        else 
        {
            a->array[k] = arg2.array[j];
            j++;
            k++;
        }
    }
    
    if(i<=arg1.y)
        while(i<=arg1.y)
        {
            a->array[k] = arg1.array[i];
            i++;
            k++;
        }

    if(j<=arg2.y)
        while(j<=arg2.y)
        {
            a->array[k] = arg2.array[j];
            j++;
            k++;
        }
    
    return a;
} 
  
// To check if array is actually sorted or not 
void isSorted3(int arr[], int len) 
{ 
    if (len==1) 
    { 
        printf("Sorting Done Successfully length %d\n",len); 
        return; 
    } 
  
    int i; 
    for (i=1; i<len; i++) 
    { 
        if (arr[i]<arr[i-1]) 
        { 
            printf("\nSorting Not Done length %d %d\n",len,i); 
            return; 
        } 
    } 
    printf("\nSorting Done Successfully length %d\n",len); 
    return; 
} 

/*  
int main() 
{ 
    int shmid; 
    key_t key = IPC_PRIVATE; 
    int *shm_array; 
    int length ;

    printf("Enter No of elements of Array:"); 
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

    for(int i =0;i<length;i++)
        scanf("%d",&shm_array[i]);    
  
    struct arg arg_cur;
    for(int i = 0;i<length;i++)
    {
        arg_cur.array[i] = shm_array[i];
    }

    arg_cur.x = 0 ;
    arg_cur.y = length - 1;

    mergeSort3(&arg_cur); 

    for(int i=0;i<length;i++)
        printf("%d ",arg_cur.array[i]);
    printf("\n");

    if (shmdt(shm_array) == -1) 
    { 
        perror("shmdt"); 
        _exit(1); 
    } 
  

    if (shmctl(shmid, IPC_RMID, NULL) == -1) 
    { 
        perror("shmctl"); 
        _exit(1); 
    } 
  
    return 0; 
} 
*/