
#include "config.h"

int str2int(std::string param) {
  return stoi(param);
}
void Config::loadAssoc(std::string param) {
  int num = str2int(param);
  associativity = num;
}
void Config::loadBlkSz(std::string param) {
  int num = str2int(param);
  blockSize = num;
}
void Config::loadCap(std::string param) {
  int num = str2int(param);
  capacity = num;
}
void Config::loadHitTIme(std::string param) {
  int num = str2int(param);
  hitTime = num;
}
void Config::loadDRAMAccessTime(std::string param) {
  int num = str2int(param);
  DRAMAccessTime = num;
}
void Config::loadAllocOnWrMiss(std::string param) {
  if (param == "true")
    allocOnWrMiss = true;
  else
    allocOnWrMiss = false;
}
void Config::loadmodel1(std::string param) {
  if (param == "none")
    mode_l1 = NONE;
  else if (param == "split")
    mode_l1 = SPLIT;
  else if (param == "unified")
    mode_l1 = UNIFIED;
}
void Config::loadReplaceAlg(std::string param) {
  if (param == "LRU")
    replaceAlg = -5;
  else if (param == "RND")
    replaceAlg = -6;
}
void Config::loadAssoc2(std::string param) {
  int num = str2int(param);
  associativity_l2 = num;
}
void Config::loadBlkSz2(std::string param) {
  int num = str2int(param);
  blockSize_l2 = num;
}
void Config::loadCap2(std::string param) {
  int num = str2int(param);
  capacity_l2 = num;
}
void Config::loadHitTIme2(std::string param) {
  int num = str2int(param);
  hitTime_l2 = num;
}

void Config::loadAllocOnWrMiss2(std::string param) {
  if (param == "true")
    allocOnWrMiss_l2 = true;
  else
    allocOnWrMiss_l2 = false;
}
void Config::loadmodel2(std::string param) {
  if (param == "none")
    mode_l2 = NONE;
  else if (param == "split")
    mode_l2 = SPLIT;
  else if (param == "unified")
    mode_l2 = UNIFIED;
}
void Config::loadReplaceAlg2(std::string param) {
  if (param == "LRU")
    replaceAlg_l2 = -5;
  else if (param == "RND")
    replaceAlg_l2 = -6;
}
void Config::loadOption(std::string option, std::string param) {
  if (option == "associativity")
    loadAssoc(param);
  else if (option == "blockSize")
    loadBlkSz(param);
  else if (option == "capacity")
    loadCap(param);
  else if (option == "hitTime")
    loadHitTIme(param);
  else if (option == "DRAMAccessTime")
    loadDRAMAccessTime(param);
  else if (option == "cacheMode_l1")
    loadmodel1(param);
  else if (option == "allocOnWrMiss")
    loadAllocOnWrMiss(param);
  else if (option == "replaceAlg")
    loadReplaceAlg(param);
  else if (option == "associativity_l2")
    loadAssoc2(param);
  else if (option == "blockSize_l2")
    loadBlkSz2(param);
  else if (option == "capacity_l2")
    loadCap2(param);
  else if (option == "hitTime_l2")
    loadHitTIme2(param);
  else if (option == "cacheMode_l2")
    loadmodel2(param);
  else if (option == "allocOnWrMiss_l2")
    loadAllocOnWrMiss2(param);
  else if (option == "replaceAlg_l2")
    loadReplaceAlg2(param);
}
std::string Config::getOption(std::string conf) {
  size_t pos = conf.find(":");
  return conf.substr(0, pos);
}
std::string Config::getParam(std::string conf) {
  size_t pos = conf.find_last_of(" ");
  return conf.substr(pos + 1);
}

Config::Config(std::string filename) :
    associativity(0),
    blockSize(0),
    capacity(0),
    hitTime(0),
    DRAMAccessTime(0) {
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  std::string conf;
  while (getline(ifs, conf)) {
    std::string option = getOption(conf);
    std::string param = getParam(conf);
    loadOption(option, param);
  }
  ifs.close();
}
int Config::getAssociativity() {
  return associativity;
}
int Config::getBlockSize() {
  return blockSize;
}
int Config::getCapacity() {
  return capacity;
}
int Config::getHitTime() {
  return hitTime;
}
int Config::getDRAMAccessTime() {
  return DRAMAccessTime;
}
int Config::getmodel1() {
  return mode_l1;
}
bool Config::getAllocOnWrMiss() {
  return allocOnWrMiss;
}
int Config::getReplaceAlg() {
  return replaceAlg;
}
int Config::getA_l2() {
  return associativity_l2;
}
int Config::getB_l2() {
  return blockSize_l2;
}
int Config::getC_l2() {
  return capacity_l2;
}
int Config::geth2() {
  return hitTime_l2;
}

int Config::getmodel2() {
  return mode_l2;
}
bool Config::getall2() {
  return allocOnWrMiss_l2;
}
int Config::getrA2() {
  return replaceAlg_l2;
}
