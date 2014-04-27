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

  // if (suffix[strlen(suffix) - 1] == '/') {
  //     strcat(suffix, "home.html");
  // }

  return 0;
}


int main(int argc, char *argv[]) {
    // printf("%s%s%s", user_agent_hdr, accept_hdr, accept_encoding_hdr);
    int listenfd, clientfd, listenport, clientlen;
    int serverfd;
    struct sockaddr_in clientaddr;
    
    // printf("yay");
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];

    char host[MAXLINE];
    int port;
    char suffix[MAXLINE];

    rio_t from_client_rio, to_server_rio, from_server_rio;//, listenrio, to_server_rio;
    char from_client_buf[MAX_LINE];//, listenbuf[MAXLINE], serverbuf[MAXLINE];
    char from_server_buf[MAX_LINE];
    // char to_server_buf[MAXLINE];


    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenport = atoi(argv[1]);

    listenfd = Open_listenfd(listenport);
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

        if (strcmp(method, "GET")) {
            fprintf(stderr, "method %s not yet implemented\n", method);
        }

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

            Close(serverfd);
        }

        Close(clientfd);
    }
    return 0;
}

void printSAin(struct sockaddr_in sockaddr) {
    printf("Port: %d\n", sockaddr.sin_port);
    printf("Addr: %u\n", sockaddr.sin_addr.s_addr);
    // Transfrom addr to ip address
    printf("IP: %s\n", inet_ntoa(sockaddr.sin_addr));
}
