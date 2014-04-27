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
static const char *http_get_request = "GET %s HTTP/1.0\r\n";


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

  // if (suffix[strlen(suffix) - 1] == '/') {
      // strcat(suffix, "home.html");
  // }

  return 0;
}



int main(int argc, char *argv[]) {
    printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, connfd, listenport, clientlen;
    int serverfd, serverport;
    struct sockaddr_in clientaddr;
    char buf[MAXLINE];
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];
    char host[MAXLINE];
    char suffix[MAXLINE];
    rio_t clientrio, serverrio;
    int i;


    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenport = atoi(argv[1]);

    listenfd = Open_listenfd(listenport);
    while (1) {
        // printf("While\n");
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);

        char host_buf[MAXLINE];
        char get_buf[MAXLINE];
        char request_buf[MAXLINE];

        Rio_readinitb(&clientrio, connfd);

        Rio_readlineb(&clientrio, buf, MAXLINE);
        sscanf(buf, "%s %s %s", method, uri, version);

        while(strcmp(buf, "\r\n")) {
            // TODO forward other headers
            Rio_readlineb(&clientrio, buf, MAXLINE);
        }


        if (strcmp(method, "GET")) {
            fprintf(stderr, "method %s not yet implemented\n", method);
        } else {


            parse_uri(uri, host, &serverport, suffix);

            // printf("host: %s\n", host);
            // printf("port: %d\n", serverport);
            // printf("suffix: %s\n", suffix);
            // printf("b");

            // now open connection with server
            serverfd = Open_clientfd(host, serverport);
            // printf("c");
            Rio_readinitb(&serverrio, serverfd);

            // construct the http request
            sprintf(get_buf, http_get_request, suffix);
            sprintf(host_buf, host_hdr, host);
            sprintf(request_buf, "%s%s%s%s%s%s\r\n",
                host_buf, user_agent_hdr, accept_hdr, accept_encoding_hdr, connection_hdr,
                proxy_connection_hdr);

            // printf("\na\n");


            // send the http request
            Rio_writen(serverfd, get_buf, strlen(get_buf));
            Rio_writen(serverfd, request_buf, strlen(request_buf));

            while((i = Rio_readlineb(&serverrio, buf, MAXLINE)) != 0){
                Rio_writen(connfd, buf, i);
            }

            Close(serverfd);
        }

        Close(connfd);
    }
    return 0;
}
