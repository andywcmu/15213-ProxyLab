/* interface */
struct cache_header *init ();
char *cache_find (struct cache_header *C, char *uri);
void cache_insert (struct cache_header *C, char *uri, char *object);