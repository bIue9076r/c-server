#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IF_00(n) if(n == 0){return 1;}
#define IF_FF(n) if(n == -1){return 1;}

#define IF_00_E(n,e) if(n == 0){perror(e); return 1;}
#define IF_FF_E(n,e) if(n == -1){perror(e); return 1;}

#define SLOG(m) printf("[Server]: %s\n",m)
#define SLOG_N(m,n) printf("[Server]: %s: %d\n",m,n)

#define NOTFOUND() response = "HTTP/1.1 404 NOTOK\r\n"

#define REQUEST_LEN 1024
#define PORT 1234

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

char request[REQUEST_LEN];
char* response;

void GET(char* p){
	response = "HTTP/1.1 200 OK\r\n";
	response = cat(response,"Content-Type: text/html\r\n\r\n");
	response = cat(response,"<!DOCTYPE html><html><head><title>Title</title></head><body><h1>Hello World</h1><script src=\"/main.js\"></script></body></html>\r\n");
}

void POST(char* p, char* b){
	response = "HTTP/1.1 200 OK\r\n";
	response = cat(response,"Content-Type: text/html\r\n\r\n");
	response = cat(response,"<!DOCTYPE html><html><head><title>Title</title></head><body><h1>Hello World</h1><script src=\"/main.js\"></script></body></html>\r\n");
}

void E400(void){
	response = "HTTP/1.1 400 WHAT\r\n";
	response = cat(response,"Content-Type: text/html\r\n\r\n");
	response = cat(response,"<!DOCTYPE html><html><head><title>400 Error</title></head><body><h1>400 Error</h1></body></html>\r\n");
}

void clearRequest(char req[REQUEST_LEN]){
	for(int i = 0; i < REQUEST_LEN; i++){
		req[i] = 0;
	}
}

char* handleRequest(char req[REQUEST_LEN]){
	char* rq = req;
	int len = stl(req);
	
	char* r = "";
	char* p = "";
	char* b = "";
	
	if(*rq == 0){
		response = "HTTP/1.1 400 WHAT\r\n";
		return response;
	}
	
	while((*(rq++) != ' ') && ((--len) != 0)){
		char c[2] = {*(rq - 1),0};
		r = cat(r, c);
	}
	
	while((*(rq++) != ' ') && ((--len) != 0)){
		char c[2] = {*(rq - 1),0};
		p = cat(p, c);
	}
	
	char check[5];
	if(len - 2 > 0){
		check[4] = 0;
		check[0] = *(rq); check[1] = *(rq + 1);
		check[2] = *(rq + 2); check[3] = *(rq + 3);
		rq++;
		len--;
		while(
		(strequ(check,"\r\n\r\n") != 0) && (len - 4 > 0)
		){
			check[0] = *(rq); check[1] = *(rq + 1);
			check[2] = *(rq + 2); check[3] = *(rq + 3);
			rq++;
			len--;
		}
		if(strequ(check,"\r\n\r\n") == 0){
			len -= 3;
			rq += 3;
		}
	}
	
	b = cat(b,rq);
	
	printf("[Server]: [Request]: [%s], [%s], [%s]\n",r,p,b);
	
	switch(stn(r)){
		case 225:
			GET(p);
			return response;
		break;
		
		case 327:
			POST(p,b);
			return response;
		break;
		
		default:
			E400();
			return response;
		break;
	}
}

int main(void){
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	
	int fd = socket(AF_INET,SOCK_STREAM,0);
	IF_FF_E(fd, "Socket Fail")
	
	IF_FF_E(bind(fd,(struct sockaddr*)&addr,sizeof(addr)), "Bind Fail")
	
	IF_FF_E(listen(fd, 3), "Listen Fail")
	
	SLOG("Hellow From BIue Server");
	SLOG_N("Port",PORT);
	while(1){
		int soc = accept(fd,(struct sockaddr*)&addr,(socklen_t*)&addr_len);
		if(soc == -1){
			perror("Accept Fail");
			continue;
		}
		
		clearRequest(request);
		read(soc, request, REQUEST_LEN);
		request[REQUEST_LEN - 1] = 0;
		
		handleRequest(request);
		
		send(soc, response, stl(response), 0);
		close(soc);
	}
	
	close(fd);
	return 0;
}
