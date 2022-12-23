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

typedef struct data{
    char array[11];
    int array_map;
}data;

int highest_index = -1;
char* highest_string = "";

void read_data(){
    int fd = shm_open("shared_memory", O_CREAT|O_RDWR, 0666);
    ftruncate(fd, 1024);

    data* ptr = (data*)mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    for(int i=0;i<5;i++){
        printf("%s  %d\n", ptr[i].array, ptr[i].array_map);
    }

    highest_index = ptr[4].array_map;
    highest_string = ptr[4].array;

    munmap(ptr, 1024);
}

void write_data(){
    int fd = shm_open("shared_memory", O_CREAT|O_RDWR, 0666);
    ftruncate(fd, 1024);

    data* ptr = (data*)mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    strcpy(ptr[0].array, highest_string);
    ptr[0].array_map = highest_index;

    printf("sended the data \n");

    sleep(2);

    munmap(ptr, 1024);
}

int main(){

    for(int i=0;i<10;i++){
        read_data();
        write_data();
    }

    shm_unlink("shared_memory");
    return 0;
}