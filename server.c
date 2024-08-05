#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IF_00(n) if(n == 0){return 1;}
#define IF_FF(n) if(n == -1){return 1;}

#define IF_00_E(n,e) if(n == 0){perror(e); return 1;}
#define IF_FF_E(n,e) if(n == -1){perror(e); return 1;}

#define SLOG(m) printf("[Server]: %s\n",m)
#define SLOG_N(m,n) printf("[Server]: %s: %d\n",m,n)

#define REQUEST_LEN 1024
#define PORT 1234

char request[REQUEST_LEN];
char* response;

int main(void){
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(1234);
	
	int fd = socket(AF_INET,SOCK_STREAM,0);
	IF_FF_E(fd, "Socket Fail")
	
	IF_FF_E(bind(fd,(struct sockaddr*)&addr,sizeof(addr)), "Bind Fail")
	
	IF_FF_E(listen(fd, 3), "Listen Fail")
	
	SLOG("Hellow From BIue Server");
	SLOG_N("Port",PORT);
	int soc = accept(fd,(struct sockaddr*)&addr,(socklen_t*)&addr_len);
	IF_FF_E(soc, "Accept Fail")
	
	read(soc, request, REQUEST_LEN);
	request[REQUEST_LEN - 1] = 0;
	response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>Title</title></head><body><h1>Hello World</h1></body></html>\r\n";
	send(soc, response, strlen(response), 0);
	
	close(soc);
	close(fd);
	return 0;
}
