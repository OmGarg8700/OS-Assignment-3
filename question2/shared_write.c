#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<string.h>
#include<error.h>
#include<fcntl.h>
#include<time.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/mman.h>

int length = 10;
char** array_strings;

typedef struct data{
    char array[11];
    int array_map;
}data;

int highest_index = -1;

void read_data(){
    int fd = shm_open("shared_memory", O_CREAT|O_RDWR, 0666);
    ftruncate(fd, 1024);

    data* ptr = (data*)mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    // for reading highest index
    highest_index = ptr[0].array_map;
    printf("highest index recieved id %d\n", highest_index);

    munmap(ptr, 1024);

}

void write_data(){
    int fd = shm_open("shared_memory", O_CREAT|O_RDWR, 0666);
    ftruncate(fd, 1024);

    data* ptr = (data*)mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    int index = highest_index+1;

    for(int i=0;i<5;i++){
        strcpy(ptr[i].array, array_strings[index+i]);
        printf("%s\n", ptr[i].array);
        ptr[i].array_map = index+i;
    }

    printf("sended the data\n");

    sleep(2);

    munmap(ptr, 1024);
}

int main(){
    struct timespec initial, final;
    clock_gettime(CLOCK_MONOTONIC, &initial);
    float initial_sec = initial.tv_sec;
    long long int initial_nsec = initial_nsec;

    // for random generation of strings
    time_t seed;
    time(&seed);
    srand(seed);

    array_strings = (char**)malloc(sizeof(char*)*50);
    for(int i=0;i<50;i++){
        array_strings[i] = (char*)malloc(sizeof(char)*(length+1));
    }

    char t;
    for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            array_strings[i][j] = 'a' + rand()%26;
        }
    }

    for(int i=0;i<10;i++){
        write_data();
        read_data();
    }

    clock_gettime(CLOCK_MONOTONIC, &final);
    float final_sec = final.tv_sec;
    long long int final_nsec = final.tv_nsec;

    float time = (final_sec - initial_sec) + ((final_nsec - initial_nsec)/1e9);
    printf("Time for completion is: %f\n", time - 20);

    shm_unlink("shared_memory");
    return 0;
}