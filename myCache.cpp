#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "cache.h"
#include "config.h"
#include "init.h"
#include "parser.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage:%s filename\n", argv[0]);
    return EXIT_FAILURE;
  }
  fprintf(
      stdout,
      "simulating on DUKE server.Ubuntu 18.04.1 LTS Intel(R) Xeon(R) CPU E5-2690 v3 @ 2.60GHz\n");
  Sys sys;
  int associativity, blockSize, capacity, hitTime, DRAMAccessTime, replaceAlg;
  int mode[2] = {0};
  bool allocOnWrMiss;
  bool l2 = false;
  Cache * l2cforc1 = nullptr;
  Cache * l2cforc2 = nullptr;
  Parser * l2p = nullptr;
  int A_l2, B_l2, C_l2, h2, rA2;
  int mode2[2] = {0};
  bool all2;
  sys.sysInit(associativity,
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

  sys.printRunInfo(
      cache1.getHitRate(), cache2.getHitRate(), cache3.getHitRate(), cache4.getHitRate());

  ifs.close();
  return EXIT_SUCCESS;
}
