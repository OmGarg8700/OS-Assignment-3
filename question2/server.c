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

void Error(char* msg){
    perror(msg);
    exit(1);
}

void read_data(int server){
    
    if(read(server, output, bytes) == -1){
        Error("Problem in reading");
    }

    highest_index = output[0].array_map;
    printf("highest index recieved id %d\n", highest_index);

}

void write_data(int server){
    int index = highest_index+1;

    for(int i=0;i<5;i++){
        strcpy(arr[i].array, array_strings[index+i]);
        printf("%s\n", arr[i].array);
        arr[i].array_map = index + i;
    }    

    if(write(server, arr, bytes*5) == -1){
        Error("Problem in writing");
    }
}

int main(int argc, char* argv[]){
    int sockfd, s2, t;
    struct sockaddr_un server, local;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);      // domain is AF_UNIX    type is SOCK_STREAM for TCP control protool is 0 (default)
    if(sockfd < 0){
        Error("Error opening socket");
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "socket");
    unlink(server.sun_path);
    if(bind(sockfd, (struct sockaddr*) &server, strlen(server.sun_path) + sizeof(server.sun_family))<0){
        Error("Binding failed");
    }
    listen(sockfd, 1);
    
    t = sizeof(server);
    if((s2 = accept(sockfd, (struct sockaddr*) &server, &t)) == -1){
        Error("Accept Error");
    }

    printf("Connected\n");

    // here time will be starting
    struct timespec initial, final;
    clock_gettime(CLOCK_MONOTONIC, &initial);
    float initial_sec = initial.tv_sec;
    long long int initial_nsec = initial_nsec;


    time_t seed;
    time(&seed);
    srand(seed);

    array_strings = (char**)malloc(sizeof(char*)*50);
    for(int i=0;i<50;i++){
        array_strings[i] = (char*)malloc(sizeof(char)*(length+1));
    }

    arr = (data*)malloc(sizeof(data)*5);        // this is for 5 struct sending
    output = (data*)malloc(sizeof(data));       // this is for highest index struct recieving

    for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            array_strings[i][j] = 'a' + rand()%26;
        }
    }


    // now we have to send and recieve


    for(int i=0;i<10;i++){
        write_data(s2);
        read_data(s2);
    }



    close(s2);
    close(sockfd);
    unlink("socket");

    clock_gettime(CLOCK_MONOTONIC, &final);
    float final_sec = final.tv_sec;
    long long int final_nsec = final.tv_nsec;

    float time = (final_sec - initial_sec) + ((final_nsec - initial_nsec)/1e9);
    printf("Time for completion is: %f\n", time);

    return 0;
}