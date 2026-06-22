#ifndef SPHEN_STR_H
#define SPHEN_STR_H

#include "types.h"

typedef struct {
	char* data;
	size_t len;
	size_t cap;
} str_t;
extern str_t str_new(const char*);
extern int str_app(str_t*, const char*);
extern int str_push(str_t*, char);
extern void str_free(str_t*);



#endif
