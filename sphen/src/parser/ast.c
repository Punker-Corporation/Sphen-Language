#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static inline int safe_mul_size(size_t a, size_t b, size_t *result) {
    if (a != 0 && b > SIZE_MAX / a) return 0;
    *result = a * b;
    return 1;
}

int newastvec(AST_vec* v) {
    if (v == NULL) {
        puts("AST::INIT::NULL\n");
        return 0;
    }
    v->len = 0;
    size_t bytes = 0;
    if(!safe_mul_size(v->cap, sizeof(AST_node), &bytes)){
    	puts("AST::INIT::OVERFLOW");
    	exit(EXIT_FAILURE);
    }
    v->cap = 256;
    v->data = (AST_node*)malloc(bytes);
    if (v->data == NULL) {
        puts("AST::INIT::ALLOC");
        exit(EXIT_FAILURE);
    }
    
    return 1;
}

AST_node getnode(AST_vec* v, Node_id index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        puts("AST::GET::NULL");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        puts("AST::GET::OUT_OF_BOUNDS");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

int pushnode(AST_vec* v, const AST_node ast) {
    if (v == NULL || v->data == NULL) {
        puts("AST::PUSH::NULL\n");
        return 0;
    }
    if (v->len >= v->cap) {
        size_t new_cap = v->cap + 256;
        if (new_cap < v->cap) {
            puts("AST::PUSH::OVERFLOW\n");
            exit(EXIT_FAILURE);
        }
        size_t bytes = 0;
		if(!safe_mul_size(new_cap, sizeof(AST_node), &bytes)){
			puts("AST::PUSH::REALLOC::OVERFLOW");
			exit(EXIT_FAILURE);
		}
        
        AST_node* tmp = (AST_node*)realloc(v->data, bytes);
        if (tmp == NULL) {
            puts("AST::PUSH::ALLOC\n");
            exit(EXIT_FAILURE);
        }
        v->data = tmp;
        v->cap = new_cap;
    }
    v->data[v->len++] = ast;
    return 1;
}

int delast(AST_vec* v) {
    if (v == NULL) return 0;
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    return 1;
}


int newvecid(Vec_id* v, Arena* a, size_t cap) {
    if (v == NULL) {
        puts("VEC_ID::INIT::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (a == NULL) {
        puts("VEC_ID::INIT::ARENA::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (cap == 0)
        cap = 1;
    
    v->len = 0;
    v->cap = cap;
    
    size_t bytes = 0;
	if(!safe_mul_size(cap, sizeof(Node_id), &bytes)){
		puts("VEC_ID::INIT::OVERFLOW");
		exit(EXIT_FAILURE);
	}
    
    v->data = (Node_id*)arena_alloc(a, bytes);
    if (v->data == NULL) {
        puts("VEC_ID::INIT::ALLOC\n");
        exit(EXIT_FAILURE);
    }
    v->arena = a;
    return 1;
}

Node_id getvecid(Vec_id* v, size_t index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        puts("VEC_ID::GET::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        puts("VEC_ID::GET::NULL\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

void pushvecid(Vec_id* v, Node_id val) {
	if (v->len >= v->cap) {
		size_t newcap = v->cap * 2;
		size_t old_off = (size_t)((u8_t*)v->data - v->arena->data);

		Node_id* newdata = arena_alloc(v->arena, sizeof(Node_id) * newcap);
		Node_id* olddata = (Node_id*)(v->arena->data + old_off);
		memcpy(newdata, olddata, sizeof(Node_id) * v->len);
		
		v->data = newdata;
		v->cap = newcap;
	}

	v->data[v->len++] = val;
}

int delvecid(Vec_id* v) {
    if (v == NULL) return 0;
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    v->arena = NULL;
    return 1;
}
