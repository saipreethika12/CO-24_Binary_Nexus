#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <unordered_map>
#include <algorithm>

struct CacheBlock
{
  uint64_t tag;
  bool valid;
  bool dirty;
};

class Set
{
private:
  std::vector<CacheBlock> blocks;
  int numBlocks;

  std::list<uint64_t> Priority_list;
  // store references of key in cache
  std::unordered_map<uint64_t, std::list<uint64_t>::iterator> ump;
  // int  associativity;

public:
  Set(unsigned int numBlocksPerSet)
  {
    blocks.resize(numBlocksPerSet);
    for (auto &block : blocks)
    {
      block.valid = false;
    }
  }

  bool search(uint64_t tag)
  {
    for (auto &block : blocks)
    {
      if (block.tag == tag && block.valid == true)
      {
        return true; // Hit
      }
    }
    return false; // Miss
  }

  void block_fetch_viaLRU(uint64_t tag)
  {

    // id=f size till now is numblocksperset then rp
    if (ump.find(tag) == ump.end())
    {
      int size = blocks.size();
      if (size < numBlocks - 1)
      {
        blocks[size + 1].tag = tag;
        blocks[size + 1].valid = true;
        uint64_t last = Priority_list.back();
        Priority_list.pop_back();
        ump.erase(last);
      }
    }
    else
    {
      // LRU
      Priority_list.erase(ump[tag]);
    }
    Priority_list.push_back(tag);
    ump[tag] = Priority_list.begin();
  }
  void block_fetch_viaRandom(uint64_t tag){
     srand((unsigned)time(NULL));
     int index = rand() % blocks.size();
    if (ump.find(tag) == ump.end())
    {
      int size = blocks.size();
      if (size < numBlocks - 1)
      {
        blocks[size + 1].tag = tag;
        blocks[size + 1].valid = true;
       
      }
    }
    else
    {
      // LRU
      Priority_list.erase(ump[tag]);
    }
    Priority_list.push_back(tag);
    ump[tag] = Priority_list.begin();
  
 
  }
};

class Cache_simulator
{
private:
  std::vector<Set> sets_cache;
  unsigned int cacheSize;
  unsigned int blockSize;
  unsigned int numSets;
  unsigned int associativity;

  std::pair<uint64_t, uint64_t> splitAddress(uint64_t address)
  {
    int num_bits_offset = std::log2(blockSize);
    address >>= num_bits_offset;
    uint64_t index = address & ((1 << numSets) - 1);
    uint64_t tag = address >> static_cast<uint64_t>(std::log2(numSets));
    return std::make_pair(tag, index);
  }

public:
  Cache_simulator(unsigned int _cacheSize, unsigned int _blockSize, unsigned int associativity)
      : cacheSize(_cacheSize), blockSize(_blockSize), associativity(associativity)
  {

    numSets = cacheSize / (blockSize * associativity);
    sets_cache.resize(numSets, Set(associativity));
  }

  bool access(uint64_t address)
  {
    std::pair<uint64_t, uint64_t> address_parts = splitAddress(address);
    uint64_t tag = address_parts.first;
    uint64_t index = address_parts.second;
    if (sets_cache[index].search(tag))
    {
      return true; // Hit
    }
    else
    {
      sets_cache[index].block_fetch_viaLRU(tag);
      return false; // Miss
    }
  }
};

#endif
