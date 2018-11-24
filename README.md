# cache-simulator
1. open "cache.config"

2. configure the parameter:
associativity: 2^n
blockSize: 2^n
capacity: 2^n
hitTime: int
DRAMAccessTime: int
cacheMode_l1: split/unified/none
allocOnWrMiss: false/true
replaceAlg: LRU/RND

3. make

4. ./myCache Dinero10000.din