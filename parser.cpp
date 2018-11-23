#include "parser.h"
int Parser::b2D(std::string b) {
  int d = 0;
  for (int i = b.size() - 1; i >= 0; i--) {
    d += (b[i] - '0') * pow(2, b.size() - 1 - i);
  }
  return d;
}
Parser::Parser(int associativity, int blockSize, int capacity) : addrLen(6) {
  pos_offset = addrLen * 4 - log2(blockSize);
  pos_setid = addrLen * 4 - log2(blockSize) - log2(capacity / (associativity * blockSize));
}
std::string Parser::parseAddress(std::string address) {
  std::string newAddress;
  for (size_t i = 0; i < address.size(); i++) {
    std::string temp = h2B(address[i]);
    newAddress.append(temp);
  }
  return newAddress;
}
std::string Parser::readCmdType(std::string command) {
  return command.substr(0, command.find(" "));
}
std::string Parser::readAddress(std::string command) {
  std::string address = command.substr(command.find(" ") + 1);
  address = upd2CorLen(address);
  address = parseAddress(address);
  return address;
}
std::string Parser::addZeroAtFront(std::string address) {
  size_t diff = addrLen - address.size();
  while (diff--) {
    address.insert(0, "0");
  }
  return address;
}
std::string Parser::upd2CorLen(std::string address) {
  if (address.size() == addrLen) {
    return address;
  }
  return addZeroAtFront(address);
}
void Parser::readCommand(std::string command) {
  cmdType = readCmdType(command);
  address = readAddress(command);
}
std::string Parser::getOffset() {
  return address.substr(pos_offset);
}
std::string Parser::getSetid() {
  return address.substr(pos_setid, pos_offset - pos_setid);
}
std::string Parser::getTag() {
  return address.substr(0, pos_setid);
}
std::string Parser::getCmdType() {
  return cmdType;
}
std::string Parser::getAddress() {
  return address;
}
std::string Parser::h2B(char h) {
  std::string bin;
  switch (h) {
    case '0':
      bin = "0000";
      break;
    case '1':
      bin = "0001";
      break;
    case '2':
      bin = "0010";
      break;
    case '3':
      bin = "0011";
      break;
    case '4':
      bin = "0100";
      break;
    case '5':
      bin = "0101";
      break;
    case '6':
      bin = "0110";
      break;
    case '7':
      bin = "0111";
      break;
    case '8':
      bin = "1000";
      break;
    case '9':
      bin = "1001";
      break;
    case 'a':
      bin = "1010";
      break;
    case 'b':
      bin = "1011";
      break;
    case 'c':
      bin = "1100";
      break;
    case 'd':
      bin = "1101";
      break;
    case 'e':
      bin = "1110";
      break;
    case 'f':
      bin = "1111";
      break;
  }
  return bin;
}
