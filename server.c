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
#define PORT 1234

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

int fexist(char* p){
	FILE* f = fopen(p,"r");
	if(f == NULL){
		return 0;
	}
	
	fclose(f);
	return 1;
}

int dexist(char* p){
	struct stat s;
	if(stat(p, &s) == 0){
		if((s.st_mode & S_IFMT) == S_IFDIR){
			return 1;
		}
	}
	return 0;
}

typedef struct urlarg_s{
	char* key;
	char* value;
} urlarg;

urlarg* cat_urlarg(urlarg* us, unsigned int* len, urlarg u){
	int l = *len;
	
	urlarg* r = (urlarg*)malloc(sizeof(urlarg) * (l + 1));
	for(int i = 0; i < l; i++){
		r[i].key = us[i].key;
		r[i].value = us[i].value;
	}
	
	r[l].key = u.key;
	r[l].value = u.value;
	
	return r;
}

typedef struct url_s{
	char* path;
	char* handle;
	unsigned int arglen;
	urlarg* args;
} url_t;

urlarg* parse_arg(char* args, unsigned int* len, urlarg** us){
	urlarg uarg;
	uarg.key = "";
	uarg.value = "";
	
	while((*args != '=') && (*args != 0)){
		char t[2] = {*(args++), 0};
		uarg.key = cat(uarg.key,t);
	}
	if(*args == 0){
		if(us == NULL){
			return NULL;
		}
		return *us;
	}
	
	++args;
	
	while((*args != '&') && (*args != 0)){
		char t[2] = {*(args++), 0};
		uarg.value = cat(uarg.value,t);
	}
	if(*args == 0){
		if(us == NULL){
			return NULL;
		}
		return *us;
	}
	
	++args;
	
	if(us == NULL){
		urlarg* t = cat_urlarg(NULL,len,uarg);
		us = &t;
	}else{
		*us = cat_urlarg(*us,len,uarg);
	}
	(*len)++;
	return parse_arg(args,len,us);
}

url_t parse_url(char* u){
	// "/file.html?v=1&b=2"
	char* path = "";
	char* handle = "";
	char* args = "";
	
	char* pu = u;
	while((*pu != '.') && (*pu != 0)){
		char t[2] = {*(pu++), 0};
		path = cat(path,t);
	}
	if(*pu == 0){
		url_t url;
		url.path = path;
		url.handle = "";
		url.arglen = 0;
		url.args = NULL;
		return url;
	}
	
	char* hu = ++pu;
	while((*hu != '?') && (*hu != 0)){
		char t[2] = {*(hu++), 0};
		handle = cat(handle,t);
	}
	if(*hu == 0){
		url_t url;
		url.path = path;
		url.handle = handle;
		url.arglen = 0;
		url.args = NULL;
		return url;
	}
	
	char* au = ++hu;
	while(*au != 0){
		char t[2] = {*(au++), 0};
		args = cat(args,t);
	}
	
	url_t url;
	url.path = path;
	url.handle = handle;
	url.arglen = 0;
	url.args = NULL;
	url.args = parse_arg(args,&url.arglen,NULL);
	return url;
}

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
