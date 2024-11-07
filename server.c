#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "lib.h"
#include "lib.c"

char request[REQUEST_LEN];
char* response;

void E400(void){
	response = "HTTP/1.1 400 WHAT\r\n";
	response = cat(response,"Content-Type: text/html\r\n\r\n");
	response = cat(response,"400: Error\r\n");
}

void E404(void){
	response = "HTTP/1.1 404 NOTFOUND\r\n";
	response = cat(response,"Content-Type: text/html\r\n\r\n");
	response = cat(response,"404: Not found\r\n");
}

void GET(url_t url){
	int dir = 0;
	char* real_path = cat(PATH,COMPATH(url.path, url.handle));
	
	if(dexist(COMPATH(url.path, url.handle))){
		url.path = cat(cat(url.path, url.handle),"index.html");
		real_path = cat(PATH,url.path);
		dir = 1;
	}
	
	if(!fexist(real_path)){
		return E404();
	}
	
	FILE* file = fopen(real_path,"r");
	if(file == NULL){
		return E400();
	}
	
	char* file_content = "";
	unsigned int file_content_len = 1;
	while(!feof(file)){
		char chr = 0;
		fread(&chr, sizeof(char), 1, file);
		if(!feof(file)){
			file_content = capn(file_content,&file_content_len,chr);
		}
	}
	
	response = "HTTP/1.1 200 OK\r\n";
	
	// TODO: check for handle
	switch(stn(url.handle)){
		case PNG_HANDLE:
		response = cat(response,"Content-Type: image/png\r\n");
		response = cat(response,"Content-Lenght: ");
		response = cat(response,its(file_content_len));
		break;
		
		case JPG_HANDLE:
		response = cat(response,"Content-Type: image/jpg\r\n");
		response = cat(response,"Content-Lenght: ");
		response = cat(response,its(file_content_len));
		break;
		
		case JPEG_HANDLE:
		response = cat(response,"Content-Type: image/jpeg\r\n");
		response = cat(response,"Content-Lenght: ");
		response = cat(response,its(file_content_len));
		break;
		
		case MP4_HANDLE:
		response = cat(response,"Content-Type: video/mp4\r\n");
		response = cat(response,"Content-Lenght: ");
		response = cat(response,its(file_content_len));
		break;
		
		case HTML_HANDLE:
		response = cat(response,"Content-Type: text/html");
		break;
		
		case JS_HANDLE:
		response = cat(response,"Content-Type: text/js");
		break;
		
		case CSS_HANDLE:
		response = cat(response,"Content-Type: text/css");
		break;
		
		default:
		response = cat(response,"Content-Type: text/");
		response = (dir)?(cat(response,"html")):(cat(response,url.handle));
		break;
	}
	
	response = cat(response,"\r\n\r\n");
	
	unsigned int response_len = stl(response);
	response = catn(response,&response_len,file_content,&file_content_len);
	//response = cat(response,file_content);
	SLOG(response);
}

void POST(url_t url, char* b){
	response = "HTTP/1.1 200 OK\r\n";
	response = cat(response,"Content-Type: text/text\r\n\r\n");
	response = cat(response,"Goodbye World");
}

void clearRequest(char req[REQUEST_LEN]){
	for(int i = 0; i < REQUEST_LEN; i++){
		req[i] = 0;
	}
}

char* handleRequest(char req[REQUEST_LEN]){
	char* rq = req;
	int rq_len = stl(req);
	
	char* r = "";
	char* p = "";
	char* b = "";
	
	if(*rq == 0){
		response = "HTTP/1.1 400 WHAT\r\n";
		return response;
	}
	
	while((*(rq++) != ' ') && ((--rq_len) != 0)){
		char c[2] = {*(rq - 1),0};
		r = cat(r, c);
	}
	
	while((*(rq++) != ' ') && ((--rq_len) != 0)){
		char c[2] = {*(rq - 1),0};
		p = cat(p, c);
	}
	
	char check[5];
	if(rq_len - 2 > 0){
		check[4] = 0;
		check[0] = *(rq); check[1] = *(rq + 1);
		check[2] = *(rq + 2); check[3] = *(rq + 3);
		rq++;
		rq_len--;
		while((strequ(check,"\r\n\r\n") != 0) && (rq_len - 4 > 0)){
			check[0] = *(rq); check[1] = *(rq + 1);
			check[2] = *(rq + 2); check[3] = *(rq + 3);
			rq++;
			rq_len--;
		}
		if(strequ(check,"\r\n\r\n") == 0){
			rq_len -= 3;
			rq += 3;
		}
	}
	
	b = cat(b,rq);
	
	printf("[Server]: [Request]: [%s], [%s], [%s]\n",r,p,b);
	
	switch(stn(r)){
		case GET_REQUEST:
			GET(parse_url(p));
			return response;
		break;
		
		case POST_REQUEST:
			POST(parse_url(p),b);
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
		
		send(soc, response, stl(response) - 1, 0);
		close(soc);
	}
	
	close(fd);
	return 0;
}
