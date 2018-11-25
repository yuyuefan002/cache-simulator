#ifndef __PARSER_H__
#define __PARSER_H__
#include <math.h>

#include <string>
#include <vector>
class Parser
{
 private:
  std::string cmdType;
  std::string address;
  size_t addrLen;
  size_t pos_offset;
  size_t pos_setid;
  std::string readCmdType(std::string command);
  std::string readAddress(std::string command);
  std::string parseAddress(std::string address);
  std::string h2B(char h);
  std::string addZeroAtFront(std::string address);
  std::string upd2CorLen(std::string address);

 public:
  Parser(int associativity, int blockSize, int capacity);
  void readCommand(std::string command);
  int b2D(std::string b);
  std::string getCmdType();
  std::string getAddress();
  std::string getOffset();
  std::string getSetid();
  std::string getTag();
  void setAddress(std::string addr);
};
#endif
