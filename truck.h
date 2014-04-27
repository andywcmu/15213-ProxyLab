/* interface */
struct cache_header *init ();
char *cache_find (char *uri);
void cache_insert (char *uri, char *object);