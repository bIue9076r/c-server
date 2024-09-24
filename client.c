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

#define SLOG(m) printf("[Client]: %s\n",m)
#define SLOG_N(m,n) printf("[Client]: %s: %d\n",m,n)

#define REQUEST_LEN 1024
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

int epow(int b, int e){
	int r = 1;
	for(int i = 0; i < e; i++){
		r = r * b;
	}
	return r;
}

unsigned int stl(char* s){
	unsigned int l = 1;
	while(*(s++) != 0){
		l++;
	}
	return l;
}

int stn(char* s){
	int v = 0;
	while(*(s++) != 0){
		v += *(s - 1);
	}
	return v + stl(s);
}

char* its(int it){
	char* r = malloc(sizeof(char) * 16);
	char* t = r;
	for(int i = 0; i < 16; i++){ *r = 0; }
	int sign = 0;
	int highest = 1;
	int index = 0;
	if(it < 0){ it = -it; sign = 1;}
	
	if(sign){
		*r = *(t++) = '-';
	}
	
	while(it >= (highest * 10)){
		highest = highest * 10;
	}
	
	while(highest > 0){
		int d = it / highest;
		*(t++) = '0' + d;
		
		it = it - d * highest;
		highest = highest / 10;
	}
	
	return r;
}

int strequ(char* s1, char* s2){
	return stn(s1) - stn(s2);
}

char* cat(char* s1, char* s2){
	int l1 = stl(s1);
	int l2 = stl(s2);
	
	char* r = (char*)malloc(sizeof(char) * (l1 + l2 - 1));
	for(int i = 0; i < l1; i++){
		r[i] = s1[i];
	}
	for(int i = 0; i < l2; i++){
		r[(l1 - 1) + i] = s2[i];
	}
	
	r[(l1 + l2) - 2] = 0;
	
	return r;
}

char* cap(char* s, char c){
	int l1 = stl(s);
	
	char* r = (char*)malloc(sizeof(char) * (l1 + 1));
	for(int i = 0; i < l1; i++){
		r[i] = s[i];
	}
	
	r[l1 - 1] = c;
	r[l1] = 0;
	
	return r;
}

char* catn(char* s1, unsigned int* n1, char* s2, unsigned int* n2){
	int l1 = *n1;
	int l2 = *n2;
	
	char* r = (char*)malloc(sizeof(char) * (l1 + l2 - 1));
	for(int i = 0; i < l1; i++){
		r[i] = s1[i];
	}
	for(int i = 0; i < l2; i++){
		r[(l1 - 1) + i] = s2[i];
	}
	
	r[(l1 + l2) - 2] = 0;
	
	*n1 = (l1 + l2 - 1);
	return r;
}

char* capn(char* s, unsigned int *n, char c){
	int l1 = *n;
	
	char* r = (char*)malloc(sizeof(char) * (l1 + 1));
	for(int i = 0; i < l1; i++){
		r[i] = s[i];
	}
	
	r[l1 - 1] = c;
	r[l1] = 0;
	
	*n = l1 + 1;
	return r;
}

#define RESPONSE_LEN 0xFFFF
char* request;
char response[RESPONSE_LEN];

int main(void){
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	
	int fd = socket(AF_INET,SOCK_STREAM,0);
	IF_FF_E(fd, "Socket Fail")
	
	SLOG("Hellow To BIue Server");
	SLOG_N("Port",PORT);
	while(1){
		int soc = connect(fd,(struct sockaddr*)&addr,(socklen_t*)&addr_len);
		if(soc == -1){
			perror("Connect Fail");
			continue;
		}
		
		request = "GET /";
		printf("%s\n",request);
		send(soc, request, stl(request) - 1, 0);
		read(soc, response, RESPONSE_LEN);
		response[RESPONSE_LEN - 1] = 0;

		close(soc);
	}
	close(fd);
	return 0;
}
