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

To run `./bin/LPMwithPruning`, we prepare FIB in `./data4test/route_table.tr` for the algorithm's initialization.
We also packet IP packet traffic in `./data4test/fib_flow.tr`.
Given those two input files, we can execute `./bin/LPMwithPruning`, and it will store IP lookup results into `./result/result.txt`.
During IP lookup process, we ales count the prefix probe times for each IP in prefix-next hop storage sub system which manage the FIB and handle prefix query for next-hop value.
For each IP address in `./data4test/fib_flow.tr`, after IP lookup, the program store a record of three items: the next-hop value, longest matching prefix and the probe times in `./result/result.txt`.
To make the program work flexibly, you can change input files for FIB, IP packet traffic, or vary the pivot level, the number of hash function for Bloom filter in main.cpp, and then rebuild the program using command `sh build.sh`.

***Note:*** If an IP lookup doesn't match any prefix, we should return a default next-hop for that IP.
So if there doesn't exist any record like `0.0.0.0/0 [default_value]` in FIB, you should add such next-hop record for prefix `0.0.0.0/0` with a default next-hop value `[default_value]` as `0.0.0.0/0 [default_value]` to that FIB file manually.

## Getting FIB
Maybe you should need some real FIBs for test, you can download real FIBs freely from [RIPE](https://www.ripe.net/).
