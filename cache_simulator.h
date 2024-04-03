#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct CacheBlock
{
  uint64_t tag;
  bool valid;
};

class Set
{
private:
  std::vector<CacheBlock> blocks;

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
      if (block.tag == tag && block.valid)
      {
        return true; // Hit
      }
    }
    return false; // Miss
  }

  void insert(uint64_t tag)
  {
    // Replace the least recently used block
    auto block = std::find_if(blocks.begin(), blocks.end(), [](const CacheBlock &b)
                              { return !b.valid; });
    if (block != blocks.end())
    {
      block->tag = tag;
      block->valid = true;
    }
    else
    {

      blocks[0].tag = tag;
      blocks[0].valid = true;
    }
  }
};

class Cache_simulator
{
private:
  std::vector<Set> cache;
  unsigned int cacheSize;
  unsigned int blockSize;
  unsigned int numSets;
  unsigned int numBlocksPerSet;

  std::pair<uint64_t, uint64_t> splitAddress(uint64_t address)
  {
    int num_bits_offset = std::log2(blockSize);
    address >>= num_bits_offset;
    uint64_t index = address & ((1 << numSets) - 1);
    uint64_t tag = address >> static_cast<uint64_t>(std::log2(numSets));
    return std::make_pair(tag, index);
  }

public:
  Cache_simulator(unsigned int _cacheSize, unsigned int _blockSize, unsigned int _numBlocksPerSet)
      : cacheSize(_cacheSize), blockSize(_blockSize), numBlocksPerSet(_numBlocksPerSet)
  {

    numSets = cacheSize / (blockSize * numBlocksPerSet);
    cache.resize(numSets, Set(numBlocksPerSet));
  }

  bool access(uint64_t address)
  {
    std::pair<uint64_t, uint64_t> address_parts = splitAddress(address);
    uint64_t tag = address_parts.first;
    uint64_t index = address_parts.second;
    if (cache[index].search(tag))
    {
      return true; // Hit
    }
    else
    {
      cache[index].insert(tag);
      return false; // Miss
    }
  }
};

#endif
