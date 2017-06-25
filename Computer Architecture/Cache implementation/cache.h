#define BLOCK_SIZE 512   // FIXME this is in bits, not sure if they want 2 or naw
#define NUM_SETS   256   // FIXME

typedef struct cache_block {
  unsigned tag;
  bool valid;
  bool dirty;
} cache_block_t;

typedef struct cache_set {
  cache_block_t blocks[2];
  unsigned lru;
} cache_set_t;
  
typedef struct cache {
  cache_set_t sets[NUM_SETS];     // the sets themsleves

  // statistics
  unsigned accesses;
  unsigned hits;
  unsigned writebacks;
} cache_t;


