#include "init.h"

void sysInit(int & associativity,
             int & blockSize,
             int & capacity,
             int & hitTime,
             int & DRAMAccessTime,
             int * mode,
             bool & allocOnWrMiss) {
  Config config("cache.config");
  associativity = config.getAssociativity();
  blockSize = config.getBlockSize();
  capacity = config.getCapacity();
  hitTime = config.getHitTime();
  DRAMAccessTime = config.getDRAMAccessTime();
  if (config.getmodel1() == SPLIT) {
    mode[0] = D_MEM_ONLY;
    mode[1] = I_MEM_ONLY;
  }
  else if (config.getmodel1() == UNIFIED) {
    mode[0] = UNI_MEM;
    mode[1] = None;
  }
  allocOnWrMiss = config.getAllocOnWrMiss();
}
