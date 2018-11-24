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
  int associativity;
  int blockSize;
  int capacity;
  int hitTime;
  int DRAMAccessTime;
  int mode[2] = {0};
  bool allocOnWrMiss;
  int replaceAlg;
  sysInit(
      associativity, blockSize, capacity, hitTime, DRAMAccessTime, mode, allocOnWrMiss, replaceAlg);
  Parser parser(associativity, blockSize, capacity);
  Cache cache1(hitTime,
               DRAMAccessTime,
               associativity,
               blockSize,
               capacity,
               mode[0],
               allocOnWrMiss,
               replaceAlg,
               false);
  Cache cache2(hitTime,
               DRAMAccessTime,
               associativity,
               blockSize,
               capacity,
               mode[1],
               allocOnWrMiss,
               replaceAlg,
               false);
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
  /*std::vector<int> res3 = cache3.getHitRate();
  std::vector<int> res4 = cache4.getHitRate();
  res1[12] = res1[9] - res3[9];
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
  ifs.close();
  fprintf(stdout, "+-----------------------------------------------+\n");
  return EXIT_SUCCESS;
}
