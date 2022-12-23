#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>

sem_t chopstick[5];

void eat(int i){
    printf("Now Philosopher %d eats with forks %d %d and with the bowl %d\n", i, i, i+1, i%2);
}

void think(int i){
    printf("Now Philosopher %d thinks \n", i);
}

void *start(void *arg)
{
    int i = (int)arg;

    sem_wait(&chopstick[i]);
    sem_wait(&chopstick[(i + 1) % 5]);

    eat(i);

    sem_post(&chopstick[(i + 1) % 5]);
    sem_post(&chopstick[i]);

    think(i);
    
}

int main()
{

    pthread_t philosopher[5];

    int i = 0, j = 0, k = 0, l = 0;
    for (int i = 0; i < 5; i++){
        sem_init(&chopstick[i], 0, 1);
    }

    for (int i = 0; i < 5; i++){
        pthread_create(&philosopher[i], NULL, start, (void *)i); 
    }

    for (int i = 0; i < 5; i++){
        pthread_join(philosopher[i], NULL);
    }

    return 0;
}