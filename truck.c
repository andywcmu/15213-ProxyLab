/* truck.c */
/* TRUCK - Terrific Recently Used Cache Kick-ass. */

#include "csapp.h"
#include "truck.h"

/* interface */
struct cache_header *init ();
char *cache_find (char *uri);
void cache_insert (char *uri, char *object);


struct cache_header {
	struct cache_block *start;
	struct cache_block *end;
	size_t cache_size;
};

struct cache_block {
	struct cache_block *next;
	size_t time_stamp;
	char *file_name;
	char *object;
}

struct cache_header *init () {
	struct cache_header *new = Malloc (sizeof(cache_header));
	struct cache_block *dummy = Malloc (sizeof(cache_block));
	new->start = dummy;
	new->end = dummy;
	return new;
}



