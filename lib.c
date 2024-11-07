#ifndef _LIB_C
#define _LIB_C
#include "lib.h"

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

#endif