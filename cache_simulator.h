#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <unordered_map>
#include <algorithm>
/// @brief 
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
    {
      
      if (block.tag == tag && block.valid == true)
      {
        return true; // Hit
      }
    }
    //block_fetch_viaRandom(tag);
    return false; // Miss
  }

  void block_fetch_viaLRU(uint64_t tag)
  {
    int size = 0;
    for (auto &block : blocks)
    {
      if (block.tag != 0 && block.valid == true)
      {
        size++;
      }
    }
    int index = rand() % numBlocks;
    if (ump.find(size) == ump.end())
    {
     
      if (size < numBlocks)
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
      blocks[size].valid = false;
      Priority_list.erase(ump[size]);
    }
    Priority_list.push_back(size);
    ump[size] = Priority_list.begin();
  }

  void block_fetch_viaRandom(int tag)
  {
    int size = 0;
    for (auto &block : blocks)
    {
      if (block.tag != 0 && block.valid == true)
      {
        size++;
      }
    }
    std::cout << "size" << size << std::endl;
    std::cout << numBlocks << std::endl;
    int index = rand() % numBlocks;
    std::cout << "indexrr" << index << std::endl;

    if (size < numBlocks)
    {
      blocks[size].tag = tag;
      blocks[size].valid = true;
    }

    else
    {

      blocks[index].tag = tag;
      blocks[index].valid = true;
    }
  }
};

class Cache_simulator
{
private:
  std::vector<Set> L1_cache;
  std::vector<Set> L2_cache;
  int cacheSize;
  int blockSize;
  int numSets;
  int associativity;
  int l1_cache_latency;
  int l2_cache_latency;
  int memory_latency;
  int cache_policy_num;

  std::pair<int, int> splitAddress_L1(uint64_t address)
  {
    int num_bits_offset = std::log2(blockSize);
    std::cout << num_bits_offset << std::endl;
    address >>= num_bits_offset;
    int num_bits = std::log2(numSets);
    int index = address & ((1 << num_bits - 1));

    int tag = address >> static_cast<int>(std::log2(numSets));
    return std::make_pair(tag, index);
  }
    std::pair<int, int> splitAddress_L2(uint64_t address)
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
  Cache_simulator(int _cacheSize, int _blockSize, int associativity, int cache_latency,int l2_cache_latency, int memory_latency,int policy_num)
      : cacheSize(_cacheSize), blockSize(_blockSize), associativity(associativity), l1_cache_latency(cache_latency),l2_cache_latency(l2_cache_latency), memory_latency(memory_latency),cache_policy_num(policy_num)
  {

    numSets = cacheSize / (blockSize * associativity);
    L1_cache.resize(numSets, Set(numSets));
    for (auto set : L1_cache)
    {
      set.set_numBlocks(associativity);
    }
  }
  int get_mem_latency()
  {
    return memory_latency;
  }
  int get_l1_cache_latency()
  {
    return l1_cache_latency;
  }
  int get_l2_cache_latency()
  {
    return l2_cache_latency;
  }
  
  std::pair<bool,int> access(int address)
  {
    std::pair<int, int> address_partsl1  = splitAddress_L1(address);
    int tag = address_partsl1.first;
    int index = address_partsl1.second;
    std::pair<int, int> address_partsl2  = splitAddress_L2(address);
    int tagl2 = address_partsl2.first;
    int indexl2 = address_partsl2.second;
    if (L1_cache[index].search(tag))
    {
      
      return {true,1};
    }
    else if(L2_cache[indexl2].search(tagl2)){
          return {true,2};
    }
    else
    {
   
      if(cache_policy_num==1){
      L1_cache[index].block_fetch_viaLRU(tag);
      L2_cache[indexl2].block_fetch_viaLRU(tagl2);
      }else{
        L1_cache[index].block_fetch_viaRandom(tag);
        L2_cache[indexl2].block_fetch_viaRandom(tagl2);
      }

      return {false,0}; 
    }
  }
};

#endif