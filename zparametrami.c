#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock =  PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
pthread_cond_t A_sideBridge = PTHREAD_COND_INITIALIZER;
pthread_cond_t B_sideBridge = PTHREAD_COND_INITIALIZER;

typedef struct Queue{
    int car_id;
    char direction;
    struct Queue *next;
    struct Queue *prev;
};

void push(Queue **head, Queue **tail, int car, char dir){
    Queue *newCar;
    newCar = (Queue *)malloc(sizeof(Queue));
    newCar->car_id=car;
    newCar->direction=dir;
    newCar->next= NULL;
    newCar->prev= NULL;

    if(*head==NULL){
        (*head)=newCar;
        (*tail)=newCar;
    }
    else{
        newCar->prev=(*tail);
        (*tail)->next=newCar;
        (*tail)=newCar;
    }
}

void pop(Queue **head){
//Queue *x=(*head);
    if((*head)==NULL){
        printf("No cars in line");
    }
    else if((*head)->next==NULL){
        printf("\nUsunieto %d\n", head->key);
        *head=NULL;}
    else{
        printf("Usunieto %d\n", head->key);
        (*head)=(*head)->next;
        (*head)->prev=NULL;
    }
    //free(tmp);
}

void show(FIFO *head){
    if((*head)==NULL){
        printf("No cars in line");
    }
    else while(*head!=NULL){
            printf("%d   ", head->key);
            head=head->next;
        }
}

int countCar_bydir(char pom)
{
    Queue *tmp = head;
    int n=0;
    for(int i = 0; tmp; tmp = tmp->next){
        if(tmp->direction==pom){
            n++;
        }
    }
    return n;
}


//f wypisywania









int main(){
     Queue *head=NULL;
     Queue *tail=NULL;

    push(&head,&tail,1);
    push(&head,&tail,4)
    pop(&head);
    show(head);
    int N=10;
    pthread_t threads[N];

    for(int i=0; i<= N, ++i){
        if(pthread_create(&threads[i], NULL, city, (void *) i)!=0){
            printf("błąd");
            exit(-1);
        }
    }


    for (int i=0; i<= N, ++i) {
        if(pthread_join(threads[i], NULL)!=0){
            printf("błąd");
            exit(-1);
        }
    }

    return 0;
}
