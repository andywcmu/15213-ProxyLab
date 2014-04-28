/* truck.c */
/* TRUCK - Terrific Recently Used Cache Kick-ass. */

#include "csapp.h"
#include "truck.h"
#include "contracts.h"

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400



void cache_print (struct cache_header *C) {
	REQUIRES (C != NULL);
	fprintf(stdout, "CACHE PRINT: size: %d num: %d\n", C->cache_size, C->cache_block_num);
	fprintf(stdout, "####### START #######\n");
	struct cache_block *ptr = C->start;
	while (ptr != C->end) {
		fprintf(stdout, "%s\t%zu\n", ptr->object_name, ptr->object_size);
		ptr = ptr->next;
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
	C->cache_size += cb->object_size;
	C->cache_block_num += 1;

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
	Free(start->object);
	Free(start->object_name);
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

struct cache_block *cache_find (struct cache_header *C, char *uri) {
	REQUIRES (C != NULL);
	struct cache_block *ptr = C->start;
	while (ptr != C->end) {
		if (!strcmp(uri, ptr->object_name)) {
			/* found the object */
			// if the block is already a LRU
			if (ptr->next == C->end) {
				return ptr;
			}
			// otherwise, move it to the end and return
			else {
				struct cache_block *old = Malloc (sizeof(struct cache_block));
				old->object_name = ptr->object_name;
				old->object_size = ptr->object_size;
				old->object = ptr->object;
				old->next = ptr->next;

				cache_delete(C, ptr);
				cache_add_to_end(C, old);
				return old;
			}

		}
		ptr = ptr->next;
	}
	/* not found */
	return NULL;
}

void cache_insert (struct cache_header *C, char *uri, char *object, size_t object_size) {
	REQUIRES (C != NULL);

	if (object_size > MAX_OBJECT_SIZE) return;

	char *copied_object = Malloc(object_size);
	memcpy(copied_object, object, object_size);

	size_t object_uri_size = strlen(uri);
	char *copied_object_uri = Malloc(object_uri_size);
	memcpy(copied_object_uri, uri, object_uri_size);

	struct cache_block *new = Malloc(sizeof(struct cache_block));
	new->object_name = copied_object_uri;
	new->object_size = object_size;
	new->object = copied_object;
	cache_add_to_end(C, new);
	return;
}





