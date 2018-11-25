/*************************************************

Description: the cache simulator, support FA DM SA cache, 
note that FA cache is just a (C/B) way SA cache, 
DM cacheis just a 1-way SA cache.Therefore, 
they are interchangable. you can configure operation mode, 
replacement algorithm, A B C size, hitTime and missTime. 
In the write process, I use write back type and create a store buffer. 
Everything you want to write go through write buffer. 
I also count the tavg, miss rate which is for analysis.

Author:Yuefan Yu

Date:11/24/2018
**************************************************/

#include "cache.h"

// whether hit in store buffer or not
bool Cache::hitinStoreBuf(std::string address) {
  for (auto pair : storeBuf) {
    // if hit
    if (pair.second == address) {
      return true;
    }
  }
  return false;
}

// pair the data in store buffer to cache
void Cache::sb2Cache() {
  for (size_t i = 0; i < storeBuf.size(); i++) {
    if (hitInCache(storeBuf[i].first.second, storeBuf[i].first.first, 1)) {
      storeBuf.erase(storeBuf.begin() + i);
    }
  }
}

// whether hit in cache or not
bool Cache::hitInCache(std::string tag, int setid, int write = 0) {
  for (size_t i = 0; i < cache[setid].size(); i++) {
    if (cache[setid][i].first == 1 && cache[setid][i].second == tag) {
      // recently used way, so have to put the end of LRU
      updLru(setid, i);
      if (write) {
        dirty[setid][i] = true;
      }
      return true;
    }
  }
  return false;
}

//update LRU list
void Cache::updLru(int setid, int pos) {
  int temp = lru[setid][pos];
  //move the current way
  lru[setid].erase(lru[setid].begin() + pos);
  //put in the back of list
  lru[setid].push_back(temp);
}

// delete strategy in cache
size_t Cache::put_in_which_way(int setid) {
  if (replaceAlg == LRU) {
    // the first in the list is the least recently used one
    int res = lru[setid][0];
    updLru(setid, 0);
    return res;
  }
  return rand() % A;
}

// update cache
void Cache::updCache(std::string tag, int setid, std::string address) {
  size_t pos = put_in_which_way(setid);
  cache[setid][pos].first = 1;
  cache[setid][pos].second = tag;
  //if block is dirty write back to cache_l2
  if (l2exist && dirty[setid][pos] == true) {
    dirty[setid][pos] = false;
    l2cache->operation("1", l2parser->getTag(), l2parser->b2D(l2parser->getSetid()), address);
  }
}

// read data from cache
void Cache::get(std::string tag, int setid, std::string address) {
  // check whether in the store buffer
  std::string temp = tag + std::to_string(setid);
  if (hitInCache(tag, setid) == true) {
    totalHit++;
  }
  else {
    // if miss in the cache
    if (l2exist) {
      //read from l2 cache
      //cache_l2.get(tag, setid, address);
      l2parser->setAddress(address);
      l2cache->operation("0", l2parser->getTag(), l2parser->b2D(l2parser->getSetid()), address);
    }
    updCache(tag, setid, address);
    // transfer data from store buffer to cache
    sb2Cache();
    if (seenBefore2.count(temp)) {
      conflict_rd++;
    }
    else {
      compulsory_rd++;
    }
  }
  seenBefore2.insert(temp);
  totalAccess++;
}

//read instuction from cache
void Cache::getInsn(std::string tag, int setid, std::string address) {
  std::string temp = tag + std::to_string(setid);
  // check whether in the store buffer
  if (hitinStoreBuf(address)) {
    totalInsnHit_sb++;
  }
  else if (hitInCache(tag, setid) == true) {
    totalInsnHit++;
  }
  else {
    // if miss in the cache
    if (l2exist) {
      //read from l2 cache
      l2parser->setAddress(address);
      l2cache->operation("2", l2parser->getTag(), l2parser->b2D(l2parser->getSetid()), address);
      //if hit in l2 cache
      //cache_l2.get(tag, setid, address);
    }
    updCache(tag, setid, address);
    // pass data from store buffer to cache
    sb2Cache();
    if (seenBefore2.count(temp)) {
      conflict_in++;
    }
    else {
      compulsory_in++;
    }
  }
  // for calculating compulsory miss
  seenBefore2.insert(temp);
  totalInsnAccess++;
}

// write data into cache
void Cache::put(std::string tag, int setid, std::string address) {
  std::string temp = tag + std::to_string(setid);
  if (hitInCache(tag, setid, 1)) {
    totalWrHit++;
  }
  else {  //put into store buffer.
    storeBuf.push_back({{setid, tag}, address});

    if (seenBefore2.count(temp)) {
      conflict_wr++;
    }
    else {
      compulsory_wr++;
    }
    if (allocOnWrMiss == true) {
      updCache(tag, setid, address);
      seenBefore2.insert(temp);
    }
  }

  totalWrAccess++;
}
void Cache::operation(std::string cmdType, std::string tag, int setid, std::string address) {
  if (cmdType == "0" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    get(tag, setid, address);
  else if (cmdType == "1" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    put(tag, setid, address);
  else if (cmdType == "2" && (mode == UNI_MEM || mode == I_MEM_ONLY))
    getInsn(tag, setid, address);
}

std::vector<int> Cache::getHitRate() {
  std::vector<int> res(15, 0);
  if (mode == UNI_MEM || mode == D_MEM_ONLY) {
    res[2] = totalWrAccess;
    res[1] = totalAccess;
    res[3] = totalAccess - totalHit - totalHit_sb;
    res[4] = totalWrAccess - totalWrHit;
    res[6] = compulsory_rd;
    res[8] = compulsory_wr;
    res[9] = conflict_rd;
    res[10] = conflict_wr;
  }
  if (mode == UNI_MEM || mode == I_MEM_ONLY) {
    res[0] = totalInsnAccess;
    res[5] = totalInsnAccess - totalInsnHit - totalInsnHit_sb;
    res[7] = compulsory_in;
    res[11] = conflict_in;
  }
  return res;
}

Cache::Cache(int h,
             int d,
             int associativity,
             int blockSize,
             int capacity,
             int m,
             bool alloc,
             int ra,
             bool l2,
             Cache * l2c,
             Parser * l2p) :
    A(associativity),
    hitTime(h),
    DRAMAccessTime(d),
    totalHit(0),
    totalHit_sb(0),
    totalAccess(0),
    conflict_wr(0),
    conflict_in(0),
    conflict_rd(0),
    compulsory_wr(0),
    compulsory_in(0),
    compulsory_rd(0),
    totalInsnHit(0),
    totalInsnHit_sb(0),
    totalInsnAccess(0),
    totalWrHit(0),
    totalWrAccess(0),
    mode(m),
    replaceAlg(ra),
    allocOnWrMiss(alloc),
    l2exist(l2),
    l2cache(l2c),
    l2parser(l2p) {
  setSize = capacity / (blockSize * associativity);
  cache.resize(setSize);
  dirty.resize(setSize);
  lru.resize(setSize);
  for (size_t i = 0; i < cache.size(); i++) {
    cache[i].resize(associativity);
    dirty[i].resize(associativity, false);
    for (int j = 0; j < associativity; j++) {
      lru[i].push_back(j);
    }
  }
}
