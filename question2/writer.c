#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<error.h>
#include<fcntl.h>
#include<time.h>
#include <string.h>


typedef struct data{
    char array[11];
    int array_map;
}data;

int length = 10;
char** array_strings;
data* arr;
data* output;
size_t bytes = sizeof(data*) + sizeof(int) + sizeof(char)*5;

int highest_index = -1;

void read_data(){
    int fd = open("myfifo", O_RDONLY);
    
    if(read(fd, output, bytes) == -1){
        printf("Error");
        return;
    }

    highest_index = output[0].array_map;
    printf("highest index recieved id %d\n", highest_index);

    close(fd);
}

void write_data(){
    int index = highest_index+1;

    for(int i=0;i<5;i++){
        strcpy(arr[i].array, array_strings[index+i]);
        printf("%s\n", arr[i].array);
        arr[i].array_map = index + i;
    }

    int fd = open("myfifo", O_WRONLY);

    if(write(fd, arr, bytes*5) == -1){
        printf("Error");
        return;
    }

    close(fd);
}

int main(){
    struct timespec initial, final;
    clock_gettime(CLOCK_MONOTONIC, &initial);
    float initial_sec = initial.tv_sec;
    long long int initial_nsec = initial_nsec;


    time_t seed;
    time(&seed);
    srand(seed);

    int length = 10;
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

    arr = (data*)malloc(sizeof(data)*5);        // this is for 5 struct sending
    output = (data*)malloc(sizeof(data));       // this is for highest index struct recieving

    for(int i=0;i<10;i++){
        write_data();
        read_data();
    }

    clock_gettime(CLOCK_MONOTONIC, &final);
    float final_sec = final.tv_sec;
    long long int final_nsec = final.tv_nsec;

    float time = (final_sec - initial_sec) + ((final_nsec - initial_nsec)/1e9);
    printf("Time for completion is: %f\n", time);

    return 0;
}