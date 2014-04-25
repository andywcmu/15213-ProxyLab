#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_LINE 1024

/* You won't lose style points for including these long lines in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection_hdr = "close\r\n";
static const char *proxy_connection_hdr = "close\r\n";

void printSAin(struct sockaddr_in* sockaddr);

int main(int argc, char *argv[]) {
    printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio;


    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    printf("1: %s\n2: %s\n", argv[0], argv[1]);


    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    printf("test1\n");
    while (1) {
        printf("while\n");
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);

        Rio_readinitb(&rio, connfd);
        Rio_readlineb(&rio, buf, MAXLINE);
        sscanf(buf, "%s %s %s", method, uri, version);

        printSAin(&clientaddr);
        printf("Method: %s\n", method);
        printf("URI: %s\n", uri);
        printf("Version: %s\n", version);

		Close(connfd);
    }
    return 0;
}

void printSAin(struct sockaddr_in* sockaddr) {
    printf("Port: %d\n", sockaddr->sin_port);
    printf("Addr: %u\n", sockaddr->sin_addr.s_addr);
}
