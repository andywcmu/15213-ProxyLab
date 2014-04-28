/* interface */

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

/*
 * Init a cache object.
 */
struct cache_header *cache_init ();

/*
 * Return a pointer to the data object.
 */
struct cache_block *cache_find (struct cache_header *C, char *uri);

/*
 * Add a data object to the cache.
 */
void cache_insert (struct cache_header *C, char *uri, char *object, size_t object_size);

/*
 * Print the cache.
 */
void cache_print (struct cache_header *C);