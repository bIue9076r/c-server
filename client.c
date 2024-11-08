#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "lib.h"
#include "lib.c"

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
		int soc = connect(fd,(struct sockaddr*)&addr,addr_len);
		if(soc == -1){
			perror("Connect Fail");
			continue;
		}
		
		request = "";//"GET /";
		SLOG(request);
		send(soc, request, stl(request) - 1, 0);
		read(soc, response, RESPONSE_LEN);
		response[RESPONSE_LEN - 1] = 0;

		close(soc);
	}
	close(fd);
	return 0;
}
