#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_LINE 1024

/* You won't lose style points for including these long lines in your code */
static const char *host_hdr = "Host: %s\r\n";
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";

void printSAin(struct sockaddr_in sockaddr);

/*
 * parse a uri http://<host>:<port(optional)><filename>. If the port part is
 * missing, a default 80 is returned.
 */
int parse_uri(char *uri, char *host, int *port, char *suffix)
{
  *port = 80;

  uri = uri + 7;
  while (*uri != '/' && *uri != ':') {
    *host = *uri;
    uri++;
    host++;
  }
  *host = '\0';

  if (*uri == ':') {
    uri++;
    sscanf(uri, "%d%s", port, uri);
  }

  strcpy(suffix, uri);

  if (suffix[strlen(suffix) - 1] == '/') {
      strcat(suffix, "home.html");
  }

  return 0;
}


int main(int argc, char *argv[]) {
    printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    char buf[MAXLINE];
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];

    rio_t rio;


    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);

        // printSAin(clientaddr);

        char host_buf[MAXLINE];
        char request_buf[MAXLINE];

        // Read request method, uri, and version
        Rio_readinitb(&rio, connfd);
        Rio_readlineb(&rio, buf, MAXLINE);
        sscanf("%s %s %s\n", method, uri, version);

        printf("%s\n", buf);


        sscanf(buf, "%s %s %s", method, uri, version);

        if (!strcmp(method, "GET")) {
            fprintf(stderr, "method %s not yet implemented\n", method);
            continue;
        }

        printf("%s", buf);
        if (request_buf == host_buf) {}

        sprintf(host_buf, host_hdr, "www.cmu.edu");
        sprintf(request_buf, "%s%s%s%s%s%s",
            host_buf, user_agent_hdr, accept_hdr, accept_encoding_hdr,
            connection_hdr, proxy_connection_hdr);

        Close(connfd);
    }
    return 0;

}

void printSAin(struct sockaddr_in sockaddr) {
    printf("Port: %d\n", sockaddr.sin_port);
    printf("Addr: %u\n", sockaddr.sin_addr.s_addr);
    // Transfrom addr to ip address
    printf("IP: %s\n", inet_ntoa(sockaddr.sin_addr));
}
