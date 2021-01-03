/*
 * author: Hongshuo Wang
 * email: hkw5146@psu.edu
 * parse_url.c for CMPSC 311 Fall 2019
 * last updated: 11/20/2019
 */
#include "parse_url.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// returns 1 if the url starts with http:// (case insensitive)
// and returns 0 otherwise
// If the url is a proper http scheme url, parse out the three
// components of the url: host, port and path and copy
// the parsed strings to the supplied destinations.
// Remember to terminate your strings with 0, the null terminator.
// If the port is missing in the url, copy "80" to port.
// If the path is missing in the url, copy "/" to path.
int parse_url(const char *url, char *host, char *port, char *path) {

int max_buf = 8096; 
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
    char *t1 = malloc(max_buf);
    char *t2 = malloc(max_buf);
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
