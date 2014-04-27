/* interface */
/*
 * Init a cache object.
 */
struct cache_header *cache_init ();

/*
 * Return a pointer to the data object.
 */
char *cache_find (struct cache_header *C, char *uri);

/*
 * Add a data object to the cache.
 */
void cache_insert (struct cache_header *C, char *uri, char *object);