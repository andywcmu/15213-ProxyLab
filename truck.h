/* interface */
struct cache_header *cache_init ();
char *cache_find (struct cache_header *C, char *uri);
void cache_insert (struct cache_header *C, char *uri, char *object);