#include "cache.h"

Cache::Cache(int h, int d, int associativity, int blockSize, int capacity, int m, bool alloc) :
    A(associativity),
    hitTime(h),
    DRAMAccessTime(d),
    totalHit(0),
    totalAccess(0),
    totalInsnHit(0),
    totalInsnAccess(0),
    totalWrHit(0),
    totalWrAccess(0),
    mode(m),
    allocOnWrMiss(alloc) {
  setSize = capacity / (blockSize * associativity);
  cache.resize(setSize);
  for (size_t i = 0; i < cache.size(); i++) {
    cache[i].resize(associativity);
  }
}
bool Cache::hitinStoreBuf(std::string address) {
  for (auto pair : storeBuf) {
    if (pair.second == address) {
      return true;
    }
  }
  return false;
}
void Cache::sb2Cache() {
  for (size_t i = 0; i < storeBuf.size(); i++) {
    if (hitInCache(storeBuf[i].first.second, storeBuf[i].first.first)) {
      totalWrHit++;
      storeBuf.erase(storeBuf.begin() + i);
    }
    if (allocOnWrMiss == true) {
      updCache(storeBuf[i].first.second, storeBuf[i].first.first);
    }
    totalWrAccess++;
  }
}
bool Cache::hitInCache(std::string tag, int setid) {
  for (size_t i = 0; i < cache[setid].size(); i++) {
    if (cache[setid][i].first == 1 && cache[setid][i].second == tag) {
      return true;
    }
  }
  return false;
}
size_t Cache::put_in_which_way() {
  return rand() % A;
}
void Cache::updCache(std::string tag, int setid) {
  size_t pos = put_in_which_way();
  cache[setid][pos].first = 1;
  cache[setid][pos].second = tag;
}
void Cache::get(std::string tag, int setid, std::string address) {
  if (hitInCache(tag, setid) == true) {
    totalHit++;
  }
  else {
    // if miss in the cache
    updCache(tag, setid);
    // check whether in the store buffer
    if (hitinStoreBuf(address)) {
    }
    // transfer data from store buffer to cache
    sb2Cache();
  }
  totalAccess++;
}
void Cache::getInsn(std::string tag, int setid, std::string address) {
  if (hitInCache(tag, setid) == true) {
    totalInsnHit++;
  }
  else {
    // if miss in the cache
    updCache(tag, setid);
    // check whether in the store buffer
    if (hitinStoreBuf(address)) {
    }
    // pass data from store buffer to cache
    sb2Cache();
  }
  totalInsnAccess++;
}
void Cache::put(std::string tag, int setid, std::string address) {
  //put into store buffer.
  storeBuf.push_back({{setid, tag}, address});
  // check hit or not
  sb2Cache();
}
void Cache::getHitRate() {
  if (mode == UNI_MEM || mode == D_MEM_ONLY) {
    fprintf(stdout, "Read Hit Rate is %.2f%%\n", (1.0 * totalHit / totalAccess) * 100);

    fprintf(stdout, "Write Hit Rate is %.2f%%\n", (1.0 * totalWrHit / totalWrAccess) * 100);
  }
  if (mode == UNI_MEM || mode == I_MEM_ONLY)
    fprintf(stdout, "Fetch Hit Rate is %.2f%%\n", (1.0 * totalInsnHit / totalInsnAccess) * 100);
}

void Cache::operation(std::string cmdType, std::string tag, int setid, std::string address) {
  if (cmdType == "0" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    get(tag, setid, address);
  else if (cmdType == "1" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    put(tag, setid, address);
  else if (cmdType == "2" && (mode == UNI_MEM || mode == I_MEM_ONLY))
    getInsn(tag, setid, address);
}
