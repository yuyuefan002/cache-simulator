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
             bool & allocOnWrMiss);

#endif
