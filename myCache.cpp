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
  sysInit(associativity, blockSize, capacity, hitTime, DRAMAccessTime, mode, allocOnWrMiss);
  Parser parser(associativity, blockSize, capacity);
  Cache cache1(hitTime, DRAMAccessTime, associativity, blockSize, capacity, mode[0], allocOnWrMiss);
  Cache cache2(hitTime, DRAMAccessTime, associativity, blockSize, capacity, mode[1], allocOnWrMiss);
  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  std::string command;
  while (getline(ifs, command)) {
    parser.readCommand(command);
    cache1.operation(
        parser.getCmdType(), parser.getTag(), parser.b2D(parser.getSetid()), parser.getAddress());
    cache2.operation(
        parser.getCmdType(), parser.getTag(), parser.b2D(parser.getSetid()), parser.getAddress());
  }
  cache1.getHitRate();
  cache2.getHitRate();
  ifs.close();
  return EXIT_SUCCESS;
}
