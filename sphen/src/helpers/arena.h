#ifndef SPHEN_MEMORY_ARENA_H
#define SPHEN_MEMORY_ARENA_H

#include "types.h"

typedef struct {
	u8_t* data;
	size_t cap;
	size_t offset;
} Arena;

extern void arena_init(Arena* a, size_t capacity);
extern void* arena_alloc(Arena* a, size_t size);
extern void arena_reset(Arena* a);
extern void arena_free(Arena* a);

#endif
