#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include <errno.h>

int waiter = 5;
int bowl = 2;
// 0 -> left,   1-> right

void choose_forks(int direction, int philospher){
    if(waiter > 1){
        waiter--;
        printf("Philospher %d acquired %d fork\n", philospher, direction);
        return;
    }
    else{
        if(waiter == 1){
            if(direction == 1){
                waiter--;
                printf("Philospher %d acquired %d fork\n", philospher, direction);
                return;
            }
            else{
                // we have to wait
                while(1){
                    if(waiter > 1){
                        waiter--;
                        printf("Philospher %d acquired %d fork\n", philospher, direction);
                        break;
                    }
                }
                return;
            }
        }
        else{
            while(1){
                    if(waiter > 0){
                        waiter--;
                        printf("Philospher %d acquired %d fork\n", philospher, direction);
                        break;
                    }
                }
            return;
        }
    }
}

void choose_bowls(int philospher){
    if(bowl > 0){
        bowl--;
        printf("Philospher %d acquired bowl\n", philospher);
        return;
    }
    else{
        // waiting condition
        while(1){
            if(bowl > 0){
                bowl--;
                printf("Philospher %d acquired bowl\n", philospher);
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
    choose_bowls(i);

    eat(i);

    bowl++;         // signal for bowl
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