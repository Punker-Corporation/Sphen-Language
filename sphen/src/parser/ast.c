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
        printf("AST::INIT::NULL\n");
        return 0;
    }
    v->len = 0;
    v->cap = 256;
    v->data = (AST_node*)malloc(v->cap * sizeof(AST_node));
    if (v->data == NULL) {
        printf("AST::INIT::ALLOC\n");
        exit(EXIT_FAILURE);
    }
    
    return 1;
}

AST_node getnode(AST_vec* v, Node_id index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        printf("AST::GET::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        printf("AST::GET::OUT_OF_BOUNDS\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

int pushnode(AST_vec* v, const AST_node ast) {
    if (v == NULL || v->data == NULL) {
        printf("AST::PUSH::NULL\n");
        return 0;
    }
    if (v->len >= v->cap) {
        size_t new_cap = v->cap + 256;
        if (new_cap < v->cap) {
            printf("AST::PUSH::OVERFLOW\n");
            exit(EXIT_FAILURE);
        }
        AST_node* tmp = (AST_node*)realloc(v->data, new_cap * sizeof(AST_node));
        if (tmp == NULL) {
            printf("AST::PUSH::ALLOC\n");
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
        printf("VEC_ID::INIT::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (a == NULL) {
        printf("VEC_ID::INIT::ARENA::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (cap == 0)
        cap = 1;
    
    v->len = 0;
    v->cap = cap;
    v->data = (Node_id*)arena_alloc(a, sizeof(Node_id) * cap);
    if (v->data == NULL) {
        printf("VEC_ID::INIT::ALLOC\n");
        exit(EXIT_FAILURE);
    }
    v->arena = a;
    return 1;
}

Node_id getvecid(Vec_id* v, size_t index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        printf("VEC_ID::GET::NULL\n");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        printf("VEC_ID::GET::NULL\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

void pushvecid(Vec_id* v, Node_id val) {
    if (v->len >= v->cap) {
        size_t newcap = v->cap * 2;

        Node_id* newdata = arena_alloc(v->arena, sizeof(Node_id) * newcap);
        memcpy(newdata, v->data, sizeof(Node_id) * v->len);

        v->data = newdata;
        v->cap = newcap;
    }

    v->data[v->len++] = val;

}

int delvecid(Vec_id* v) {
    if (v == NULL) return 0;
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    return 1;
}
