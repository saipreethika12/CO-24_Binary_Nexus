#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H
#include<iostream>
#include<vector>
#include<cmath>
struct CacheBlock {
  uint64_t tag;
  // ideally I should have allocated 64bytes of memory data[]
  bool valid;
};
class Cache_simulator{
 private:
    std::vector<CacheBlock> cache;
    unsigned int cacheSize;
    unsigned int blockSize;
    unsigned int numBlocks;

    std::pair<uint64_t, uint64_t> splitAddress(uint64_t address) 
    {
      // variable address if for byte
      int num_bits_offset = std::log2(blockSize);
      address = address >> num_bits_offset;
      uint64_t index = address & ((1 << numBlocks) - 1);
      uint64_t tag = address >> static_cast<uint64_t>(std::log2(numBlocks));
      return std::make_pair(tag, index);
    }

  public:
    Cache_simulator(unsigned int _cacheSize, unsigned int _blockSize)
      : cacheSize(_cacheSize), blockSize(_blockSize) {
        
       // assert(cacheSize % blockSize == 0);
        numBlocks = cacheSize / blockSize;
        cache.resize(numBlocks);
        for (auto b : cache) {
          b.valid = false;
        }
      }

    bool access (uint64_t address) {
      // split the address: |---------tag --------------| --index---| --offset---|
      auto a = splitAddress(address);
      uint64_t index = a.second;
      uint64_t tag = a.first;

      if (cache[index].tag == tag && cache[index].valid == true) {
        return true;
      }
      else {
        cache[index].tag = tag;
        cache[index].valid = true;
        return false;
      }
    }

};

#endif
