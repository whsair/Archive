#include <stdio.h>
#include "csapp.h"
#include <string.h>
#include <stdlib.h>
#include "cache.h"

//StudentID: hkw5146
//Author: Hongshuo Wang

// define a struct with header type to hold each header
// each header contain name and value
typedef struct _Header{
  char name[MAXBUF];
  char value[MAXBUF];
} Header;

// free all the allocation in the headers array
void free_headers(int start_point, Header *headers) {
  int i;
  for (i = start_point; i < 90; i++) {
    free(headers[i].name);
    free(headers[i].value);

  }

}

// This is the helper function that handle the request from the client
int request_headers_handler(rio_t *rp, char *host, Header *headers)
{
  char* buf = malloc(MAXLINE);
  char* name;
  char* value;
  char* temp = "\r\n";
  char* temp_host = malloc(MAXLINE);
  int i = 0;
  Header header;

  int header_counter = 0;


// we first need to store the neccessary headers in the struct Headers
// store the host and its responding name then update the number of headers
  strcpy(header.name, "Host");
  strcpy(temp_host, " ");
  strcat(temp_host, host);
  strcpy(header.value, strcat(temp_host, temp));
  headers[header_counter] = header;
  header_counter++;

// store the user-Agent
  strcpy(header.name, "User-Agent");
  strcpy(header.value, " Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n");
  headers[header_counter] = header;
  header_counter++;

// store the connection: close
  strcpy(header.name, "Connection");
  strcpy(header.value, " close\r\n");
  headers[header_counter] = header;
  header_counter++;

// store the proxy-connection: close
  strcpy(header.name, "Proxy-Connection");
  strcpy(header.value, " close\r\n");
  headers[header_counter] = header;
  header_counter++;


// go through the actually request headers and compare each header's name (case insensitively)
  while(strcmp(buf, "\r\n")) {

    i = rio_readlineb(rp, buf, MAXLINE);
    name = strtok_r(buf, ": ", &value);

    if (i <= 0) {
      free(temp_host);
      free(buf);
      free_headers(0, headers);
      return 0;
    }
    if (i > 2) {

// check if the header has a "host" name just skip it since we have stored that after
      if (!(strncasecmp(name, "Host", strlen("Host")))) {


        continue;
        
// if the header has a "connection"  or "proxy-connection" name; just skip it
      } else if (!(strncasecmp(name, "Connection", strlen("Connection"))) ||
          !(strncasecmp(name, " Proxy-Connection", strlen("Proxy-Connection")))) {


        continue;
// if the header has a "user-agent" name; skip it
      } else if (!strncasecmp(name, "User-Agent", strlen("User-Agent"))) {

        continue;

// if the header has a "IF-Modified-since" or "if-none-match" name; skip it
      } else if (!(strncasecmp(name, "If-Modified-Since", strlen("If-Modified-Since"))) ||
          !(strncasecmp(name, "If-None-Match", strlen("If-None-Match")))) {
        continue;

      } else {
// else just store them in the headers array without any change
        strcpy(header.name, name);
        strcpy(header.value, value);
        headers[header_counter] = header;
        //printf("Name: %s\n  %d\n  Value: %s\n", header.name, header_counter, header.value);
        header_counter++;

      }


    }

  }



  free(buf);
  free(temp_host);

// finally, we will return the number of header we store in the header
  return header_counter;
}

// this function from hw9 we will input a url and return host, port and path if
// the url is valid
int parse_url(const char *url, char *host, char *port, char *path) {


// Using the function strncasecmp which campare dst and src character by character
// without case sensitivity what campare the first seven bits of input url with "HTTP://"
// make sure that url is in the proper scheme; otherwise, function should return 0

  if ((strncasecmp(url, "HTTP://", strlen("HTTP://"))) == 0) {

// set up the variable to hold the token (by strtok_r function)
// set up the variable rest to hold the rest of string
// after running strtok_r function
// allocate data buffer (max_buf 8096) for variable temp and rest
// t1 and t2 are helper pointer to hold the location of the malloc
// temp and rest pointer will always change in this function
// at last, if we free(temp) or free(rest) error will happen since
// temp and rest doesn't pointer to the initial malloc location now.
    char *token;
    char *temp;
    char *rest;
    char *t1 = malloc(MAXBUF);
    char *t2 = malloc(MAXBUF);
    temp = t1;
    rest = t2;
    int i;

// copy the url withour the first seven bit (the length of HTTP://
// to the rest (copying also include url's null terminator
    strncpy(rest, url + 7, strlen(url) - 6);

// strtok_r function will create the token depending on the input
// delimiter. Bascially, it output the part of string until to get
// the delimiter and set up the rest of string to your input location
// it output a char pointer that points to the token
// If the input delimiter is not found in the string, just print out
// the raw string and set up null to the input location
//
// First, we check whether there is port number in the string
// by setting ";" as the delimiter
    token = strtok_r(rest, ":", &rest);

// if the port number doesn't exist, variable token will be set as
// the raw string, and the string length of rest varialbe will 0
// since it set as null
    if (strlen(rest) == 0) {

// since the port number doesn's exists we set it up 80 as default
      strcpy(port, "80");

// Next, we need to check whether there are path number in the string
      token = strtok_r(token, "/", &rest);

// Like the preivous we did, after know there is no part number
// in the url, the rest of nonezero token will be the host
// we also set up path pointer to "/" as default
      if (strlen(rest) == 0) {
        if (token != 0) {
          strcpy(host, token);
          strcpy(path, "/");
        }

// if the path part does exists, the set up host pointer to be token
// and the rest part of the string should be path part of url
// before we set it up we need to all "/" at the beginning of the path part
      } else {
        strcpy(host, token);
        strcpy(path, "/");
        strcat(path, rest);
      }
    } else {

// now we need to handle there is ";" in the url
// it possible that ":" is part of path in the url and port number doesn't exist in
// the url
// To handle this situation, we need to see the forward part of ":" which is also the
// current token output by the strtok_r function. we need to see whether there exist
// a "/" in this part. By the definition of port part of url, that mean port number
// doens't exist in the url. When we see that, we need to set up the default port
// number 80. and recombine the path part (also adding "\" and make up the ":" since
// strtok_r function will get rid of the delimiter.
// At last, pass every thing to the pointer they belongs to.
      strcpy(temp, token);
      token = strtok_r(temp, "/", &temp);
      if (strlen(temp) != 0) {
        strcpy(host, token);
        strcpy(port, "80");
        strcpy(token, "/");
        strcat(token, temp);
        strcpy(temp, ":");
        strcat(token, temp);
        strcat(token, rest);
        strcpy(path, token);
      } else {

// Now, ":" appears in the now location which denotes that port number exists in the url
// now token is the current host number since locates forwards that ":"
        strcpy(host, token);

// Now the rest part of our string contains only port part and path port (possibly doesn't exist)
// we use strtok_r with "/" to check the forward part of string
        token = strtok_r(rest, "/", &rest);

// if "/" doesn;t exsits, that means the none zero rest of string is all port number
// set up the path part "/" as default value.
        if (strlen(rest) == 0) {

          for (i = 0; i < strlen(token); i++) {
            if (isdigit(token[i]) == 0) {
              free(t1);
              free(t2);
              return 0;
            }
          }
          strcpy(port, token);
          strcpy(path, "/");

// if "/" exists that means current token is the port part and the rest of string is
// path part. Before we set up the path part, we need to add "/" at its beginning.

        } else {
          for (i = 0; i < strlen(token); i++) {
            if (isdigit(token[i]) == 0) {
              free(t1);
              free(t2);
              return 0;
            }
          }
          strcpy(port, token);
          strcpy(path, "/");
          strcat(path, rest);
        }
      }
    }

// everything is correct, we free the buffer and return 1
    free(t1);
    free(t2);
    return 1;
  }

// otherwise return 0

  return 0;
}

// connect to a server using open_clientfd
// if the connection is success return the fd
// else return -1
int connect_to_server(char *host, char *port) {

  int i = 0;
  if ((i = open_clientfd(host, port)) >= 0) {
    return i;
  } else {
    return -1;
  }
}

// this fucntion send the request to server by combining what we store headers before
void send_request_to_server(Header *headers, char *host, char *path,
    int server_fd, int header_counter, rio_t *server_rio) {
  char* buf = malloc(MAXBUF);
  char* temp_buf = malloc(MAXBUF);
  int i;


  rio_readinitb(server_rio, server_fd);


// the first line I send to the server which is Get /path HTTP/1.0
  sprintf(buf, "GET %s HTTP/1.0\r\n", path);

// go through the structure headers combine every name and header
  for (i = 0; i < header_counter; i++) {
    sprintf(temp_buf, "%s:%s", headers[i].name, headers[i].value);
    strcat(buf, temp_buf);


  }
// all the last "\r\n" to the end of our buffer
  strcat(buf, "\r\n");

// foward the request to server
  rio_writen(server_fd, buf, strlen(buf));
  //free_headers(header_counter, headers);
  free(buf);
  free(temp_buf);

  return;


}

// this is the function that get the headers and bit content from the server then
// send them to the client
void foward_to_browser(rio_t *server_rio, int client_fd, int server_fd, CacheList *cache, char *url) {

  char* value;
  char* buf = malloc(MAXLINE);
  char* temp_buf = malloc(MAXLINE);
  void* temp = malloc(MAX_OBJECT_SIZE);
  char* headers = malloc(MAX_OBJECT_SIZE);

// flag => 1 if the one of header is "HTTO/1.0 200 OK\r\n"
//        0 else
// flag => 1 if the one of header contains "Content-length: "
//         0 else
  int flag1 = 0, flag2 = 0;
  int i = 0, content_length = -1;


  
  rio_readinitb(server_rio, server_fd);

//check whether the respond line from server is empty, if it is free every buffer and return
  if ((i = rio_readlineb(server_rio, buf, MAX_OBJECT_SIZE)) <= 0) {

    free(buf);
    free(temp);
    free(headers);
    free(temp_buf);
    return;
  }

// check the header, write to the cache buffer and write to the client
  while (strcmp(buf, "\r\n")) {

// write to the headers' buffer since we need to check
    strcat(headers, buf);

// check the flag1 condition
    if (!strcasecmp(buf, "HTTP/1.0 200 OK\r\n")) {
      flag1 = 1;
    }

// check the flag2 condition and get the content-length from the headers
    if (!strncasecmp(buf, "Content-length: ", strlen("Content-length: "))) {
      flag2 = 1;
      strcpy(temp_buf, buf);
      strtok_r(buf, ": ", &value);
      sscanf(value, "%d", &content_length);
      strcpy(buf, temp_buf);

    }

// write to the client line by line

    rio_writen(client_fd, buf, i);

// read line by line from the server
    i = rio_readlineb(server_rio, buf, MAXLINE);

  }

// add the last "\r\n" to the headers
  strcat(headers, buf);

// writers all headers to the client
  rio_writen(client_fd, buf, i);

// write binary content to the client if it exists
  if ((i = rio_readnb(server_rio, temp, MAX_OBJECT_SIZE)) > 0) {

    rio_writen(client_fd, temp, i);
  }

// if all four requirement is ok, we cache this item
// Only cache web objects when the server’s response status code is 200 OK;
// Only cache web objects when the server supplies the object size in a header ”Content-Length:” (caseinsensitive);
// Only cache web objects whose size is no bigger than the maximum object size defined by the macro MAX_OBJECT_SIZE•
// Only cache web objects that you actually received the number of bytes for the web object
// that matches the size described in the Content-Length header.
  if ((i <= MAX_OBJECT_SIZE) && (i == content_length) && (flag1) && (flag2)) {

    cache_URL(url, headers, temp, i, cache);
  } else {

// else we need to free the allocation
    free(temp);
    free(headers);

  }

// close the server
  close(server_fd);

  free(buf);
  free(temp_buf);
  return;



}


/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    sprintf(buf, "%sContent-type: text/html\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}
/* $end clienterror */

// Browser sends HTTP request to the proxy server.
// Proxy server communicates with thebrowser through fd
// and handles the client in doit(fd)
void doit(int fd, CacheList *cache)
{
  char* buf = malloc(MAXLINE), *method= malloc(MAXLINE), *url = malloc(MAXLINE), *version = malloc(MAXLINE);
  char* host = malloc(MAXLINE), *port = malloc(MAXLINE), *path = malloc(MAXLINE);
  rio_t rio, server_rio;
  int server_fd, header_counter = 0;
  CachedItem *item;
  Header headers_lines[90];

// Read request line and headers
  rio_readinitb(&rio, fd);

// read the request line if null just return
  if (!rio_readlineb(&rio, buf, MAXLINE)) {
    //free_headers(0, headers_lines);
    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, url, version);

// our server only handle the 'get" instruction otherwise return
  if (strcasecmp(method, "GET")) {
      clienterror(fd, method, "501", "Not Implemented",
                    "Proxy does not implement this method");
    //free_headers(0, headers_lines);
    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

// input the url and get the responding host port and path
// if url is valid

  if (!(parse_url(url, host, port, path))) {
        clienterror(fd, host, "404", "Not found",
                    "Proxy couldn't find this file");
    //free_headers(0, headers_lines);
    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

// search the item in the cache by the url if it exists just output all the content
// and the headers to the client instead of fowarding to server
  item = find(url, cache);
  if (item != NULL) {
    printf("Gets from cache\n");
    rio_writen(fd, item-> headers, strlen(item-> headers));
    rio_writen(fd, item-> item_p, item-> size);
    //free_headers(0, headers_lines);
    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

// deal with the request headers from the client

  if ((header_counter = request_headers_handler(&rio, host, headers_lines)) <= 0) {

    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

// connect to server
  if ((server_fd = connect_to_server(host, port)) < 0) {
    //free_headers(0, headers_lines);
    free(buf);
    free(method);
    free(version);
    free(host);
    free(port);
    free(path);
    free(url);
    return;
  }

// fowarding request to server
  send_request_to_server(headers_lines, host, path, server_fd, header_counter, &server_rio);

// deal with content and headers from the server including the cache storation
  foward_to_browser(&server_rio, fd, server_fd, cache, url);

// free all buf

  free(buf);
  free(method);
  free(version);
  free(host);
  free(port);
  free(path);
  free(url);

}
/* $end doit */


// start off proxy as a network server.
int main(int argc, char **argv)
{
  CacheList cache_store;
  cache_init(&cache_store);
  int listenfd, connfd;


  char* hostname = malloc(MAXLINE), *port = malloc(MAXLINE);
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;


  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port> \n", argv[0]);
    exit(1);
  }

// ignoreSIGPIPE
  Signal(SIGPIPE, SIG_IGN);
  listenfd = Open_listenfd(argv[1]);

// once theserver starts, should avoid using Accept/Close that might
// terminate the server when there is error.
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd, &cache_store);                                             //line:netp:tiny:doit
    close(connfd);                                            //line:netp:tiny:close
  }

// clear up the cache
  cache_destruct(&cache_store);
  free(hostname);
  free(port);
  return 0;
}


/* $end parse_uri */





