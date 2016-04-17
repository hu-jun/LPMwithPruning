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

Runing this executable file , it will output experiment results in the form of files into `./result`: the files with the file name like `FIBIP_*.txt` are results of FIB traffic, the files like `RandomIP_*.txt` are results of random traffic.
If you want to use different routing tables or IP packet traffics for test, please refer to the data format in `./data4test/fibIp and./data4test/randomIP` respectvely. 

***Note:*** you may need to provide a default forward routing (such as "111.111.111.1/0 26") for each routing table file, so that if the longest prefix matching length is 0,it still can find a next hop for the ip, otherwise results are not accurate of provisions. 


## Workloads Generation
...
