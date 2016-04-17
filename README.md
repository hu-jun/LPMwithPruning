# LPMwithPruning

## Introduction

Nowadays, the FIB size of backbone routers takes on an explosive increase, making IP lookup a challenging issue. 
An effective solution is to use Bloom filters. 
However, Bloom filters have false positives, making Bloom filter based algorithms suffer from query speed degradation with FIB explosion.
To address this issue, we propose a novel algorithm — **Longest Prefix Matching with Pruning**, which prune unneeded prefix probes incurred by false positive.


## Building

    $ sh build.sh
    
There is an example in main.cpp, which shows the basic usage of the Longest Prefix Matching with Pruning algorithm. For example, using **sh build.sh**, we can get an executable file named **LPMwithPruning** in ./bin.

## Usage

	$ ./bin/LPMwithPruning

Runing this executable file, it will store experiment results into files in `./result`. There has two types of results: one is xxxx represented by files with the file name format of `FIBIP_*.txt`, the other is xxxx represented by files with the file name format of `RandomIP_*.txt`.
If you want to use different routing tables or IP packet traffics for test, please refer to the data format in `./data4test/fibIp and./data4test/randomIP` respectvely. 

***Note:*** you may need to add a next-hop record with a default next-hop value `default value` as "111.111.111.1/0 `default value`" to each FIB file for the case that an IP lookup doesn't mathch any prefix, so we can return the next-hop with `default value`. 


## Workloads Generation
...
