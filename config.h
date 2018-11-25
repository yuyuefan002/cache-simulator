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
  int replaceAlg;
  int associativity_l2;
  int blockSize_l2;
  int capacity_l2;
  int hitTime_l2;
  int mode_l2;
  bool allocOnWrMiss_l2;
  int replaceAlg_l2;
  void loadAssoc(std::string param);
  void loadBlkSz(std::string param);
  void loadCap(std::string param);
  void loadOption(std::string option, std::string param);
  void loadHitTIme(std::string param);
  void loadDRAMAccessTime(std::string param);
  void loadmodel1(std::string param);
  void loadAllocOnWrMiss(std::string param);
  void loadReplaceAlg(std::string param);
  void loadAssoc2(std::string param);
  void loadBlkSz2(std::string param);
  void loadCap2(std::string param);
  void loadOption2(std::string option, std::string param);
  void loadHitTIme2(std::string param);
  void loadmodel2(std::string param);
  void loadAllocOnWrMiss2(std::string param);
  void loadReplaceAlg2(std::string param);
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
  int getReplaceAlg();
  int getA_l2();
  int getB_l2();
  int getC_l2();
  int geth2();
  int getmodel2();
  bool getall2();
  int getrA2();
};
#endif
