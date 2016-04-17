#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <tr1/unordered_map>
#include "std_bf.h"

using namespace std;


#define EMPTYHOP		0    //Trie node doesnt't has a next hop
#define LEVELCOUNT     33  //The count of all levels
#define BFCOUNT        34   //Bloom filters' size
#define PIVOT_LEVEL_NUM 24      //default pivot push at level 24th
#define BFHASH_COUNT_DEFAULT  4    //The count of hash function in each bloom filter
#define Ln2                 0.69314718055995

#define MAX_LINE_QUERY 5000000   //The max count of each query


struct Node
{
    Node* parent;
    Node* lchild;
    Node* rchild;
    char * prefix; //The prefix of a IP
    int nodeLevel; //The node belong to which level
    int next_hop;
};

class Trie
{
public:
    Node* rootNode;                 //root node of Trie
    unsigned int allNodeCount;      //to count all the nodes in Trie tree, including empty node
    unsigned int solidNodeCount;    //to count all the solid nodes in Trie tree
	
	string route_table_file;       //Routing table file
	string query_file;				//The file to query

    unsigned int pivotLevelNum;  // pivot level at the #th level

    unsigned int bf_hash_count;  //The count of hash function in each bloom filter

    uint64_t memory_access;

    std::tr1::unordered_map<std::string,uint32_t>* unodMap; //bulid a map for each level to query the next-hop of a IP. key is the prefix of the ip, value is the next-hop of the IP.

    StandardBF *bloomFilter[BFCOUNT];
    unsigned int elementCount[BFCOUNT]; //the count of nodes of ecah bloom filter will check

    unsigned int hit_count[LEVELCOUNT]; //the count of hit number at every level.

    unsigned int probe_count[LEVELCOUNT];


    Trie(unsigned int pivotLevelNumber, unsigned int bfhashcount, string route_table_file, string query_file);
    ~Trie();

    // creat a new ndoe
    void createNewNode(Node* &node);

    //get the total number of nodes in Trie
	void getNodesCounts();
	//initiate the bloom filters
	void initBloomFilters();

	//traversal the trie
	void pretraversal(Node* root);

	void reset_hit_and_probe_count();

    bool isLeaf(Node * node);


	void addToTrie(int insertport, char *insert_C, int operation_type, char *sprefix);

    unsigned int buildTrieFromFile(string FileName, Node * root);

    void trieDestroy(Node *root);


    //if ip is 255.1.1.1/8,return 11110001
    char* ipToLongThenToChar(const string &ip,int &iPrefixLen);

    //if 'node' is solid,return the next-hop of 'node'.
    //if 'node' is empty,return the next-hop of its nearest ancestor
    unsigned int GetAncestorHop(Node* node);


    //pivot at the 'levelNum' level
    void pivotLevel(Node *root, int levelNum);


    //when trie is changed,maps and bloom filters should rebuild
    void resetThenBuildMapsAndBloomFilters(Node *root, std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs,int bfCount,int pivotLevelNum);

    //query next-hop from 'levelNum' level to 0th level. 'queryCount' is the count of probe at maps
    int queryNextHopFromLevelNum(const string &ip, int levelNum, int& queryCount);

    //query next-hop at 'levelNum' level level. 'queryCount' is the count of probe at maps
    int queryNextHopAtLevelNum(const string &ip, int levelNum);

    //use bloom filters to query next-hop from 'levelNumFrom' level to 'levelNumTo' level. 'queryCount' is the count of probe at maps
    int queryNextHopByBFFromLevel1ToLevel2(const string &ip, int levelNumFrom, int levelNumTo, int& queryCount);

    //use bloom filters to query next-hop from 'levelNum' level to 0th level. 'queryCount' is the count of probe at maps
    int queryNextHopByBFFromLevelNum(const string &ip, int levelNum, int& queryCount);

    //Longest Prefix Matching with Pruning
    int queryNextHopByBFPivotLevel(const string &ip, int pivotLevelNum, int& queryCount);

private:
    //if ip is 255.255.1.1 and levelNum is 24,return string like 255.255.1.1/24
    string formateIP(const string &ip, int levelNum);

    //reset all maps and bloom filters
    void resetMapsAndBloomFilters(std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs, int bfCount);

    //bulid all maps and bloom filters when trie is initiated or changed.
    void buildMapsAndBloomFilters(Node *root, std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs,int bfCount,int pivotLevelNum);

};




#endif // TRIE_H_INCLUDED
