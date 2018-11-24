#include "init.h"

void sysInit(int & associativity,
             int & blockSize,
             int & capacity,
             int & hitTime,
             int & DRAMAccessTime,
             int * mode,
             bool & allocOnWrMiss,
             int & replaceAlg) {
  Config config("cache.config");
  associativity = config.getAssociativity();
  blockSize = config.getBlockSize();
  capacity = config.getCapacity();
  hitTime = config.getHitTime();
  DRAMAccessTime = config.getDRAMAccessTime();
  fprintf(stdout, "+-----------------------------------------------+\n");
  fprintf(stdout, "Cache info:\n\n");
  fprintf(stdout,
          "associatiity: %d    blockSize: %d    capacity: %d\n",
          associativity,
          blockSize,
          capacity);
  fprintf(stdout, "hit Time: %d cycle   DRAMAccesstime: %d cycle   \n", hitTime, DRAMAccessTime);
  fprintf(stdout, "l1 mode: ");
  if (config.getmodel1() == SPLIT) {
    mode[0] = D_MEM_ONLY;
    mode[1] = I_MEM_ONLY;
    fprintf(stdout, "split\n");
  }
  else if (config.getmodel1() == UNIFIED) {
    mode[0] = UNI_MEM;
    mode[1] = None;
    fprintf(stdout, "unified\n");
  }
  else {
    fprintf(stdout, "none\n");
  }
  allocOnWrMiss = config.getAllocOnWrMiss();
  fprintf(stdout, "alloc on write miss: ");
  fprintf(stdout, allocOnWrMiss ? "True\n" : "False\n");
  replaceAlg = config.getReplaceAlg();
  fprintf(stdout, "replacement algorithm: ");
  fprintf(stdout, replaceAlg == LRU ? "LRU\n" : "RND\n");
  fprintf(stdout, "+-----------------------------------------------+\n");
  fprintf(stdout, "operation info:\n\n");
}
