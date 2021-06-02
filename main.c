#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(){

    int N=10;
    pthread_t threads[N];

    for(int i=0; i<= N, ++i){
        if(pthread_create(&threads[i], NULL, fun, NULL)!=0){
            printf("błąd");
            exit(-1);
        }
    }


    for (int i=0; i<= N, ++i) {
        pthread_join(threads[i], NULL);
    }


#funkcje A B

#semafor i mutex na most
    return 0;
}
