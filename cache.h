#ifndef __CACHE_H__
#define __CACHE_H__

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#define I_MEM_ONLY -2
#define D_MEM_ONLY -3
#define UNI_MEM -4
#define None 0
#define LRU -5
#define RND -6
#include <iostream>

#include "parser.h"
class Cache
{
 private:
  int A;
  int hitTime;
  int DRAMAccessTime;
  int setSize;
  int totalHit;
  int totalHit_sb;
  int totalAccess;
  int conflict_wr;
  int conflict_in;
  int conflict_rd;
  int compulsory_wr;
  int compulsory_in;
  int compulsory_rd;
  int totalInsnHit;
  int totalInsnHit_sb;
  int totalInsnAccess;
  int totalWrHit;
  int totalWrAccess;
  int mode;
  int replaceAlg;
  bool allocOnWrMiss;
  bool l2exist;
  Cache * l2cache;
  Parser * l2parser;
  std::unordered_set<std::string> seenBefore2;
  std::vector<std::vector<std::pair<int, std::string> > > cache;
  std::vector<std::vector<bool> > dirty;
  std::vector<std::vector<int> > lru;
  void get(std::string tag, int setid, std::string address);
  void put(std::string tag, int setid, std::string address);
  void getInsn(std::string tag, int setid, std::string address);
  bool hitInCache(std::string tag, int setid, int write);
  void updCache(std::string tag, int setid, std::string address);
  size_t put_in_which_way(int setid);
  void updLru(int setid, int pos);
  void collectMiss(std::string temp, int & conflict, int & compulsory);
  void l2operation(std::string cmdType, std::string address);

 public:
  Cache(int h,
        int d,
        int a,
        int b,
        int c,
        int m,
        bool alloc,
        int ra,
        bool l2,
        Cache * l2c,
        Parser * l2p);
  void endOperation();
  std::vector<int> getHitRate();
  void operation(std::string cmdType, std::string tag, int setid, std::string address);
};
#endif
