/* truck.c */
/* TRUCK - Terrific Recently Used Cache Kick-ass. */

#include "csapp.h"
#include "truck.h"
#include "contracts.h"

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

struct cache_header {
	struct cache_block *start;
	struct cache_block *end;
	int cache_size;
	int cache_block_num;
};

struct cache_block {
	struct cache_block *next;
	size_t object_size;
	char *object_name;
	char *object;
};

void cache_print (struct cache_header *C) {
	REQUIRES (C != NULL);
	fprintf(stdout, "CACHE PRINT: size: %d num: %d\n", C->cache_size, C->cache_block_num);
	fprintf(stdout, "####### START #######\n");
	struct cache_block *ptr = C->start;
	while (ptr != C->end) {
		fprintf(stdout, "%s\t%zu\n", ptr->object_name, ptr->object_size);
	}
	fprintf(stdout, "######## END ########\n");
}

struct cache_header *cache_init () {
	struct cache_header *new = Malloc (sizeof(struct cache_header));
	struct cache_block *dummy = Malloc (sizeof(struct cache_block));
	new->start = dummy;
	new->end = dummy;
	new->cache_size = 0;
	new->cache_block_num = 0;
	return new;
}

void cache_add_to_end (struct cache_header *C, struct cache_block *cb) {
	REQUIRES (C != NULL);
	REQUIRES (C->end != NULL);
	REQUIRES (cb != NULL);
	C->end->object_name = cb->object_name;
	C->end->object = cb->object;
	C->end->object_size = cb->object_size;
	/* the original cb becomes a dummy node */
	C->end->next = cb;
	C->end = cb;
	return;
}

void cache_evict (struct cache_header *C) {
	REQUIRES(C != NULL);
	REQUIRES(C->cache_block_num != 0);
	struct cache_block *start = C->start;
	C->cache_size -= start->object_size;
	C->cache_block_num -= 1;
	ASSERT (0 <= C->cache_size && C->cache_size <= MAX_CACHE_SIZE);
	ASSERT (C->cache_block_num >= 0);

	C->start = C->start->next;
	Free(start);
	return;
}

void cache_delete (struct cache_header *C, struct cache_block *cb) {
	REQUIRES (C != NULL);
	REQUIRES (cb != NULL);
	/* if the node is the last node */
	if (cb->next == C->end) {
		// In our implementation, this will happen only if the cache
		// consists of exactly one block.
		if (C->cache_block_num == 1) {
			C->end = C->start;
			C->cache_block_num = 0;
			ASSERT (C->cache_size == cb->object_size);
			C->cache_size = 0;
		}
		else {
			fprintf(stderr, "cache_delete invariant error.\n");
		}
	}
	/* the node is somewhere in the middle */
	else {
		C->cache_block_num -= 1;
		C->cache_size -= cb->object_size;
		ASSERT (0 <= C->cache_size && C->cache_size <= MAX_CACHE_SIZE);
		ASSERT (C->cache_block_num >= 0);
		cb->object_name = cb->next->object_name;
		cb->object_size = cb->next->object_size;
		cb->object = cb->next->object;
		cb->next = cb->next->next;
	}
	return;
}

char *cache_find (struct cache_header *C, char *uri) {
	REQUIRES (C != NULL);
	struct cache_block *ptr = C->start;
	while (ptr != C->end) {
		if (!strcmp(uri, ptr->object_name)) {
			/* found the object */
			// if the block is already a LRU
			if (ptr->next == C->end) {
				return ptr->object;
			}
			// otherwise, move it to the end and return
			else {
				char *ret = ptr->object;
				cache_delete(C, ptr);
				cache_add_to_end(C, ptr);
				return ret;
			}

		}
	}
	/* not found */
	return NULL;
}

void cache_insert (struct cache_header *C, char *uri, char *object) {
	REQUIRES (C != NULL);
	size_t object_size = strlen(object);
	if (object_size > MAX_OBJECT_SIZE) return;
	struct cache_block *new = Malloc (sizeof(struct cache_block));
	new->object_name = uri;
	new->object_size = object_size;
	new->object = object;
	cache_add_to_end(C, new);
	return;
}





