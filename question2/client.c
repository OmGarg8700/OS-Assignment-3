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
#include<sys/socket.h>
#include<sys/un.h>

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

void Error(char* msg){
    perror(msg);
    exit(1);
}

void read_data(int server){

    if(read(server, output, bytes*5) == -1){
        printf("Error");
        return;
    }

    for(int i=0;i<5;i++){
        printf("%s  %d\n", output[i].array, output[i].array_map);
    }

    highest_index = output[4].array_map;
    highest_string = output[4].array;

}

void write_data(int server){
    int index = highest_index+1;

    strcpy(arr[0].array, highest_string);
    arr[0].array_map = highest_index;

    if(write(server, arr, bytes) == -1){
        printf("Error");
        return;
    }

    printf("sended the data \n");
}

int main(int argc, char* argv[]){

    int sockfd, t, len;
    struct sockaddr_un server;
    socklen_t client_len;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);      // domain is AF_UNIX    type is SOCK_STREAM for TCP control protool is 0 (default)
    if(sockfd < 0){
        Error("Error opening socket");
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "socket");
    if(connect(sockfd, (struct sockaddr*) &server, strlen(server.sun_path) + sizeof(server.sun_family)) < 0){
        close(sockfd);
        Error("Could not connect");
    }


    // main code starting here
    arr = (data*)malloc(sizeof(data));        // this is for 5 struct sending
    output = (data*)malloc(sizeof(data)*5);       // this is for highest index struct recieving

    for(int i=0;i<10;i++){
        read_data(sockfd);
        write_data(sockfd);
    }

    printf("Success ");
    close(sockfd);

    return 0;
}