#ifndef __INIT_H__
#define __INIT_H__

#include "cache.h"
#include "config.h"
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
#endif
