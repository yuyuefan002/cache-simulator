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