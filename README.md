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

Runing this executable file, it will output results to the folder `./result`, the files as FIBIP_\*.txt are results of FIB traffic, the files as RandomIP_\*.txt are results of random traffic.
If you want to adjust the routing tables and the test traffic, you need to reference the data format of files in `./data4test/fibIp and./data4test/randomIP`. 
***Note:*** you may need to provide a default forward routing (such as "111.111.111.1/0 26") for each routing table file, so that if the longest prefix matching length is 0,it still can find a next hop for the ip, otherwise results are not accurate of provisions. 


## Workloads Generation
...
