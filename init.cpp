#include "init.h"
Sys::Sys() : h_l1(0), h_l2(0), DAT(0), l2(false) {}
void Sys::sysInit(int & associativity,
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
                  int & rA2) {
  Config config("cache.config");
  associativity = config.getAssociativity();
  A_l2 = config.getA_l2();
  blockSize = config.getBlockSize();
  B_l2 = config.getB_l2();
  capacity = config.getCapacity();
  C_l2 = config.getC_l2();
  h_l1 = hitTime = config.getHitTime();
  h_l2 = h2 = config.geth2();
  DAT = DRAMAccessTime = config.getDRAMAccessTime();
  fprintf(stdout, "+-----------------------------------------------+\n");
  fprintf(stdout, "L1 Cache info:\n\n");
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
  if (config.getmodel2() == SPLIT) {
    mode2[0] = D_MEM_ONLY;
    mode2[1] = I_MEM_ONLY;
    l2 = true;
  }
  else if (config.getmodel2() == UNIFIED) {
    mode2[0] = UNI_MEM;
    mode2[1] = None;
    l2 = true;
  }
  allocOnWrMiss = config.getAllocOnWrMiss();
  all2 = config.getall2();
  fprintf(stdout, "alloc on write miss: ");
  fprintf(stdout, allocOnWrMiss ? "True\n" : "False\n");
  replaceAlg = config.getReplaceAlg();
  rA2 = config.getrA2();
  fprintf(stdout, "replacement algorithm: ");
  fprintf(stdout, replaceAlg == LRU ? "LRU\n" : "RND\n");
  fprintf(stdout, "+-----------------------------------------------+\n");
  if (config.getmodel2() != SPLIT && config.getmodel2() != UNIFIED) {
    return;
  }
  fprintf(stdout, "+-----------------------------------------------+\n");
  fprintf(stdout, "L2 Cache info:\n\n");
  fprintf(stdout, "associatiity: %d    blockSize: %d    capacity: %d\n", A_l2, B_l2, C_l2);
  fprintf(stdout, "hit Time: %d cycle   DRAMAccesstime: %d cycle   \n", h2, DRAMAccessTime);
  fprintf(stdout, "l2 mode: ");
  if (config.getmodel2() == SPLIT) {
    fprintf(stdout, "split\n");
  }
  else if (config.getmodel2() == UNIFIED) {
    fprintf(stdout, "unified\n");
  }
  else {
    fprintf(stdout, "none\n");
  }

  fprintf(stdout, "alloc on write miss: ");
  fprintf(stdout, all2 ? "True\n" : "False\n");
  fprintf(stdout, "replacement algorithm: ");
  fprintf(stdout, rA2 == LRU ? "LRU\n" : "RND\n");
  fprintf(stdout, "+-----------------------------------------------+\n");
}
void Sys::printResult(std::vector<int> res1, std::vector<int> res2) {
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
          "Demand Fetches     %7d        %5d       %5d      %5d      %5d       %5d\n",
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
          "Demand Misses      %7d        %5d       %5d      %5d      %5d       %5d\n",
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
          "  Compulsory Misses%7d        %5d       %5d      %5d      %5d       %5d\n",
          (cormiss + cowmiss + coirmiss),
          coirmiss,
          cormiss + cowmiss,
          cormiss,
          cowmiss,
          zero);
  fprintf(stdout,
          "  Capacity Misses  %7d        %5d       %5d      %5d      %5d       %5d\n",

          (caprmiss + capwmiss + capirmiss),
          capirmiss,
          caprmiss + capwmiss,
          caprmiss,
          capwmiss,
          zero);
  fprintf(stdout,
          "  Conflict Misses  %7d        %5d       %5d      %5d      %5d       %5d\n",

          (cfrmiss + cfwmiss + cfirmiss),
          cfirmiss,
          cfrmiss + cfwmiss,
          cfrmiss,
          cfwmiss,
          zero);
}
void tavg(float a, std::string msg) {
  fprintf(stdout, "%s tavg: %.2f cycle\n", msg.c_str(), a);
}
void Sys::printRunInfo(std::vector<int> res1,
                       std::vector<int> res2,
                       std::vector<int> res3,
                       std::vector<int> res4) {
  printResult(res1, res2);
  float tavg_r_l2 = DAT;
  float tavg_ir_l2 = DAT;
  if (l2) {
    tavg_r_l2 = h_l2 + 1.0 * (res3[3] + res4[3]) / (res3[1] + res4[1]) * DAT;
    tavg_ir_l2 = h_l2 + 1.0 * (res3[5] + res4[5]) / (res3[0] + res4[0]) * DAT;
  }
  float tavg_r_l1 = h_l1 + 1.0 * (res1[3] + res2[3]) / (res1[1] + res2[1]) * tavg_r_l2;
  float tavg_ir_l1 = h_l1 + 1.0 * (res1[5] + res2[5]) / (res1[0] + res2[0]) * tavg_ir_l2;
  tavg(tavg_r_l1, "\nl1 read");
  tavg(tavg_ir_l1, "l1 fetch");
  if (l2) {
    fprintf(stdout, "+-----------------------------------------------+\n");
    printResult(res3, res4);

    tavg(tavg_r_l2, "\nl2 read");
    tavg(tavg_ir_l2, "l2 fetch");
    fprintf(stdout, "+-----------------------------------------------+\n");
  }
}
