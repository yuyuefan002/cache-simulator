#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <fstream>
#include <string>
#include <vector>
#define NONE 0
#define SPLIT 1
#define UNIFIED 2
class Config
{
  std::vector<int> associativity;
  std::vector<int> blockSize;
  std::vector<int> capacity;
  int hitTime;
  int DRAMAccessTime;
  int mode_l1;
  std::vector<bool> allocOnWrMiss;
  std::vector<int> replaceAlg;
  std::vector<int> associativity_l2;
  std::vector<int> blockSize_l2;
  std::vector<int> capacity_l2;
  int hitTime_l2;
  int mode_l2;
  std::vector<bool> allocOnWrMiss_l2;
  std::vector<int> replaceAlg_l2;
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
  void loadAssoc_I(std::string param);
  void loadBlkSz_I(std::string param);
  void loadCap_I(std::string param);
  void loadAllocOnWrMiss_I(std::string param);
  void loadReplaceAlg_I(std::string param);
  void loadAssoc2_I(std::string param);
  void loadBlkSz2_I(std::string param);
  void loadCap2_I(std::string param);
  void loadAllocOnWrMiss2_I(std::string param);
  void loadReplaceAlg2_I(std::string param);
  std::string getOption(std::string conf);
  std::string getParam(std::string conf);

 public:
  Config(std::string filename);
  std::vector<int> getAssociativity();
  std::vector<int> getBlockSize();
  std::vector<int> getCapacity();
  int getHitTime();
  int getDRAMAccessTime();
  int getmodel1();
  std::vector<bool> getAllocOnWrMiss();
  std::vector<int> getReplaceAlg();
  std::vector<int> getA_l2();
  std::vector<int> getB_l2();
  std::vector<int> getC_l2();
  int geth2();
  int getmodel2();
  std::vector<bool> getall2();
  std::vector<int> getrA2();
};
#endif
