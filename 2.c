#include <stdio.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/ipc.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>
#include <limits.h>
#include <string.h>

pthread_cond_t empty[1000], fill[1000];
pthread_mutex_t mutex[1000];
int bnum;

struct booth{
    int id ;
    int voter ;
    int dummy ;
    int evm ;
    int cur_evm;
    int vcount;
};

void put(struct booth * booth_item) {
    (*booth_item).vcount += 1;
}

void get(struct booth * booth_item,int count) {

    (*booth_item).dummy -= count;
    (*booth_item).vcount -= count;
}

void voter_in_slot()
{
    sleep(0.5);
}

void *voter_wait_for_evm(void * bitem) {
    int i;
    struct booth * booth_item = ((struct booth *)bitem);
    pthread_mutex_lock(&mutex[(*booth_item).id]);
    while ((*booth_item).vcount == 10)
        pthread_cond_wait(&empty[(*booth_item).id], &mutex[(*booth_item).id]);
    
    put(booth_item);

    pthread_cond_signal(&fill[(*booth_item).id]);
    pthread_mutex_unlock(&mutex[(*booth_item).id]);

}

void * polling_ready_evm(void * bitem) {
    struct booth * booth_item = ((struct booth *)bitem);
    int evm_no = (*booth_item).cur_evm;
    while((*booth_item).dummy >0)
    {
        pthread_mutex_lock(&mutex[(*booth_item).id]);
        int count = (rand()%10) + 1;
        printf("Evm %d free with %d slots in booth %d\n",evm_no,count,booth_item->id);
        if((booth_item->vcount) < count)
        {
            pthread_mutex_unlock(&mutex[(*booth_item).id]);
            continue;
        }

        for(int j=0;j<count;j++)
            printf("Voter assigned to evm %d in booth %d\n",evm_no,booth_item->id);
        get(booth_item,count);
        printf("Evm %d moving to voting stage in booth %d\n",evm_no,booth_item->id);
        voter_in_slot();
        printf("Evm %d finished voting stage in booth %d\n",evm_no,booth_item->id);    
        pthread_cond_signal(&empty[(*booth_item).id]);
        pthread_mutex_unlock(&mutex[(*booth_item).id]);
    }

}

void * booth_init(struct booth * booth_item) {

    //struct booth * booth_item = ((struct booth *)bitem);
    pthread_t vrobot[(*booth_item).voter],erobot[(*booth_item).evm];
    int status;

    for (int i=0;i<(*booth_item).voter;i++)
    {
        status = pthread_create( &vrobot[i], NULL, voter_wait_for_evm,(void *)booth_item);
        if(status)
        {
            fprintf(stderr,"Voter thread Error - pthread_create() return code: %d\n",status);
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<(*booth_item).evm;i++)
    {   
        booth_item->cur_evm = i;
        status = pthread_create( &erobot[i], NULL, polling_ready_evm,(void *)booth_item);
        if(status)
        {
            fprintf(stderr,"Evm thread Error - pthread_create() return code: %d\n",status);
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<(*booth_item).voter;i++)
        pthread_join( vrobot[i], NULL);

    for (int i=0;i<(*booth_item).evm;i++)
        pthread_join( erobot[i], NULL);
    
    printf("Voters finished voting for booth %d\n",(*booth_item).id);

}

int main()
{
    int status=0;
    printf("Enter number of booths\n");
    scanf("%d",&bnum);
    struct booth bitems[bnum];
    pthread_t brobot[bnum];

    for (int i=0;i<bnum;i++)
    {
        bitems[i].id = i;
        scanf("%d %d",&bitems[i].voter,&bitems[i].evm);
        bitems[i].dummy = bitems[i].voter;
    }

    for (int i=0;i<bnum;i++)
    {
        bitems[i].dummy = bitems[i].voter;
        bitems[i].vcount = 0;
        status = pthread_create( &brobot[i], NULL, booth_init,&bitems[i]);
        if(status)
        {
            fprintf(stderr,"Booth thread Error - pthread_create() return code: %d\n",status);
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<bnum;i++)
        pthread_join( brobot[i], NULL);

    printf("All booths are done\n");

}