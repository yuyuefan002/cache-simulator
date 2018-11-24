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



Description: I programed a cache simulator, which can simulate Fully Associated Cache, Directed Mapped Cache and Set Associated Cache. This is a one-level cache simulator, l1 cache is linked directly to DRAM. You can configure several things: 1. the l1 cache in split/unified/none mode. 2. The store strategy is write-back, there is a store buffer for write data. You can also turn on/off allocate on write miss mode, which is moved missed line into cache when writing. 3. replacement algorithm in LRU/RND mode.

Note that if you want to distinguish conflict miss and capacity miss. There are two solution. The first is run in the fully associated mode, to see how many conflict+capacity miss there is. Since no confilct miss in fa cache, so all of them are capacity miss. Then go back to your configuration to get the conflict miss. The second way is to distinguish capacity miss is that if consecutive accesses to the same block are separated by accesses to at least N other distinct blocks where N is number of blocks in cache, then it is a capacity miss.

Note that if turn on allocOnWrMiss, you can observe that write hit rate rises a lot.