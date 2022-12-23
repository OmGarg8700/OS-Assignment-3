#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include <errno.h>

int waiter = 5;
// 0 -> left,   1-> right

void choose_forks(int direction, int philospher){
    if(waiter > 1){
        waiter--;
        printf("Philospher %d acquired %d\n", philospher, direction);
        return;
    }
    else{
        if(waiter == 1){
            if(direction == 1){
                waiter--;
                printf("Philospher %d acquired %d\n", philospher, direction);
                // return true;
            }
            else{
                // we have to wait
                while(1){
                    if(waiter > 1){
                        waiter--;
                        printf("Philospher %d acquired %d\n", philospher, direction);
                        break;
                    }
                }
                // return 0;
            }
        }
        else{
            while(1){
                    if(waiter > 0){
                        waiter--;
                        printf("Philospher %d acquired %d\n", philospher, direction);
                        break;
                    }
                }
            // return 0;
        }
    }
}

int phil = 2;
void fun(int p){
    if(phil > 0){
        phil--;
        printf("Philospher %d is running ", p);
    }
    else{
        while(1){
            if(phil > 0){
                phil--;
                printf("Philospher %d is running ", p);
                break;
            }
        }
    }
}

void eat(int i){
    printf("Now Philosopher %d eats with forks %d %d and with the bowl %d\n", i, i, i+1, i%2);
}

void think(int i){
    printf("Now Philosopher %d thinks \n", i);
}

void *start(void *arg)
{
    int i = (int)arg;

    choose_forks(0, i);
    choose_forks(1, i);

    eat(i);

    waiter++;       // signal for right
    waiter++;       // signal for left

    think(i);
    
}

int main(){
    pthread_t philosopher[5];

    for (int i = 0; i < 5; i++){

        pthread_create(&philosopher[i], NULL, start, (void *)i); 
    }

    for (int i = 0; i < 5; i++){

        pthread_join(philosopher[i], NULL);
    }
    return 0;
}