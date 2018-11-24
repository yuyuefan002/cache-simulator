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
             int & replaceAlg);

void printResult(std::vector<int> res1, std::vector<int> res2);
#endif
