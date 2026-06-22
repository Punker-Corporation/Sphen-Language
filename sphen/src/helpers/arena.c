#include "arena.h"
#include <stdlib.h>
#include <stdio.h>

void arena_init(Arena* a, size_t capacity) {
    a->data = malloc(capacity);
    a->cap = capacity;
    a->offset = 0;
	if(a->data == NULL){
		puts("ARENA::INIT::ERROR\n");
		exit(EXIT_FAILURE);
	}
}
void* arena_alloc(Arena* a, size_t size) {
	if(a == NULL || a->data == NULL){
		puts("ARENA::ALLOC::NULL\n");
		exit(EXIT_FAILURE);
	}
	void* ptr;
	if (a->offset + size >= a->cap) {
		size_t new_cap = a->cap;
        new_cap *= 2;
        u8_t* tmp = (u8_t*)realloc(a->data, new_cap);

        if(a->cap > new_cap){
          	puts("ARENA::ALLOC::OVERFLOW\n");
       	    exit(EXIT_FAILURE);
        }
        if(!tmp){
        	puts("ARENA::ALLOC::ERROR\n");
        	exit(EXIT_FAILURE);
        }
    	a->cap = new_cap;
    	a->data = tmp;
    }
    ptr = a->data + a->offset;
    a->offset += size;
    return ptr;
}
void arena_reset(Arena* a) {
    if(a == NULL || a->data == NULL){
   		puts("ARENA::RESET::NULL\n");
   		exit(EXIT_FAILURE);
   	}
   	a->offset = 0;
}
void arena_free(Arena* a) {
    if(a == NULL || a->data == NULL) return;
   	free(a->data);
   	a->data = NULL;
   	a->cap = a->offset = 0;
}
