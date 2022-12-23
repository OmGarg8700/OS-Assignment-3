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
char* highest_string = "";

void read_data(){
    int fd = open("myfifo", O_RDONLY);
    
    if(read(fd, output, bytes*5) == -1){
        printf("Error");
        return;
    }

    for(int i=0;i<5;i++){
        printf("%s  %d\n", output[i].array, output[i].array_map);
    }

    highest_index = output[4].array_map;
    highest_string = output[4].array;

    close(fd);
}

void write_data(){
    int fd = open("myfifo", O_WRONLY);

    int index = highest_index+1;

    strcpy(arr[0].array, highest_string);
    arr[0].array_map = highest_index;

    if(write(fd, arr, bytes) == -1){
        printf("Error");
        return;
    }

    printf("sended the data \n");

    close(fd);
}

int main(){
    arr = (data*)malloc(sizeof(data));        // this is for 5 struct sending
    output = (data*)malloc(sizeof(data)*5);       // this is for highest index struct recieving

    for(int i=0;i<10;i++){
        read_data();
        write_data();
    }
    return 0;
}