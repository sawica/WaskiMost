#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue = PTHREAD_MUTEX_INITIALIZER;

int counter_A=0;
int counter_B=0;
int counter_AB=0;
int counter_BA=0;

typedef struct Car {
    int car_id;
    char direction;
    struct Car *next;
    struct Car *prev;
}Car;

Car *head;
Car *tail;

void push(Car *car) {
    Car *newCar = car;

    if (pthread_mutex_lock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
    if (head == NULL) {
        head = newCar;
        tail = newCar;
    } else {
        newCar->prev = tail;
        tail->next = newCar;
        tail = newCar;
    }
    if (pthread_mutex_unlock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
}

void pop() {
//Queue *x=(*head);
    if (pthread_mutex_lock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
    if (head == NULL) {
        printf("No cars in line");
    } else if (head->next == NULL) {
        head = NULL;
    } else {
        head = head->next;
        head->prev = NULL;
    }
    if (pthread_mutex_unlock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
    //free(tmp);
}

void show() {
    if (pthread_mutex_lock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
    if (head == NULL) {
        printf("No cars in line");
    } else
        while (head != NULL) {
            printf("%d s  ", head->car_id);
            head = head->next;
        }if (pthread_mutex_unlock(&queue) != 0) {
        printf("błąd");
        exit(-1);
    }
}

void printResult(Car *car) {
    if (car->direction == 'A')
        printf("\n A-%d %d>>> [<< %d <<] <<<%d %d-B\n ", counter_A, counter_AB, car->car_id, counter_BA, counter_B);
    else printf("\n A-%d %d>>> [>> %d >>] <<<%d %d-B\n ", counter_A, counter_AB, car->car_id, counter_BA, counter_B);
}
//tzreba jeszcze dopisac wypisywanie listy w kolejce
//" Po uruchomieniu programu z parametrem -debug należy wypisywać całą zawartość kolejek po obu stronach mostu, nie tylko ilość samochodów."

int countCar_bydir(char pom) {
    Car *tmp = head;
    int n = 0;
    for (int i = 0; tmp; tmp = tmp->next) {
        if (tmp->direction == pom) {
            n++;
        }
    }
    return n;
}

int car_in_queue(int id) {
    Car *tmp = head;
    for (int i = 0; tmp; tmp = tmp->next) {
        if (tmp->car_id == id) {
            return 1;
        }
    }
    return 0;
}

Car* getCar() {
    Car *tmp = head;
    /*for (int i = 0; tmp; tmp = tmp->next) {
        if (tmp->car_id == id) {
            return tmp;
        }
    }*/
    return tmp;

}

int car_on_top(Car *x) {
    Car *tmp = head;
        if (tmp == x) {
            return 1;
        }
    return 0;
}

void city(Car *car) {
    printf("city");
    if (car->direction == 'A') {
        car->direction = 'B';
        printf("a++");

        counter_A++;printf("%d",counter_A);
    } else {
        car->direction = 'A';
        printf("b++");
        counter_B++;printf("%d",counter_B);
    }
    int time = rand() % 10;

    if (car->direction == 'A') {
        counter_B--;
        counter_BA++;
    } else {
        counter_A--;
        counter_AB++;
    }sleep(time);
    push(car);
   // *bridge(car->car_id);
}

void *generate(void *id) {
    int tmp;
    tmp =  rand() % 2;
    int id_num = *((int *) id);
    char dir;
    if (tmp == 0) dir = 'A';
    else dir = 'B';

    Car *newCar = (Car *) malloc(sizeof(Car));
    newCar->car_id = id_num;
    newCar->direction = dir;
    push(newCar);
    //sleep(4);
}



void *bridge() {
    Car *newCar = (Car *) malloc(sizeof(Car));
    while(1){
        newCar = getCar();
        if(car_on_top(newCar)==1 && newCar->direction=='A'){

            if (pthread_mutex_lock(&lock) != 0) {
                printf("błąd lock");
                exit(-1);
            }
            counter_BA--;
            sleep(1);
            printResult(newCar);
            pop();

            city(newCar);printResult(newCar);
            newCar = NULL;
            if (pthread_mutex_unlock(&lock) != 0) {
                printf("błąd lock");
                exit(-1);
            }
        }
        else if(car_on_top(newCar)==1 && newCar->direction == 'B'){

            if (pthread_mutex_lock(&lock) != 0) {
                printf("błąd lock");
                exit(-1);
            }
            counter_AB--;
            sleep(1);
            pop();
            printResult(newCar);
            city(newCar);
            newCar = NULL;
            if (pthread_mutex_unlock(&lock) != 0) {
                printf("błąd lock ");
                exit(-1);
            }}
        free(newCar);

    }
}


int main() {

    head = NULL;
    tail = NULL;
/*
    counter_A = 0;
    counter_B = 0;
    counter_AB = 0;
    counter_BA = 0;

    Car *newCar = (Car *) malloc(sizeof(Car));
    Car *newCar1 = (Car *) malloc(sizeof(Car));
    Car *newCar2 = (Car *) malloc(sizeof(Car));
    newCar->car_id = 1;
    newCar->direction = 'A';
    newCar1->car_id = 2;
    newCar1->direction = 'A';
    newCar2->car_id = 3;
    newCar2->direction = 'B';
    push(newCar);
    push(newCar1);
    push(newCar2);
    pop();
    show();
*/
    int N = 3;
    pthread_t threads[N], bridge_pthread;
    int rc;
    int ints[N];
    for (int i = 1; i <= N; ++i) {
        ints[i]=i;
        if (pthread_create(&threads[i], NULL, generate, (void *)&ints[i]) != 0) {
            printf("błąd create");
            exit(-1);
        }
    }
    if (pthread_create(&bridge_pthread, NULL, bridge, NULL) != 0) {
        printf("błąd create");
        exit(-1);
    }

    for (int i = 1; i <= N; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("błąd join");
            exit(-1);
        }
    }
    if (pthread_join(bridge_pthread, NULL) != 0) {
        printf("błąd join");
        exit(-1);
    }
//show();
    return 0;
}
