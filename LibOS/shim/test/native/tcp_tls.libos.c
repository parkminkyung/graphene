/* -*- mode:c; c-file-style:"k&r"; c-basic-offset: 4; tab-width:4; indent-tabs-mode:nil; mode:auto-fill; fill-column:78; -*- */
/* vim: set ts=4 sw=4 et tw=78 fo=cqt wm=0: */

/* a simple helloworld test */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <shim_unistd.h>

#define SRV_IP "127.0.0.1"
#define PORT 9914
#define BUFLEN 512
#define NPACK 10

int server(void)
{
    char buf[6000]= {0, };
    struct sockaddr_in servaddr, cli;
    int s, i,  connfd;
    unsigned int len;

    if ((s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
        fprintf(stderr, "socket() failed\n");
        exit(1);
    }

    memset((char *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding newly created socket to given IP and verification 
    if ((bind(s, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 

    // Now server is ready to listen and verification 
    if ((listen(s, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 


    // Accept the data packet from client and
    // verification 
    connfd = accept(s, (struct sockaddr *)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 

    sleep(1);
    
    read_nonuser_data(connfd, buf, 6000-4096);

    printf("read data: %d\n", buf[0]);
           
    close(s);
    return 0;
}

int client(void)
{
    struct sockaddr_in servaddr;
    int s, i;
    socklen_t slen = sizeof(servaddr);
    char buf[6000]= {1, };
    buf[4096] = 1;
    int res;

    if ((s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1) {
        fprintf(stderr, "socket() failed\n");
        exit(1);
    }

    memset((char *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    if ( connect(s, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in) != 0)){
        printf("error while connect\n");        
    }
    else
        printf("done?\n");

    write(s, buf, 6000);

    close(s);
    return 0;
}

int main(int argc, char ** argv)
{
    printf("start\n");
    FILE *fptr;

    fptr = fopen("test","r");

    if (fptr == NULL){
        printf("ptr null\n");
    }
    fclose(fptr);

	checkpoint("init");

    if (argc > 1) {
        if (strcmp(argv[1], "client") == 0) {
            client();
            printf("end client\n");
            return 0;
        }

        if (strcmp(argv[1], "server") == 0) {

            server();
            printf("end server\n");
            return 0;
        }
    }

    return 0;
}
