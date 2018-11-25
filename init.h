#ifndef __INIT_H__
#define __INIT_H__

#include "cache.h"
#include "config.h"
class Sys
{
 private:
  int h_l1;
  int h_l2;
  int DAT;
  bool l2;

 public:
  Sys();
  void sysInit(int & associativity,
               int & blockSize,
               int & capacity,
               int & hitTime,
               int & DRAMAccessTime,
               int * mode,
               bool & allocOnWrMiss,
               int & replaceAlg,
               int & A_l2,
               int & B_l2,
               int & C_l2,
               int & h2,
               int * mode2,
               bool & all2,
               int & rA2);
  void printResult(std::vector<int> res1, std::vector<int> res2);
  void printRunInfo(std::vector<int> res1,
                    std::vector<int> res2,
                    std::vector<int> res3,
                    std::vector<int> res4);
  void l2modeConf(bool & l2, Parser * l2p, Cache * l2cforc1, Cache * l2cforc2);
};
#endif
