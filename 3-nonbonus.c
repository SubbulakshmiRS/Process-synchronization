#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>

int start,end;

void mergeSort2(int a[], int x, int y) ;  
void SelectionSort2(int arr[], int n);
void merge2(int a[],int x1,int y1,int x2,int y2);  
  
void mergeSort2(int a[], int x, int y) 
{ 
    int i, len=(y-x+1); 

    if (len<=5) 
    { 
        SelectionSort2(a+x, len); 
        return; 
    } 
  
    pid_t lpid,rpid; 
    lpid = fork(); 
    if (lpid<0) 
    { 
        // Lchild proc not created 
        perror("Left Child Proc. not created\n"); 
        _exit(-1); 
    } 
    else if (lpid==0) 
    { 
        mergeSort2(a,x,x+len/2-1); 
        _exit(0); 
    } 
    else
    { 
        rpid = fork(); 
        if (rpid<0) 
        { 
            // Rchild proc not created 
            perror("Right Child Proc. not created\n"); 
            _exit(-1); 
        } 
        else if(rpid==0) 
        { 
            mergeSort2(a,x+len/2,y); 
            _exit(0); 
        } 
    } 
  
    int status; 
 
    waitpid(lpid, &status, 0); 
    waitpid(rpid, &status, 0); 

    merge2(a, x,x+len/2-1,x+len/2,y); 
} 

void SelectionSort2(int arr[], int n) 
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
  
void merge2(int a[],int x1,int y1,int x2,int y2) 
{ 
    int i = x1,j=x2,k=0;
    int len = y2-x1+1; 
    int sorted[len]; 
    while(i<=y1&&j<=y2)
    {
        if(a[i]<a[j])
        {
            sorted[k] = a[i];
            i++;
            k++;
        }
        else 
        {
            sorted[k] = a[j];
            j++;
            k++;
        }
    }
    
    if(i<=y1)
        while(i<=y1)
        {
            sorted[k] = a[i];
            i++;
            k++;
        }

    if(j<=y2)
        while(j<=y2)
        {
            sorted[k] = a[j];
            j++;
            k++;
        }
    for(int l=x1;l<=y2;l++)
        a[l]=sorted[l-x1];
} 
  
// To check if array is actually sorted or not 
void isSorted2(int arr[], int len) 
{ 
    if (len==1) 
    { 
        printf("Sorting Done Successfully\n"); 
        return; 
    } 
  
    int i; 
    for (i=1; i<len; i++) 
    { 
        if (arr[i]<arr[i-1]) 
        { 
            printf("Sorting Not Done\n"); 
            return; 
        } 
    } 
    printf("Sorting Done Successfully\n"); 
    return; 
} 
/*  
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
  
    for(int i =0;i<length;i++)
        scanf("%d",&shm_array[i]);

    mergeSort2(shm_array, 0, length-1); 

    for(int i=0;i<length;i++)
        printf("%d ",shm_array[i]);
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
}*/ 
