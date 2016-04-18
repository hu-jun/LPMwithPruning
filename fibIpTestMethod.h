#ifndef FIBIPTESTMETHOD_H_INCLUDED
#define FIBIPTESTMETHOD_H_INCLUDED

#include "Trie.h"
#include <iostream>
#include <fstream>


void fibIpTest(string route_table_file, string query_file, int pivotlevel, int hashcount)
{
	printf("\n\n algorithm starts...\n\n");

	ofstream resultFile("./result/result.txt");
	resultFile<<"ip\t"<<"next-hip\t"<<"longest matching prefix length\t"<<"probe times"<<endl;


	Trie trie = Trie(pivotlevel, hashcount, route_table_file, query_file);

    trie.buildTrieFromFile(trie.route_table_file, trie.rootNode);

    trie.getNodesCounts();
    cout<<"before pivot---->allCount: "<<trie.allNodeCount<<"  solidCount: "<<trie.solidNodeCount<<endl;


    string filename_query = trie.query_file;


    cout<<"begin pivotLevel"<<endl<<endl<<endl<<endl;
    trie.pivotLevel(trie.rootNode, trie.pivotLevelNum);

    trie.getNodesCounts();
    cout<<"after pivot---->allCount: "<<trie.allNodeCount<<"  solidCount: "<<trie.solidNodeCount<<endl;

    cout<<"resetThenBuildMapsAndBloomFilters start"<<endl;
    trie.resetThenBuildMapsAndBloomFilters(trie.rootNode, trie.unodMap, LEVELCOUNT ,trie.bloomFilter, BFCOUNT, trie.pivotLevelNum);
    cout<<"resetThenBuildMapsAndBloomFilters end"<<endl;



    int queryCount = 0;
    int emptyCount = 0;


    ifstream fin(filename_query.c_str());
    trie.reset_hit_and_probe_count();
    
    cout<<"\n\n\nrunning.";
	
	int tempCount = 0;	


    while (!fin.eof()) {

		tempCount++;
		if(0==tempCount%1000)
		{
			cout<<".";
		}


        string Prefix;
        //iNextHop = 0;
        fin >> Prefix;

        if(Prefix.empty())
            continue;

        int endIndex = Prefix.find_last_of("/");
        string ipQueryStr = Prefix.substr(0, endIndex);

        int temp = queryCount;

        int matchLength = 0;

        int resultPort = trie.queryNextHopByBFPivotLevel(ipQueryStr, trie.pivotLevelNum, queryCount,matchLength);

        if(EMPTYHOP == resultPort)
        {
            emptyCount++;
        }

        int probeTimes = queryCount - temp;

        resultFile<<ipQueryStr<<"\t"<<resultPort<<"\t"<<matchLength<<"\t"<<probeTimes<<endl;

    }
    fin.close();

    cout<<"\n\n\npivotLevelNum is "<< trie.pivotLevelNum<<" Bloom Filter hash function is " <<hashcount << "  queryCount: " << queryCount<<"  emptyCount3 is "<<emptyCount<< endl;


    cout<<"\n\n----------------query finished------------------------end---------"<<endl<<endl<<endl;

    if(0 != emptyCount)
    {
        cout<<"\n\n############# Attention please:  the result is not correct, ##########"<<endl;
        cout<<"############# because you have not defined a default next-hop in FIB file ####"<<endl;
        cout<<"############# you should add line as '1.0.1.0/0 26' in FIB file ########"<<endl<<endl<<endl;
    }

    resultFile.close();
    trie.trieDestroy(trie.rootNode);

}


#endif // FIBIPTESTMETHOD_H_INCLUDED
