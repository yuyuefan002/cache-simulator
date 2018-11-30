/*************************************************

Description: the parser class, which is used for 
parse the command line, split the command type, 
tag, setid and offset apart for further use.
It is much easier to realize in the hardware.
However, in the software, you have to be aware of
the variable type, and spliting them apart is not as 
easy as it in the hardware.

Author:Yuefan Yu

Date:11/24/2018
**************************************************/

#include "parser.h"

// binary to deximal
int Parser::b2D(std::string b) {
  int d = 0;
  for (int i = b.size() - 1; i >= 0; i--) {
    d += (b[i] - '0') * pow(2, b.size() - 1 - i);
  }
  return d;
}

// init
Parser::Parser(int associativity, int blockSize, int capacity) :
    cmdType(""),
    address(""),
    addrLen(6) {
  pos_offset = addrLen * 4 - log2(blockSize);
  pos_setid = addrLen * 4 - log2(blockSize) - log2(capacity / (associativity * blockSize));
}

// parse the address in the command, return the bin type
std::string Parser::parseAddress(std::string address) {
  std::string newAddress;
  // from start to end
  for (size_t i = 0; i < address.size(); i++) {
    // hex to bin
    std::string temp = h2B(address[i]);
    // append at the end of new string
    newAddress.append(temp);
  }
  return newAddress;
}

// split the command type from command
std::string Parser::readCmdType(std::string command) {
  return command.substr(0, command.find(" "));
}

// split the address from command
std::string Parser::readAddress(std::string command) {
  std::string address = command.substr(command.find(" ") + 1);

  // the address may omit zero at front, restore it
  address = upd2CorLen(address);
  // get the binary type address
  address = parseAddress(address);
  return address;
}

// add zero at the front of address, some address omits 0 at front
std::string Parser::addZeroAtFront(std::string address) {
  size_t diff = addrLen - address.size();
  while (diff--) {
    address.insert(0, "0");
  }
  return address;
}

// return the correct length of address
std::string Parser::upd2CorLen(std::string address) {
  if (address.size() == addrLen) {
    return address;
  }
  return addZeroAtFront(address);
}

// read the command
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

//transfer hex to bin
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
void Parser::setAddress(std::string addr) {
  address = addr;
}
