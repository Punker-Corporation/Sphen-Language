#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static inline int safe_mul_size(size_t a, size_t b, size_t *result) {
    if (a != 0 && b > SIZE_MAX / a) return 0;
    *result = a * b;
    return 1;
}

int newastvec(ASTVec* v) {
    if (v == NULL) {
        printf("ast vec does not exist\n");
        return 0;
    }
    v->len = 0;
    v->cap = 256;
    v->data = (ASTNode*)malloc(v->cap * sizeof(ASTNode));
    if (v->data == NULL) {
        printf("ast memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

ASTNode getnode(ASTVec* v, NodeId index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        printf("ast vec is null, uninitialized, or empty\n");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        printf("ast node index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

int pushnode(ASTVec* v, const ASTNode ast) {
    if (v == NULL || v->data == NULL) {
        printf("ast vec is null or uninitialized\n");
        return 0;
    }
    if (v->len >= v->cap) {
        size_t new_cap = v->cap + 256;
        if (new_cap < v->cap) {
            printf("capacity overflow\n");
            exit(EXIT_FAILURE);
        }
        ASTNode* tmp = (ASTNode*)realloc(v->data, new_cap * sizeof(ASTNode));
        if (tmp == NULL) {
            printf("ast memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        v->data = tmp;
        v->cap = new_cap;
    }
    v->data[v->len++] = ast;
    return 1;
}

int delast(ASTVec* v) {
    if (v == NULL) return 0;
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    return 1;
}

int newvecid(VecId* v, unsigned long cap) {
    if (v == NULL) {
        printf("vec id does not exist\n");
        return 0;
    }
    if (cap == 0) {
        cap = 1;
    }
    v->len = 0;
    v->cap = cap;
    v->data = (NodeId*)malloc(v->cap * sizeof(NodeId));
    if (v->data == NULL) {
        printf("vec id memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

NodeId getvecid(VecId* v, NodeId index) {
    if (v == NULL || v->data == NULL || v->len == 0) {
        printf("vec id is null, uninitialized, or empty\n");
        exit(EXIT_FAILURE);
    }
    if (index >= v->len) {
        printf("vec id index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

int pushvecid(VecId* v, NodeId id) {
    if (v == NULL || v->data == NULL) {
        printf("vec id is null or uninitialized\n");
        return 0;
    }
    if (v->len >= v->cap) {
        size_t new_cap;
        if (!safe_mul_size(v->cap, 2, &new_cap)) {
            printf("capacity overflow\n");
            exit(EXIT_FAILURE);
        }
        NodeId* tmp = (NodeId*)realloc(v->data, new_cap * sizeof(NodeId));
        if (tmp == NULL) {
            printf("vec id memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        v->data = tmp;
        v->cap = new_cap;
    }
    v->data[v->len++] = id;
    return 1;
}

int delvecid(VecId* v) {
    if (v == NULL) return 0;
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
    return 1;
}
