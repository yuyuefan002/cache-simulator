#ifndef __CACHE_H__
#define __CACHE_H__

#include <string>
#include <utility>
#include <vector>
#define I_MEM_ONLY -2
#define D_MEM_ONLY -3
#define UNI_MEM -4
#define None 0
#include "parser.h"
class Cache
{
 private:
  int A;
  int hitTime;
  int DRAMAccessTime;
  int setSize;
  int totalHit;
  int totalAccess;
  int totalInsnHit;
  int totalInsnAccess;
  int totalWrHit;
  int totalWrAccess;
  int mode;
  bool allocOnWrMiss;
  std::vector<std::vector<std::pair<int, std::string> > > cache;
  std::vector<std::pair<std::pair<int, std::string>, std::string> > storeBuf;
  bool hitinStoreBuf(std::string address);
  void sb2Cache();
  void get(std::string tag, int setid, std::string address);
  void put(std::string tag, int setid, std::string address);
  void getInsn(std::string tag, int setid, std::string address);
  bool hitInCache(std::string tag, int setid);
  void updCache(std::string tag, int setid);
  size_t put_in_which_way();

 public:
  Cache(int h, int d, int a, int b, int c, int m, bool alloc);

  void getHitRate();
  void operation(std::string cmdType, std::string tag, int setid, std::string address);
};
#endif
