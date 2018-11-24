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
void printResult(std::vector<int> res1, std::vector<int> res2) {
  int zero = 0;
  int instrn = res1[0] + res2[0];
  int read = res1[1] + res2[1];
  int write = res1[2] + res2[2];
  int rmiss = res1[3] + res2[3];
  int wmiss = res1[4] + res2[4];
  int irmiss = res1[5] + res2[5];
  int cormiss = res1[6] + res2[6];
  int cowmiss = res1[8] + res2[8];
  int coirmiss = res1[7] + res2[7];
  int caprmiss = res1[9] + res2[9];
  int capwmiss = res1[10] + res2[10];
  int capirmiss = res1[11] + res2[11];
  int cfrmiss = res1[12] + res2[12];
  int cfwmiss = res1[13] + res2[13];
  int cfirmiss = res1[14] + res2[14];
  fprintf(stdout,
          "Metrics              Total       Instrn       Data       Read       Write       Misc\n");
  fprintf(
      stdout,
      "------               -----       ------       -----      -----      -----       -----\n");

  fprintf(stdout,
          "Demand Fetches      %7d      %5d       %5d       %5d       %5d      %5d\n",
          instrn + read + write,
          instrn,
          read + write,
          read,
          write,
          zero);

  float total = instrn + read + write;
  fprintf(stdout,
          " Fraction of total   %.4f      %.4f       %.4f     %.4f     %.4f      %.4f\n",
          (total) / total,
          instrn / total,
          (read + write) / total,
          read / total,
          write / total,
          zero / total);
  fprintf(stdout,
          "Demand Misses      %7d       %5d        %5d     %5d      %5d       %5d\n",
          rmiss + wmiss + irmiss,
          irmiss,
          rmiss + wmiss,
          rmiss,
          wmiss,
          zero);

  fprintf(stdout,
          " Demand Misses rate  %.4f      %.4f       %.4f     %.4f     %.4f      %.4f\n",
          (rmiss + wmiss + irmiss) / total,
          1.0 * irmiss / instrn,
          1.0 * (rmiss + wmiss) / (read + write),
          1.0 * rmiss / read,
          1.0 * wmiss / write,
          1.0 * zero);
  fprintf(stdout,
          "  Compulsory Misses   %d         %d          %d         %d         %d         %d\n",
          (cormiss + cowmiss + coirmiss),
          coirmiss,
          cormiss + cowmiss,
          cormiss,
          cowmiss,
          zero);
  fprintf(stdout,
          "  Capacity Misses     %d          %d           %d          %d          %d          %d\n",
          (caprmiss + capwmiss + capirmiss),
          capirmiss,
          caprmiss + capwmiss,
          caprmiss,
          capwmiss,
          zero);
  fprintf(stdout,
          "  Conflict Misses      %d           %d            %d           %d          %d         "
          " %d\n",
          (cfrmiss + cfwmiss + cfirmiss),
          cfirmiss,
          cfrmiss + cfwmiss,
          cfrmiss,
          cfwmiss,
          zero);
}
