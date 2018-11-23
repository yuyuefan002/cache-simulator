#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <fstream>
#include <string>
#define NONE 0
#define SPLIT 1
#define UNIFIED 2
class Config
{
  int associativity;
  int blockSize;
  int capacity;
  int hitTime;
  int DRAMAccessTime;
  int mode_l1;
  bool allocOnWrMiss;
  void loadAssoc(std::string param);
  void loadBlkSz(std::string param);
  void loadCap(std::string param);
  void loadOption(std::string option, std::string param);
  void loadHitTIme(std::string param);
  void loadDRAMAccessTime(std::string param);
  void loadmodel1(std::string param);
  void loadAllocOnWrMiss(std::string param);
  std::string getOption(std::string conf);
  std::string getParam(std::string conf);

 public:
  Config(std::string filename);
  int getAssociativity();
  int getBlockSize();
  int getCapacity();
  int getHitTime();
  int getDRAMAccessTime();
  int getmodel1();
  bool getAllocOnWrMiss();
};
#endif
