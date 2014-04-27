#include <stdio.h>
#include "csapp.h"
#include "truck.h"
#include "contracts.h"

/* Recommended max cache and object sizes */
#define MAX_LINE 1024
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including these long lines in your code */
static const char *host_hdr = "Host: %s\r\n";
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";
static const char *get_request_hdr = "GET %s HTTP/1.0\r\n";


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

  return 0;
}

inline static void create_headers_to_server (char *to_server_buf, char *host, char *suffix) {
    char host_buf[MAXLINE];
    char get_request_buf[MAXLINE];
    sprintf(get_request_buf, get_request_hdr, suffix);
    sprintf(host_buf, host_hdr, host);
    sprintf(to_server_buf, "%s%s%s%s%s%s%s\r\n",
        get_request_buf,
        host_buf, user_agent_hdr, accept_hdr, accept_encoding_hdr, connection_hdr,
        proxy_connection_hdr);
    return;
}


int main(int argc, char *argv[]) {
    int listenfd, clientfd, serverfd;
    int listenport, serverport;
    size_t clientlen;
    struct sockaddr_in clientaddr;
    
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char host[MAXLINE], suffix[MAXLINE];

    char buf[MAXLINE];
    char object_buf[MAX_OBJECT_SIZE];

    rio_t clientrio, serverrio;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    struct cache_header *C = cache_init();

    cache_insert(C, "andy", "hahahahahaha");

    listenport = atoi(argv[1]);

    listenfd = Open_listenfd(listenport);
    
    while (1) {
        cache_print(C);

        clientlen = sizeof(clientaddr);
        clientfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);

        /* Read method, uri, version + other key:value pairs from client */
        // Read method, uri, version
        Rio_readinitb(&clientrio, clientfd);
        Rio_readlineb(&clientrio, buf, MAXLINE);
        sscanf(buf, "%s %s %s", method, uri, version);

        // Read other key:value pairs
        while(strcmp(buf, "\r\n")) {
            // TODO forward other headers
            Rio_readlineb(&clientrio, buf, MAXLINE);
        }


        if (strcmp(method, "GET")) {
            fprintf(stderr, "method %s not yet implemented\n", method);
        }

        /* If the request method is GET */
        else {
            /* see if already in cache */
            char *object = cache_find(C, uri);
            if (object != NULL) {
                Rio_writen(clientfd, object, strlen(object));
            }
            /* not in cache */
            else {
                parse_uri(uri, host, &serverport, suffix);

                char to_server_buf[MAXLINE];
                create_headers_to_server(to_server_buf, host, suffix);

                /* Send to server */
                serverfd = Open_clientfd(host, serverport);
                Rio_readinitb(&serverrio, serverfd);
                Rio_writen(serverfd, to_server_buf, strlen(to_server_buf));

                /* Get from server and send to client */
                // size_t buflen;
                // while((buflen = Rio_readlineb(&serverrio, buf, MAXLINE)) != 0){
                
                while(Rio_readlineb(&serverrio, buf, MAXLINE) != 0){
                    // Rio_writen(clientfd, buf, buflen);
                    strcat(object_buf, buf);
                }

                Rio_writen(clientfd, object_buf, strlen(object_buf));
                /* add to cache */
                cache_insert (C, uri, object_buf);

                /* clear the buffer */
                strcpy(object_buf, "");

                Close(serverfd);
            }
        }

        Close(clientfd);
    }
    return 0;
}
