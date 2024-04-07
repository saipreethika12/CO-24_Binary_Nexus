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
  int tag;
  // int offset;
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
  Set(int numBlocksPerSet)
  {
    blocks.resize(numBlocksPerSet);
    for (auto &block : blocks)
    {
      block.valid = false;
    }
    this->numBlocks = numBlocksPerSet;
  }
  void set_numBlocks(int numBlocks)
  {
    this->numBlocks = numBlocks;
  }

  bool search(int tag)
  {
    for (auto &block : blocks)
    {   std::cout<<"blockin"<<block.tag<<std::endl;
      if (block.tag == tag && block.valid == true)
      {
        return true; // Hit
      }
    }
     block_fetch_viaRandom( tag);
    return false; // Miss
  }

  void block_fetch_viaLRU(uint64_t tag)
  {
     int size = blocks.size();
    // id=f size till now is numblocksperset then rp
    if (ump.find(size) == ump.end())
    {
///correct
      if (size == numBlocks)
      {
        blocks[size].tag = tag;
        blocks[size].valid = true;
        uint64_t last = Priority_list.back();
        Priority_list.pop_back();
        ump.erase(last);
      }
    }
    else
    {
      // LRU
      blocks[size].valid=false;
      Priority_list.erase(ump[size]);
    }
    Priority_list.push_back(size);
    ump[size] = Priority_list.begin();
  }

  void block_fetch_viaRandom(int tag)
  {
    // srand((unsigned)time(NULL));
    int size = blocks.size();
     std::cout <<"size"<<size<< std::endl;
    std::cout << numBlocks << std::endl;
    int index = rand() % numBlocks;
     std::cout <<"indexrr"<<index<< std::endl;

    // if (size < numBlocks)
    // {
    //   blocks[size].tag = tag;
    //   blocks[size].valid = true;
    // }

    // else
    // {

     // blocks[index].valid = false;
      blocks[index].tag=tag;
      blocks[index].valid=true;
      
   // }
  }
};

class Cache_simulator
{
private:
  std::vector<Set> sets_cache;
  int cacheSize;
  int blockSize;
  int numSets;
  int associativity;
  int cache_latency;
  int memory_latency;

  std::pair<int, int> splitAddress(uint64_t address)
  {
    int num_bits_offset = std::log2(blockSize);
    std::cout << num_bits_offset << std::endl;
    address >>= num_bits_offset;
    int num_bits = std::log2(numSets);
    int index = address & ((1 << num_bits - 1));

    int tag = address >> static_cast<int>(std::log2(numSets));
    return std::make_pair(tag, index);
  }

public:
  Cache_simulator();
  Cache_simulator(int _cacheSize, int _blockSize, int associativity, int cache_latency, int memory_latency)
      : cacheSize(_cacheSize), blockSize(_blockSize), associativity(associativity), cache_latency(cache_latency), memory_latency(memory_latency)
  {

    numSets = cacheSize / (blockSize * associativity);
    sets_cache.resize(numSets, Set(numSets));
    for (auto set : sets_cache)
    {
      set.set_numBlocks(associativity);
    }
  }
  int get_mem_latency()
  {
    return memory_latency;
  }
  int get_cache_latency()
  {
    return cache_latency;
  }
  bool access(int address)
  {
    std::pair<int, int> address_parts = splitAddress(address);
    int tag = address_parts.first;
    int index = address_parts.second;
    if (sets_cache[index].search(tag))
    {  std::cout<<"im in"<<std::endl;
      return true; // Hit
    }
    else
    {
      std::cout << "add" << address << std::endl;
      std::cout << "tag"
                << " " << tag << std::endl;
      std::cout << "index"
                << " " << index << std::endl;
      std::cout << "ass"
                << " " << associativity << std::endl;
                //set_cachse[index].
      sets_cache[index].block_fetch_viaRandom(tag);

      return false; // Miss
    }
  }
};

#endif
