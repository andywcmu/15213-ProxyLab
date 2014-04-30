/*
 * Terrific Recently Used Cache Kick-ass.
 * truck.h - interface of cache.
 */
#include "csapp.h"


/*
 * struct of single cache block
 */
struct cache_block {
	struct cache_block *next;
	size_t object_size;
	char *object_name;
	char *object;
};

/*
 * Init a cache object.
 */
struct cache_header *cache_init ();

/*
 * Find an object in the cache with the given uri. If the object exists in
 * cache, a pointer to it is returned. Otherwise NULL is returned.
 *
 * If the object exists in cache, it also automatically perform a cache hit
 * operation.
 */
struct cache_block *cache_find (struct cache_header *C, char *uri);

/*
 * Add a data object to the cache. Automatically evict LRU object(s) if cache
 * is full.
 */
void cache_insert (struct cache_header *C,
    char *uri, char *object, size_t object_size);