#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

const int LINE_LENGTH = 80;

void init_cache(cache_t *cache);
bool find_block_and_update_lru(cache_t *cache, unsigned address, bool write);
void fill_cache_with_block(cache_t *cache, unsigned address, bool write);

int
main(int argc, char *argv[]) {
  bool verbose = (argc == 3) && (strncmp(argv[2], "verbose", 8) == 0);

  // check to see if there are the right number of parameters
  if ((argc != 2) && !verbose) {
    printf("%s <trace_file> [verbose]\n", argv[0]);
    exit(0);
  }

  // open the trace file for reading
  FILE *trace_file = fopen(argv[1], "r");

  // instantiate the cache
  cache_t cache;
  init_cache(&cache);

  // parse the trace, performing the accesses on the cache
  char type;
  unsigned address;
  char line[LINE_LENGTH];
  while (!feof(trace_file)) { // while we haven't gotten to the end
    char *read_line = fgets(line, LINE_LENGTH, trace_file);
    if (read_line == NULL) {
      continue;    // if we hit the end of file, feof will be true
    }

    // parse the line of the trace file to extract the address
    sscanf(line, "%c %x", &type, &address);
	 bool write = (type == 'W');

    // check to see if that address hits in the cache, and if it
    // doesn't then bring that line into the cache.
    bool hit = find_block_and_update_lru(&cache, address, write);
    if (!hit) {
      fill_cache_with_block(&cache, address, write);
    }

    // print verbose output if requested.
    if (verbose) {
      printf("%s %x\n", hit ? "HIT " : "MISS", address);
    }
  }

  // output final statistics
  printf("Reached end of file\n");
  printf("hits: %d, accesses: %d, hit rate: %.2f, writebacks: %d\n", 
			cache.hits, cache.accesses, (100.0 * cache.hits)/cache.accesses, cache.writebacks);
}
