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

Runing this executable file, it will store experiment results into files in `./result`. There has two types of results: one is xxxx stored in files with the file name format of `FIBIP_*.txt`, the other is xxxx stored in files with the file name format of `RandomIP_*.txt`.
If you want to use different routing tables or IP packet traffics for test, please refer to the data format in `./data4test/fibIp and./data4test/randomIP` respectvely. 

***Note:*** If an IP lookup doesn't mathch any prefix, we should return a default next-hop for that IP.
So if there dosen't exist any record like `*/0 [default_value]` in FIB, you should add such next-hop record for prefix `*/0` with a default next-hop value `[default_value]` as `*/0 [default_value]` to that FIB file manualy. 

In this example,input includes part A(a file path of the fib, a file path of automatic generation of IP packet traffic, and a pivot level number specified by user),part B(a file path of the fib, a file path of prefix-based traffic traffic, and a pivot level number specified by user).On the one hand,part A is used to test random traffic,on the other hand,part B is used to test prefix-based traffic. You can modify input parameters in main.cpp and then rebuild the program by using command sh build.sh.

Our algorithm returns the corresponding next hop port of each IP, it also returns the real detection times count of all IP trafics, the real detection refers to the query in FIB table to find the longest matching prefix.

## Workloads Generation
In our experiments, we focus on the longest prefix matching with IPv4. We use IPv4 FIB tables downloaded from 5 routers provided by RIPE in August 9 2013(https://www.ripe.net/), and assign each FIB table with a specific IP packet traffic containing approximately 5 million IP packets. To validate the robustness and efficiency of our proposed IP lookup algorithm, we generated two different IP packet traffic patterns:
1.random traffic: randomly generated IP addresses.
2.prefix-based traffic: IP addresses generated obey the prefix distribution of the corresponding FIB table.
