# LPMwithPruning

## Introduction

A **sketch** is a probabilistic data structure that is used to record frequencies of different items in a multi-set.
Various types of sketches have been proposed in the literature and applied in a variety of fields, such as natural language processing, compressed sensing, network traffic analysis, and distributed data sets.
While several variants of sketches have been proposed in the past, the existing sketches cannot improve the accuracy of **the CM sketch** while keeping the update ability.
Here, we propose a new sketch, ***the MC sketch***, that has significantly higher accuracy compared to prior art, while keeping the query speed and update speed unchanged.


## Building

	$ sh build.sh
    
There is an example in main.cpp, which shows the basic usage of the Longest Prefix Matching with Pruning. For example, using sh build.sh, we can get an executable file named LPMwithPruning in ./bin.

## Usage

	$ ./bin/LPMwithPruning

Runing this executable file, the results will output to the folder ./result, the files FIBIP_* are results of FIB traffic, the files RandomIP_* are results of random traffic.
If you want to adjust the routing tables and the test traffic, you need to reference the data format of files in ./data4test/fibIp and./data4test/randomIP . 
Note: you may need to provide a default forward routing (such as "111.111.111.1/0 26") for each routing table file, so that if the longest prefix matching length is 0,it still can find a next hop for the ip, otherwise results are not accurate of provisions. 


## Workloads Generation
...