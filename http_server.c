#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (){
    const int QUEUE_SIZE = 100;
    int socket_fd, new_socket_fd;
    char *response = "GET / HTTP/1.1\nContent-Length: 10\n\nTest value";
    struct sockaddr_in address;
    int addr_size = sizeof(address);

    // We have to create socket 
    //int socket (int family, int type, int protocol) 
    // This operation returns file description as an integer 
    // Family <- e.g IPv4/IPv6...
    // Type <- e.g TCP/UDP...
    // Protocol <- if we want to support more than one protocol 
    printf("Start");
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Cannot create file description");
        return 0;
    }
    // //The special address for this is 0.0.0.0, defined by the symbolic constant INADDR_ANY
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(8888);
    
    if ((bind(socket_fd, (struct sockaddr *)&address, sizeof(address))) < 0){
        printf("Failed to bind to socket");
        return 0;
    }

    // socket should be ready to accept connection
    if (listen(socket_fd, QUEUE_SIZE)  < 0) {
        printf("Failed to listen on socket");
        return 0;
    } 

    //now we have to accept connection and create new fd for it
    while(1){
        if ( (new_socket_fd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addr_size)) < 0 ){
            printf("Failed to accept new connection");
            return 0;
        }
        char buffer[16384] = {0};
        if(read(new_socket_fd, buffer, 16384) < 0){
            printf("Request was empty");
        }
        printf("%s", buffer);
        
        write(new_socket_fd, response, strlen(response));
        close(new_socket_fd);
    }
}
