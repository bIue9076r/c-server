#ifndef _LIB_H
#define _LIB_H
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define C10e0 1
#define C10e1 10
#define C10e2 100
#define C10e3 1000
#define C10e4 10000
#define C10e5 100000
#define C10e6 1000000
#define C10e7 10000000
#define C10e8 100000000
#define C10e9 1000000000
#define C10e10 10000000000
#define C10e11 100000000000
#define C10e12 1000000000000
#define C10e13 10000000000000
#define C10e14 100000000000000

#define IF_00(n) if(n == 0){return 1;}
#define IF_FF(n) if(n == -1){return 1;}

#define IF_00_E(n,e) if(n == 0){perror(e); return 1;}
#define IF_FF_E(n,e) if(n == -1){perror(e); return 1;}

#define SLOG(m) printf("[Server]: %s\n",m)
#define SLOG_N(m,n) printf("[Server]: %s: %d\n",m,n)

#define REQUEST_LEN 1024
#define RESPONSE_LEN 0xFFFF
#define PORT 4321

#define COMPATH(f,h) cat(f,cat(".",h))

#define GET_REQUEST 225
#define POST_REQUEST 327
#define PNG_HANDLE 326
#define JPG_HANDLE 322
#define JPEG_HANDLE 423
#define MP4_HANDLE 274
#define HTML_HANDLE 438
#define JS_HANDLE 222
#define CSS_HANDLE 335
#define PATH "./public"

typedef struct urlarg_s{
	char* key;
	char* value;
} urlarg;

typedef struct url_s{
	char* path;
	char* handle;
	unsigned int arglen;
	urlarg* args;
} url_t;

int epow(int, int);
unsigned int stl(char*);
int stn(char*);
char* its(int);
int strequ(char*, char*);
char* cat(char*, char*);
char* cap(char*, char);
char* catn(char*, unsigned int*, char*, unsigned int*);
char* capn(char*, unsigned int*, char);
int fexist(char*);
int dexist(char*);
urlarg* cat_urlarg(urlarg*, unsigned int*, urlarg);
urlarg* parse_arg(char*, unsigned int*, urlarg**);
url_t parse_url(char*);

#endif