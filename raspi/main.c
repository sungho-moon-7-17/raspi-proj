#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "HTTP.h"

int main(){
    int raspi_socket, client_socket;
    struct sockaddr_in raspi_addr, client_addr;
    int client_addr_len;

    int optval = 1;


    raspi_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (raspi_socket == -1){
        perror("socket fail");
        return -1;
    }
    
    //////////////////////////////////  test용  /////////////////////////////////////////////
    setsockopt(raspi_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    /////////////////////////////////////////////////////////////////////////////////////////

    memset(&raspi_addr, 0, sizeof(raspi_addr));
    raspi_addr.sin_family = AF_INET;
    raspi_addr.sin_port = htons(8080);
    raspi_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(raspi_socket, (struct sockaddr *)&raspi_addr, sizeof(raspi_addr)) == -1){
        perror("bind fail");
        return -1;
    }

    if (listen(raspi_socket, 5) == -1){
        perror("listen fail");
        return -1;
    }
    while(1){
        char requestBuf[BUFSIZ];
        char responseBuf[BUFSIZ];
        int readSize;

        client_socket = accept(raspi_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1){
            perror("accept fail");
            return -1;
        }
        
        read(client_socket, requestBuf, BUFSIZ);
        printf("%s\n\n", requestBuf);

        struct request reqData;

        if (splitRequest(&reqData, requestBuf) != 0)
            continue;
        
        if (!strcmp(reqData.requestStartLine.HTTP_method, "GET")){  // response to get
        
            // status line area
            readSize = sprintf(responseBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");  
            write(client_socket, responseBuf, readSize);    // sends to client

            // response header area (no header)

            // body area
            int fd = open("./web/index.html", O_RDONLY);
            while((readSize = read(fd, responseBuf, BUFSIZ)) > 0){                                 
                write(client_socket, responseBuf, readSize);
            }
            close(fd);

        }
        else{
            printf("error\n\n");
        }

    }

    return 0;
}