#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#  define PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP \
  { { 0, 0, 0, PTHREAD_MUTEX_ERRORCHECK_NP, 0, { 0 } } }

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//pthread_cond_t A_sideBridge = PTHREAD_COND_INITIALIZER;
//pthread_cond_t B_sideBridge = PTHREAD_COND_INITIALIZER;
pthread_cond_t OnOffBridge = PTHREAD_COND_INITIALIZER;

int counter_A;
int counter_B;
int counter_AB;
int counter_BA;

typedef struct Car {
    int car_id;
    char direction;
    struct Car *next;
    struct Car *prev;
} Car;

Car *head;
Car *tail;

void push(Car *car) {
    Car *newCar = car;

    if (head == NULL) {
        head = newCar;
        tail = newCar;
    } else {
        newCar->prev = tail;
        tail->next = newCar;
        tail = newCar;
    }
}

void pop() {
//Queue *x=(*head);
    if (head == NULL) {
        printf("No cars in line");
    } else if (head->next == NULL) {
        printf("\nUsunieto %d\n", head->car_id);
        head = NULL;
    } else {
        printf("Usunieto %d\n", head->car_id);
        head = head->next;
        head->prev = NULL;
    }
    //free(tmp);
}

void show() {
    if (head == NULL) {
        printf("No cars in line");
    } else
        while (head != NULL) {
            printf("%d   ", head->car_id);
            head = head->next;
        }
}

void printResult(Car *car) {
    if (car->direction == 'A')
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B", counter_A, counter_AB, car->car_id, counter_BA, counter_B);
    else printf("A-%d %d>>> [>> %d >>] <<<%d %d-B", counter_A, counter_AB, car->car_id, counter_BA, counter_B);
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

void *generate(void *id) {
    int tmp;
    tmp =  rand() % 2;
    unsigned int id_num = *((unsigned int *) id);
    char dir;
    if (tmp == 0) dir = 'A';
    else dir = 'B';

    Car *newCar = (Car *) malloc(sizeof(Car));
    newCar->car_id = id_num;
    newCar->direction = dir;
    push(newCar);
}

void city(Car *car) {

    if (car->direction == 'A') {
        car->direction = 'B';
        counter_A++;
    } else {
        car->direction = 'A';
        counter_B++;
    }
    int time = rand() % 10;
    sleep(time);
    push(car);
    if (car->direction == 'A') {
        counter_B--;
        counter_BA++;
    } else {
        counter_A--;
        counter_AB++;
    }

}

void bridge() {
    int on_bridge = 0;  //mozna pozniej zmienic na true/false
    //Car *tmp = (Car *)malloc(sizeof(Car));
    Car *tmp = NULL;
    while (1) {
        if (pthread_mutex_lock(&lock) != 0) {
            printf("błąd");
            exit(-1);
        }

        if (on_bridge == 1) {
            pthread_cond_wait(&OnOffBridge, &lock);
        } else {
            pthread_cond_signal(&OnOffBridge);
            tmp = head;
            if (tmp->direction == 'A') {
                counter_BA--;
            } else {
                counter_AB--;
            }
            on_bridge = 1;
            if (pthread_mutex_unlock(&lock) != 0) {
                printf("błąd");
                exit(-1);
            }
            sleep(1);
            pop();
            printResult(tmp);
            city(tmp);
            on_bridge = 0;
            tmp = NULL;
        }

    }

    free(tmp);


}


int main() {
    head = NULL;
    tail = NULL;

    counter_A = 0;
    counter_B = 0;
    counter_AB = 0;
    counter_BA = 0;

    show();
    int N = 10;
    pthread_t threads[N];
    int rc;
    unsigned int ints[N];
    for (unsigned int i = 0; i <= N; ++i) {
        ints[i]=i;
        rc = pthread_create(&threads[i], NULL, generate, &ints[i]);
        if(rc != 0){
            printf("błąd");
            exit(-1);
        }
    }

    bridge();

    for (int i = 0; i <= N; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("błąd");
            exit(-1);
        }
    }

    return 0;
}
