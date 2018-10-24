#include <stdio.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/ipc.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>
#include <limits.h>
#include <string.h>
#include <semaphore.h> 

/*
type 1:player , 0:refree
*/

sem_t player;
pthread_mutex_t refree = PTHREAD_MUTEX_INITIALIZER;

//global variables
int game_count,triple[3];
int p,r,n;
int input[2];
int probability[2];
int abc;
int val;

//function definitions
int find_type();
void * enterAcademy();
void meetOrganizer(int type);
void enterCourt(int type);
void warmUp();
void adjustEquipment();
void StartGame();

int find_type()
{
    int n = 2;
    probability[0] = (int)((r/(p+r))*100);
    probability[1] = 100 - probability[0];

    int prob_sum[n];
    memset(prob_sum, 0 ,sizeof(prob_sum));

    prob_sum[0] = probability[0];
    for(int i = 1;i<n;i++)
        prob_sum[i] = prob_sum[i-1] + probability[i];

    int r = (rand() % 100) + 1;

    if(r<= prob_sum[0])
        return input[0];

    for(int i= 1;i<n;i++)
    {
        if(r > prob_sum[i-1] && r<= prob_sum[i])
            return input[i];
    }

}

void * enterAcademy()
{
    int type;
    type= find_type();
    printf("Enter academy type %d\n",type);
    meetOrganizer(type);
}

void meetOrganizer(int type)
{
    if(type == 1)
    {
        p--;
        sem_wait(&player);
        if(triple[0] == 0)
            triple[0] = 1;
        else 
        triple[1]=1;
    }
    else 
    {
        r--;
        pthread_mutex_lock( &refree );
        triple[2] = 1 ;    
    }

    printf("meet organizer \n");
    while(triple[0] != 1 || triple[1] != 1||triple[2] != 1); // busy waiting

    enterCourt(type);
    abc++;
    if(abc == 3)
    {
        game_count++;
        triple[0]=0;
        triple[1]=0;
        triple[2]=0; 
        sem_post(&player); 
        sem_post(&player); 
        pthread_mutex_unlock( &refree );
        abc = 0;
        val = 2;  
    }

}


void enterCourt(int type)
{
    printf("enter the court type %d\n",type);
    if(type == 1)
        warmUp();
    else 
        adjustEquipment();
    StartGame();
}

void warmUp()
{
    sleep(1);
    printf("warm up\n");
}

void adjustEquipment()
{
    sleep(0.5);
    printf("adjust equipment\n");
}

void StartGame()
{
    printf("a new game has started [%d]\n",game_count+1);

}

int main()
{
    input[0] = 0;
    input[1] = 1;
    sem_init(&player, 1, 2); 
    triple[0]=triple[1]=triple[2]=0;

    printf("Enter n\n");
    scanf("%d",&n);
    p = 2*n;
    r = n;
    pthread_t thread_id[3*n];

    int i, j;
   
    for(i=0; i < 3*n; i++)
    {
        sleep(rand()%3);
        pthread_create( &thread_id[i], NULL, enterAcademy, NULL);
    }                                             

    for(int i = 0;i<3*n;i++)
        pthread_join( thread_id[i], NULL);

    printf("GAME OVER\n");
    exit(EXIT_SUCCESS);

}

