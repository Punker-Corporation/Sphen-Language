#include "str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

inline str_t str_new(const char* s) {
	size_t len = (!s) ? 0 : strlen(s);
	size_t cap = (len + 1 < 8)? 8 : len + 1;
	char* data = malloc(cap);
	if(!data){
		puts("STRING:NEW:MALLOC:ERROR");
		exit(EXIT_FAILURE);
	}
	if(s)
		memcpy(data, s, len);
	data[len] = '\0';
	return (str_t){
        .data = data,
        .len = len,
        .cap = cap
    };
}


int str_app(str_t* s1, const char* s2) {
	if(!s1 || !s1->data || !s2) return false;
	size_t s2_len = strlen(s2);
	
	while(s1->cap - s1->len <= s2_len){
		if(s1->cap > SIZE_MAX / 2) return false;
		size_t new_cap = s1->cap *= 2;
		char* tmp = realloc(s1->data, new_cap);
		if(!tmp){
			puts("STRING:APPEND:REALLOC:ERROR");
			exit(EXIT_FAILURE);
		}
		s1->cap = new_cap;
		s1->data = tmp;
	}
	memcpy(s1->data + s1->len, s2, s2_len);
	s1->len += s2_len;
	s1->data[s1->len] = '\0';
	
	return true;
}

int str_push(str_t* s, const char c) {
	if(!s || !s->data || !c) return false;
	
	if(s->len + 1 >= s->cap ){
		if(s->cap > SIZE_MAX / 2) return false;
		size_t new_cap = s->cap *= 2;
		char* tmp = realloc(s->data, new_cap);
		if(!tmp){
			puts("STRING:PUSH:REALLOC:ERROR");
			exit(EXIT_FAILURE);
		}
		s->cap = new_cap;
		s->data = tmp;
	}
	s->data[s->len] = c;
	s->len++;
	s->data[s->len] = '\0';
	
	return true;
}

void str_free(str_t* s) {
	if(!s) return;

    free(s->data);
    
	s->data = NULL;
	s->len = 0;
	s->cap = 0;
}
