#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "cache.h"
#include "config.h"
#include "init.h"
#include "parser.h"
void tavg(float a, std::string msg) {
  fprintf(stdout, "%s tavg: %.2f cycle\n", msg.c_str(), a);
}
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage:%s filename\n", argv[0]);
    return EXIT_FAILURE;
  }
  fprintf(
      stdout,
      "simulating on DUKE server.Ubuntu 18.04.1 LTS Intel(R) Xeon(R) CPU E5-2690 v3 @ 2.60GHz\n");
  int associativity;
  int blockSize;
  int capacity;
  int hitTime;
  int DRAMAccessTime;
  int mode[2] = {0};
  bool allocOnWrMiss;
  int replaceAlg;
  bool l2 = false;
  Cache * l2cforc1 = nullptr;
  Cache * l2cforc2 = nullptr;
  Parser * l2p = nullptr;
  int A_l2, B_l2, C_l2, h2;
  int mode2[2] = {0};
  bool all2;
  int rA2;
  sysInit(associativity,
          blockSize,
          capacity,
          hitTime,
          DRAMAccessTime,
          mode,
          allocOnWrMiss,
          replaceAlg,
          A_l2,
          B_l2,
          C_l2,
          h2,
          mode2,
          all2,
          rA2);
  Parser parser2(A_l2, B_l2, C_l2);
  Cache cache3(h2, DRAMAccessTime, A_l2, B_l2, C_l2, mode2[0], all2, rA2, false, nullptr, nullptr);
  Cache cache4(h2, DRAMAccessTime, A_l2, B_l2, C_l2, mode2[1], all2, rA2, false, nullptr, nullptr);
  if (mode2[0] || mode2[1]) {
    l2 = true;
    l2p = &parser2;
  }
  if (mode2[0] == D_MEM_ONLY) {
    l2cforc1 = &cache3;
    l2cforc2 = &cache4;
  }
  else if (mode2[0] == UNI_MEM) {
    l2cforc1 = l2cforc2 = &cache3;
  }
  Parser parser(associativity, blockSize, capacity);
  Cache cache1(hitTime,
               DRAMAccessTime,
               associativity,
               blockSize,
               capacity,
               mode[0],
               allocOnWrMiss,
               replaceAlg,
               l2,
               l2cforc1,
               l2p);
  Cache cache2(hitTime,
               DRAMAccessTime,
               associativity,
               blockSize,
               capacity,
               mode[1],
               allocOnWrMiss,
               replaceAlg,
               l2,
               l2cforc2,
               l2p);

  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  std::string command;
  while (getline(ifs, command)) {
    parser.readCommand(command);
    std::string cmdType = parser.getCmdType();
    std::string tag = parser.getTag();
    int setid = parser.b2D(parser.getSetid());
    std::string address = parser.getAddress();
    cache1.operation(cmdType, tag, setid, address);
    cache2.operation(cmdType, tag, setid, address);
  }
  /*Parser parser2(capacity / blockSize, blockSize, capacity);
  Cache cache3(hitTime,
               DRAMAccessTime,
               capacity / blockSize,
               blockSize,
               capacity,
               mode[0],
               allocOnWrMiss,
               replaceAlg);
  Cache cache4(hitTime,
               DRAMAccessTime,
               capacity / blockSize,
               blockSize,
               capacity,
               mode[1],
               allocOnWrMiss,
               replaceAlg);
  ifs.close();
  ifs.open(argv[1], std::ifstream::in);
  while (getline(ifs, command)) {
    parser2.readCommand(command);
    std::string cmdType = parser2.getCmdType();
    std::string tag = parser2.getTag();
    int setid = parser.b2D(parser2.getSetid());
    std::string address = parser2.getAddress();
    cache3.operation(cmdType, tag, setid, address);
    cache4.operation(cmdType, tag, setid, address);
    }*/
  std::vector<int> res1 = cache1.getHitRate();
  std::vector<int> res2 = cache2.getHitRate();
  std::vector<int> res3 = cache3.getHitRate();
  std::vector<int> res4 = cache4.getHitRate();
  /*res1[12] = res1[9] - res3[9];
  res1[13] = res1[10] - res3[10];
  res1[14] = res1[11] - res3[11];
  res1[9] = res3[9];
  res1[10] = res3[10];
  res1[11] = res3[11];
  res2[12] = res2[9] - res4[9];
  res2[13] = res2[10] - res4[10];
  res2[14] = res2[11] - res4[11];
  res2[9] = res4[9];
  res2[10] = res4[10];
  res2[11] = res4[11];*/
  printResult(res1, res2);
  float tavg_r_l2 = DRAMAccessTime;
  float tavg_ir_l2 = DRAMAccessTime;
  if (l2) {
    tavg_r_l2 = h2 + 1.0 * (res3[3] + res4[3]) / (res3[1] + res4[1]) * DRAMAccessTime;
    tavg_ir_l2 = h2 + 1.0 * (res3[5] + res4[5]) / (res3[0] + res4[0]) * DRAMAccessTime;
  }
  float tavg_r_l1 = hitTime + 1.0 * (res1[3] + res2[3]) / (res1[1] + res2[1]) * tavg_r_l2;
  float tavg_ir_l1 = hitTime + 1.0 * (res1[5] + res2[5]) / (res1[0] + res2[0]) * tavg_ir_l2;
  tavg(tavg_r_l1, "\nl1 read");
  tavg(tavg_ir_l1, "l1 fetch");
  if (l2) {
    fprintf(stdout, "+-----------------------------------------------+\n");
    printResult(res3, res4);

    tavg(tavg_r_l2, "\nl2 read");
    tavg(tavg_ir_l2, "l2 fetch");
    fprintf(stdout, "+-----------------------------------------------+\n");
  }
  ifs.close();
  return EXIT_SUCCESS;
}
