#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

// initialize the cache by allocating space for all of the cache
// blocks and initializing them to be invalid and with 0 last access
// times.
void init_cache(cache_t *cache) {
  // initialize the cache blocks to invalid
  for (int i = 0 ; i < NUM_SETS ; i ++) {
	 for (int j = 0 ; j < 2 ; j ++) {
		cache->sets[i].blocks[j].valid = false;
		cache->sets[i].blocks[j].dirty = false;
	 }
	 cache->sets[i].lru = 0;
  }

  cache->accesses = 0;
  cache->hits = 0;
  cache->writebacks = 0;
}


// given an address, extract the tag field 
//if an address has m bits, the tag is the most significant m-s-n bits
//64 byte blocks, so n = 6
//32KB total size, 128 byte blocks, s is total sets, s = log2(32KB/128) ~ 8
//m = 32
unsigned address_to_tag(unsigned address) {
    //tag is m-s-n msbs
    //m-s-n = 32-8-6 = 18 bits
    
    //to get tag, we have to and lsbs with 14 0s, then shift it over 14 bits
    unsigned tag = address & 0xffffc000;
    tag = tag >> 14;
    return tag;
}

// given an address, extract the index field
//address is most siginificant s bits occuring after tag
//s = 8
//n = 6, so s starts at bit 6 and goes to bit 13
unsigned address_to_index(unsigned address) {
  
  //8 1s from 6 to 13 with trailing 6 0s
    unsigned index = address & 0x3fc0;
    index = index >> 6;
    return index;
}

// Given an address, look up in cache "cache" to see if that
// address hits.  If it does update the LRU.
// Include necessary updates to the access, hit, writeback counters
bool find_block_and_update_lru(cache_t *cache, unsigned address, bool write) {
    unsigned tag = address_to_tag(address);
    unsigned index = address_to_index(address);
    printf("%i \n", tag);
    printf("%i \n", index);

    
    // FIXME
    //we access the cache
    cache->accesses++;
    
    //find the index that the address would be mapped to
    cache_set_t set = cache->sets[index];
    
    //first we need to check if the address exists in the cache
    //then if it does we will update lru if necessary and return true
    //otherwise we'll have to write the lru block with the new address
    
    //if it is a read, then the data shouldn't be dirty, if it's a write, we need to make the data dirty
    
    
    //check if it exists
    if(tag == set.blocks[0].tag){
      //update lru and hit counts
      printf("gets into true part");
      set.lru = 1;
      cache->hits++;
      
      if(write){
        set.blocks[0].dirty = 1;
        set.blocks[0].valid = 0;
        cache->writebacks++;
      }
      else{
        set.blocks[0].dirty = 0;
        set.blocks[0].valid = 1;
      }
      
      //update actual cache
      cache->sets[index] = set;
      return true;
    }
    else if(tag == set.blocks[1].tag){
      printf("gets into true part");
      set.lru = 0;
      cache->hits++;
      
      if(write){
        set.blocks[1].dirty = 1;
        set.blocks[1].valid = 0;
        cache->writebacks++;
      }
      else{
        set.blocks[1].dirty = 0;
        set.blocks[1].valid = 1;
      }
      
      //update actual cache
      cache->sets[index] = set;
      return true;
    }

    return false;  // FIXME: return true if we found the block and false if not
}

// This function should find the LRU block of the correct set and replace it 
// with one that contains "address," updating the LRU appropriately.
// Include necessary updates to the access, hit, writeback counters
void fill_cache_with_block(cache_t *cache, unsigned address, bool write) {
  unsigned tag = address_to_tag(address);
  unsigned index = address_to_index(address);

  // FIXME
  //get set based off index
  cache_set_t set = cache->sets[index];
  
  //write to lru
  cache_block_t block = set.blocks[set.lru];
  block.tag = tag;
  
  if(write){
    block.dirty = 1;
    block.valid = 0;
    cache->writebacks++;
    
    //need these last two lines because we duplicated objects
    set.blocks[set.lru] = block;
    set.lru ^= 1;
  }
  else{
    block.dirty = 0;
    block.valid = 1;
    
    //need these last two lines because we duplicated objects
    set.blocks[set.lru] = block;
    set.lru ^= 1;
  }
  
  //fix actual cache object we are working with
  cache->sets[index] = set;
}
