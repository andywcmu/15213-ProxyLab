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
static const char *to_server_request_hdr = "GET %s HTTP/1.0\r\n";

static const char *cookie_hdr = "Cookie: __utma=44984886.1661963158.1346006067.1396225764.1398549567.3; __utmz=44984886.1396225764.2.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __utmc=44984886\r\n";
static const char *accept_language_hdr = "Accept-Language: en-US,en;q=0.5\r\n";



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
<<<<<<< HEAD
  //     strcat(suffix, "home.html");
=======
      // strcat(suffix, "home.html");
>>>>>>> FETCH_HEAD
  // }

  return 0;
}



int main(int argc, char *argv[]) {
<<<<<<< HEAD
    // printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, clientfd, listenport, clientlen;
    int serverfd;
    struct sockaddr_in clientaddr;
    
    // printf("yay");
=======
    printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, connfd, listenport, clientlen;
    int serverfd, serverport;
    struct sockaddr_in clientaddr;
    char buf[MAXLINE];
>>>>>>> FETCH_HEAD
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];

    char host[MAXLINE];
    int port;
    char suffix[MAXLINE];
<<<<<<< HEAD

    rio_t from_client_rio, to_server_rio, from_server_rio;//, listenrio, to_server_rio;
    char from_client_buf[MAX_LINE];//, listenbuf[MAXLINE], serverbuf[MAXLINE];
    char from_server_buf[MAX_LINE];
    // char to_server_buf[MAXLINE];
=======
    rio_t clientrio, serverrio;
>>>>>>> FETCH_HEAD


    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenport = atoi(argv[1]);

    listenfd = Open_listenfd(listenport);
<<<<<<< HEAD
    // printf("yay");
    while (1) {
        clientlen = sizeof(clientaddr);
        clientfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);
        
        // Read request method, uri, and version
        Rio_readinitb(&from_client_rio, clientfd);

        char temp_buf[100000];

        // Read method, uri, version
        Rio_readlineb(&from_client_rio, temp_buf, 100000);
        sscanf(temp_buf, "%s %s %s", method, uri, version);

        while (strcmp(temp_buf, "\r\n")) {
            Rio_readlineb(&from_client_rio, temp_buf, 100000);
            strcat(from_client_buf, temp_buf);
        }
=======
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);

        char host_buf[MAXLINE];
        char get_buf[MAXLINE];
        char request_buf[MAXLINE];

        Rio_readinitb(&clientrio, connfd);

        Rio_readlineb(&clientrio, buf, MAXLINE);
        sscanf(buf, "%s %s %s", method, uri, version);

        while(1) {
            int i = Rio_readlineb(&clientrio, buf, MAXLINE);
            if (i == 2) {
                break;
            }
            printf("%s", buf);
        }

>>>>>>> FETCH_HEAD

        if (strcmp(method, "GET")) {
            fprintf(stderr, "method %s not yet implemented\n", method);
        } else {


            parse_uri(uri, host, &serverport, suffix);

            // printf("host: %s\n", host);
            // printf("port: %d\n", serverport);
            // printf("suffix: %s\n", suffix);
            // printf("b");

<<<<<<< HEAD
        /* The request method is "GET" */
        else {
            if (parse_uri(uri, host, &port, suffix)) {
                // CHANGE THIS PART
                printf("Parse error\n");
                exit(1);
            }

            char host_tmp[MAXLINE];

            sprintf(host_tmp, host_hdr, host);
            sprintf(from_client_buf, "%s%s%s%s%s%s%s%s\r\n", host_tmp, user_agent_hdr, accept_hdr, accept_encoding_hdr,
                connection_hdr, proxy_connection_hdr, cookie_hdr, accept_language_hdr);

            // now open connection with server
            serverfd = Open_clientfd(host, port);
            Rio_readinitb(&to_server_rio, serverfd);
            char to_server_buf[MAXLINE];
            sprintf(to_server_buf, to_server_request_hdr, suffix);

            Rio_writen(serverfd, to_server_buf, strlen(to_server_buf));
            fprintf(stderr, "%s\n", to_server_buf);
            Rio_writen(serverfd, from_client_buf, strlen(from_client_buf));
            fprintf(stderr, "%s\n", from_client_buf);

            Rio_readinitb(&from_server_rio, serverfd);

            while (Rio_readlineb(&from_server_rio, temp_buf, 100000) != 0) {
                strcat(from_server_buf, temp_buf);
                Rio_writen(clientfd, temp_buf, strlen(temp_buf)); 
                fprintf(stderr, "%s\n", temp_buf);
            }

=======
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

            int i;
            while((i = Rio_readlineb(&serverrio, buf, MAXLINE)) != 0){
                Rio_writen(connfd, buf, i);
            }

>>>>>>> FETCH_HEAD
            Close(serverfd);
        }

        Close(connfd);
    }
    return 0;
}
