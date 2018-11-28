#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <vector>

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
  int hitTime, h2, DRAMAccessTime;
  std::vector<int> associativity, blockSize, capacity, replaceAlg;
  int mode[2] = {0};
  std::vector<bool> allocOnWrMiss;
  bool l2 = false;
  Cache * l2cforc1 = nullptr;
  Cache * l2cforc2 = nullptr;
  Parser * l2p1 = nullptr;
  Parser * l2p2 = nullptr;
  std::vector<int> A_l2, B_l2, C_l2, rA2;
  int mode2[2] = {0};
  std::vector<bool> all2;
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
  Parser parser3(A_l2[0], B_l2[0], C_l2[0]);
  Parser parser4(A_l2[1], B_l2[1], C_l2[1]);
  Cache cache3(h2,
               DRAMAccessTime,
               A_l2[0],
               B_l2[0],
               C_l2[0],
               mode2[0],
               all2[0],
               rA2[0],
               false,
               nullptr,
               nullptr);
  Cache cache4(h2,
               DRAMAccessTime,
               A_l2[1],
               B_l2[1],
               C_l2[1],
               mode2[1],
               all2[1],
               rA2[1],
               false,
               nullptr,
               nullptr);
  if (mode2[0] || mode2[1]) {
    l2 = true;
  }
  if (mode2[0] == D_MEM_ONLY) {
    l2cforc1 = &cache3;
    l2p1 = &parser3;
    l2cforc2 = &cache4;
    l2p2 = &parser4;
  }
  else if (mode2[0] == UNI_MEM) {
    l2cforc1 = l2cforc2 = &cache3;
    l2p1 = &parser3;
    l2p2 = &parser3;
  }
  Parser parser1(associativity[0], blockSize[0], capacity[0]);
  Parser parser2(associativity[1], blockSize[1], capacity[1]);
  Cache cache1(hitTime,
               DRAMAccessTime,
               associativity[0],
               blockSize[0],
               capacity[0],
               mode[0],
               allocOnWrMiss[0],
               replaceAlg[0],
               l2,
               l2cforc1,
               l2p1);
  Cache cache2(hitTime,
               DRAMAccessTime,
               associativity[1],
               blockSize[1],
               capacity[1],
               mode[1],
               allocOnWrMiss[1],
               replaceAlg[1],
               l2,
               l2cforc2,
               l2p2);

  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  std::string command;

  while (getline(ifs, command)) {
    parser1.readCommand(command);
    std::string cmdType = parser1.getCmdType();
    std::string tag = parser1.getTag();
    int setid = parser1.b2D(parser1.getSetid());
    std::string address = parser1.getAddress();
    cache1.operation(cmdType, tag, setid, address);
    parser2.readCommand(command);
    cmdType = parser2.getCmdType();
    tag = parser2.getTag();
    setid = parser2.b2D(parser2.getSetid());
    address = parser2.getAddress();
    cache2.operation(cmdType, tag, setid, address);
  }

  sys.printRunInfo(
      cache1.getHitRate(), cache2.getHitRate(), cache3.getHitRate(), cache4.getHitRate());

  ifs.close();
  return EXIT_SUCCESS;
}
