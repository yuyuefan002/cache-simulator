#include "cache.h"

Cache::Cache(int h,
             int d,
             int associativity,
             int blockSize,
             int capacity,
             int m,
             bool alloc,
             int ra) :
    A(associativity),
    hitTime(h),
    DRAMAccessTime(d),
    totalHit(0),
    totalHit_sb(0),
    totalAccess(0),

    conflict_in(0),
    conflict_rd(0),
    compulsory_in(0),
    compulsory_rd(0),
    totalInsnHit(0),
    totalInsnHit_sb(0),
    totalInsnAccess(0),
    totalWrHit(0),
    totalWrAccess(0),
    mode(m),
    replaceAlg(ra),
    allocOnWrMiss(alloc) {
  setSize = capacity / (blockSize * associativity);
  cache.resize(setSize);
  lru.resize(setSize);
  for (size_t i = 0; i < cache.size(); i++) {
    cache[i].resize(associativity);
    for (int j = 0; j < associativity; j++) {
      lru[i].push_back(j);
    }
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
      storeBuf.erase(storeBuf.begin() + i);
    }
  }
}
bool Cache::hitInCache(std::string tag, int setid) {
  for (size_t i = 0; i < cache[setid].size(); i++) {
    if (cache[setid][i].first == 1 && cache[setid][i].second == tag) {
      // recently used way, so have to put the end of LRU
      updLru(setid, i);
      return true;
    }
  }
  return false;
}
void Cache::updLru(int setid, int pos) {
  int temp = lru[setid][pos];
  lru[setid].erase(lru[setid].begin() + pos);
  lru[setid].push_back(temp);
}
size_t Cache::put_in_which_way(int setid) {
  if (replaceAlg == LRU) {
    int res = lru[setid][0];
    updLru(setid, 0);
    return res;
  }
  return rand() % A;
}
void Cache::updCache(std::string tag, int setid) {
  size_t pos = put_in_which_way(setid);
  cache[setid][pos].first = 1;
  cache[setid][pos].second = tag;
}
void Cache::get(std::string tag, int setid, std::string address) {
  // check whether in the store buffer
  if (hitinStoreBuf(address)) {
    totalHit_sb++;
  }
  else if (hitInCache(tag, setid) == true) {
    totalHit++;
  }
  else {
    // if miss in the cache
    updCache(tag, setid);
    // transfer data from store buffer to cache
    sb2Cache();
    if (seenBefore.count(address)) {
      conflict_rd++;
    }
    else {
      compulsory_rd++;
    }
  }
  seenBefore.insert(address);
  totalAccess++;
}
void Cache::getInsn(std::string tag, int setid, std::string address) {
  // check whether in the store buffer
  if (hitinStoreBuf(address)) {
    totalInsnHit_sb++;
  }
  else if (hitInCache(tag, setid) == true) {
    totalInsnHit++;
  }
  else {
    // if miss in the cache
    updCache(tag, setid);
    // pass data from store buffer to cache
    sb2Cache();
    if (seenBefore.count(address)) {
      conflict_in++;
    }
    else {
      compulsory_in++;
    }
  }
  seenBefore.insert(address);
  totalInsnAccess++;
}
void Cache::put(std::string tag, int setid, std::string address) {
  if (hitInCache(tag, setid)) {
    totalWrHit++;
  }
  else {  //put into store buffer.
    storeBuf.push_back({{setid, tag}, address});
    if (allocOnWrMiss == true) {
      updCache(tag, setid);
    }
  }
  totalWrAccess++;
}
void Cache::getHitRate() {
  if (mode == UNI_MEM || mode == D_MEM_ONLY) {
    fprintf(
        stdout, "Read Hit Rate is %.2f%%\n", (1.0 * (totalHit + totalHit_sb) / totalAccess) * 100);
    fprintf(stdout,
            "Average Read Hit Time is %.2f cycle\n",
            hitTime + (1 - 1.0 * (totalHit + totalHit_sb) / totalAccess) * DRAMAccessTime);
    fprintf(stdout, "Write Hit Rate is %.2f%%\n", (1.0 * totalWrHit / totalWrAccess) * 100);
    fprintf(stdout, "total read miss is: %d\n", totalAccess - totalHit - totalHit_sb);
    fprintf(stdout,
            "Read compulsory miss is: %d, rate is: %.2f%%\n",
            compulsory_rd,
            1.0 * compulsory_rd / (totalAccess - totalHit - totalHit_sb) * 100);
    fprintf(stdout,
            "Read capacity+conflict miss is: %d, rate is: %.2f%%\n\n",
            conflict_rd,
            1.0 * conflict_rd / (totalAccess - totalHit - totalHit_sb) * 100);
  }
  if (mode == UNI_MEM || mode == I_MEM_ONLY) {
    fprintf(stdout,
            "Fetch Hit Rate is %.2f%%\n",
            (1.0 * (totalInsnHit + totalInsnHit_sb) / totalInsnAccess) * 100);
    fprintf(
        stdout,
        "Average Fetch Hit Time is %.2f cycle\n",
        hitTime + (1 - 1.0 * (totalInsnHit + totalInsnHit_sb) / totalInsnAccess) * DRAMAccessTime);
    fprintf(stdout, "total fetch miss is: %d\n", totalInsnAccess - totalInsnHit - totalInsnHit_sb);
    fprintf(stdout,
            "Fetch compulsory miss is: %d, rate is: %.2f%%\n",
            compulsory_in,
            1.0 * compulsory_in / (totalInsnAccess - totalInsnHit - totalInsnHit_sb) * 100);
    fprintf(stdout,
            "Fetch capacity+conflict miss is: %d, rate is: %.2f%%\n",
            conflict_in,
            1.0 * conflict_in / (totalInsnAccess - totalInsnHit - totalInsnHit_sb) * 100);
  }
}

void Cache::operation(std::string cmdType, std::string tag, int setid, std::string address) {
  if (cmdType == "0" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    get(tag, setid, address);
  else if (cmdType == "1" && (mode == UNI_MEM || mode == D_MEM_ONLY))
    put(tag, setid, address);
  else if (cmdType == "2" && (mode == UNI_MEM || mode == I_MEM_ONLY))
    getInsn(tag, setid, address);
}
