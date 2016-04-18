#ifndef RANDOMIPTESTMETHOD_H_INCLUDED
#define RANDOMIPTESTMETHOD_H_INCLUDED

#include "Trie.h"
#include <iostream>
#include <fstream>


char * int2ipstr (int ip);


void randomIpTest(string route_table_file, string query_file, int pivotlevel)
{
	printf("\n\n algorithm starts...\n\n");

	ofstream PL_probeNum("../result/RandomIp_PL_probeNum.txt");

    if(PL_probeNum.is_open())
    {
        PL_probeNum << "bfHashCount"<<"\t"<< "pivot_level_num"<<"\t"<<"probeNum\n";
    }

    ofstream methods_compare("../result/RandomIp_methods_compare.txt");
    if(methods_compare.is_open())
    {
        methods_compare << "bfHashCount"<<"\t"<< "pivot_level_num"<<"\t"<<"dirctly probe count"<<"\t"<<"PBF probe probe"<<"\t"<<"Pruning probe count\n";
    }

    ofstream BF_count_change("../result/RandomIp_BF_count_change.txt");
    if(BF_count_change.is_open())
    {
        BF_count_change << "bfHashCount"<<"\t"<< "pivot_level_num"<<"\t"<<"the count of solid nodes before pivot"<<"\t"<<"the total length of bloom filters for solid nodes before pivot"<<"\t"<<"the count of solid nodes after pivot"<<"\t"
        <<"the total length of bloom filters for solid nodes after pivot"<<"\t"<<"the count of empty nodes"<<"\t"<<"the length of bloom filter for empty nodes"<<endl;
    }

    ofstream BF_count_at_every_level("../result/RandomIp_BF_count_at_every_level.txt");
    if(BF_count_at_every_level.is_open())
    {
        BF_count_at_every_level<< "bfHashCount"<<"\t"<<"pivot_level_num\t";
        for(int i = 0; i < LEVELCOUNT; i++)
        {
            BF_count_at_every_level <<i<<"th ceng\t";
        }
        BF_count_at_every_level<<"emptyCount"<<endl;
    }

    ofstream hit_count_at_every_level("../result/RandomIp_hit_count_at_every_level.txt");
    if(hit_count_at_every_level.is_open())
    {
        hit_count_at_every_level<< "bfHashCount"<<"\t"<<"pivot_level_num\t";
        for(int i = 0; i < LEVELCOUNT; i++)
        {
            hit_count_at_every_level <<i<<"th ceng\t";
        }
        hit_count_at_every_level<<endl;
    }

    ofstream probe_count_at_every_level("../result/RandomIp_probe_count_at_every_level.txt");
    if(probe_count_at_every_level.is_open())
    {
        probe_count_at_every_level<< "bfHashCount"<<"\t"<<"pivot_level_num\t";
        for(int i = 0; i < LEVELCOUNT; i++)
        {
            probe_count_at_every_level <<i<<"th ceng\t";
        }
        probe_count_at_every_level<<endl;
    }


    bool flag_first_methed = true;
    bool flag_second_methed = true;
    bool flag_third_methed = true;


    int queryCount1 = 0;
    int emptyCount1 = 0;


    int queryCount2 = 0;
    int emptyCount2 = 0;


    int queryCount3 = 0;
    int emptyCount3 = 0;


    int levelFrom = LEVELCOUNT - 1;


    for(unsigned bfhashcount = 2;bfhashcount <= 16; bfhashcount ++)
    {

        cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>> bfhashcount is "<<bfhashcount<<endl<<endl<<endl;


		Trie trie = Trie(pivotlevel, bfhashcount, route_table_file, query_file);
		trie.buildTrieFromFile(trie.route_table_file, trie.rootNode);

		trie.getNodesCounts();
		cout<<"allCount: "<<trie.allNodeCount<<"  solidCount: "<<trie.solidNodeCount<<endl;


		cout<<"resetThenBuildMapsAndBloomFilters start"<<endl;
		trie.resetThenBuildMapsAndBloomFilters(trie.rootNode, trie.unodMap, LEVELCOUNT ,trie.bloomFilter, BFCOUNT, 0);
		cout<<"resetThenBuildMapsAndBloomFilters end"<<endl;




		//unsigned int iNextHop;

		string filename_query = trie.query_file;


		queryCount1 = 0;
		emptyCount1 = 0;

		unsigned int first_count_line = 0;

		if(flag_first_methed)
		{
			ifstream fin1(filename_query.c_str());
			while (!fin1.eof()) {
				unsigned int intIp;
				//iNextHop = 0;
				fin1 >>intIp;

				char *charIp = int2ipstr(intIp);

				string ipQueryStr = charIp;

				free(charIp);

				int result = trie.queryNextHopFromLevelNum(ipQueryStr, levelFrom, queryCount1);

				if(EMPTYHOP == result)
				{
					//cout<<ipQueryStr<<" next hop is empty"<<endl;
					emptyCount1++;
				}

				first_count_line++;

				if(first_count_line >= MAX_LINE_QUERY)
				{
					 break;
				}

			}
			fin1.close();

			cout<<"query by levelFrom "<< levelFrom << "  queryCount1: " << queryCount1<<" emptyCount1 is " << emptyCount1<< endl;
		}

		methods_compare <<trie.bf_hash_count<<"\t"<< trie.pivotLevelNum << "\t";
		methods_compare << queryCount1 << "\t";

		unsigned int tempCount = 0;
		unsigned int tempBFSolidCount = 0;

		BF_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int temp=0; temp < BFCOUNT - 1; temp++)
		{
			tempCount += trie.elementCount[temp];

			tempBFSolidCount += trie.bf_hash_count/Ln2 * trie.elementCount[temp];

			BF_count_at_every_level<<trie.elementCount[temp] << "\t";
		}

		BF_count_at_every_level << endl;

		BF_count_change <<trie.bf_hash_count<<"\t"<< trie.pivotLevelNum << "\t";
		BF_count_change << tempCount << "\t";
		BF_count_change << tempBFSolidCount << "\t";

		tempCount = 0;
		tempBFSolidCount = 0;
		trie.reset_hit_and_probe_count();

		cout<<"\n\n----------------first--method------------------------end-----"<<endl;


		queryCount2 = 0;
		emptyCount2 = 0;

		unsigned int second_count_line = 0;
		if(flag_second_methed)
		{
			ifstream fin2(filename_query.c_str());
			while (!fin2.eof()) {
				unsigned int intIp;
				//iNextHop = 0;
				fin2 >>intIp;

				char *charIp = int2ipstr(intIp);

				string ipQueryStr = charIp;

				free(charIp);

				int result = trie.queryNextHopByBFFromLevelNum(ipQueryStr, levelFrom, queryCount2);

				if(EMPTYHOP == result)
				{
					//cout<<ipQueryStr<<" next hop is empty"<<endl;
					emptyCount2++;
				}

				second_count_line++;
				if(second_count_line >= MAX_LINE_QUERY)
				{
					cout<< "second_count_line->" << second_count_line;
					break;
				}


			}
			fin2.close();

			cout<<"Query by bloom filter levelFrom "<< levelFrom  << "  queryCount2: " << queryCount2 <<"  emptyCount2 is " <<emptyCount2<< endl;
		}



		methods_compare << queryCount2 << "\t";


		hit_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int i = 0; i < LEVELCOUNT; i++)
		{
			hit_count_at_every_level << trie.hit_count[i] << "\t";
		}
		hit_count_at_every_level << endl;


		probe_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int i = 0; i < LEVELCOUNT; i++)
		{
			probe_count_at_every_level << trie.probe_count[i] << "\t";
		}
		probe_count_at_every_level << endl;


		cout<<"\n\n----------------second--method-----------------------end------"<<endl;

		cout<<"begin pivotLevel"<<endl;
		trie.pivotLevel(trie.rootNode, trie.pivotLevelNum);

		trie.getNodesCounts();
		cout<<"allCount: "<<trie.allNodeCount<<"  solidCount: "<<trie.solidNodeCount<<endl;

		cout<<"resetThenBuildMapsAndBloomFilters start"<<endl;
		trie.resetThenBuildMapsAndBloomFilters(trie.rootNode, trie.unodMap, LEVELCOUNT ,trie.bloomFilter, BFCOUNT, trie.pivotLevelNum);
		cout<<"resetThenBuildMapsAndBloomFilters end"<<endl;



		queryCount3 = 0;
		emptyCount3 = 0;

		unsigned int third_count_line = 0;

		if(flag_third_methed)
		{
			ifstream fin3(filename_query.c_str());
			trie.reset_hit_and_probe_count();

			while (!fin3.eof()) {

				unsigned int intIp;
				//iNextHop = 0;
				fin3 >>intIp;

				char *charIp = int2ipstr(intIp);

				string ipQueryStr = charIp;

				free(charIp);



				int result = trie.queryNextHopByBFPivotLevel(ipQueryStr, trie.pivotLevelNum, queryCount3);

				if(EMPTYHOP == result)
				{
					//cout<<ipQueryStr<<" next hop is empty"<<endl;
					emptyCount3++;
				}

				third_count_line++;
				if(third_count_line >= MAX_LINE_QUERY)
				{
					 break;
				}

			}
			fin3.close();
		}
		cout<<"query by PivotLevel, trie.pivotLevelNum is "<< trie.pivotLevelNum << "  queryCount3: " << queryCount3<<"  emptyCount3 is "<<emptyCount3<< endl;




		hit_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int i = 0; i < LEVELCOUNT; i++)
		{
			hit_count_at_every_level << trie.hit_count[i] << "\t";
		}
		hit_count_at_every_level << endl;



		probe_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int i = 0; i < LEVELCOUNT; i++)
		{
			probe_count_at_every_level << trie.probe_count[i] << "\t";
		}
		probe_count_at_every_level << endl;


		PL_probeNum<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t"<<queryCount3<<endl;
		methods_compare << queryCount3 << endl;

		BF_count_at_every_level<<trie.bf_hash_count<<"\t"<<trie.pivotLevelNum<<"\t";
		for(int temp=0; temp < BFCOUNT - 1; temp++)
		{
			tempCount += trie.elementCount[temp];
			tempBFSolidCount += trie.bf_hash_count/Ln2 * trie.elementCount[temp];

			 BF_count_at_every_level<<trie.elementCount[temp] << "\t";
		}

		BF_count_change << tempCount << "\t";
		BF_count_change << tempBFSolidCount << "\t";

		BF_count_change << trie.elementCount[BFCOUNT - 1] << "\t";
		BF_count_at_every_level << trie.elementCount[BFCOUNT - 1] << endl;

		unsigned int tempBFEmptyCount = trie.elementCount[BFCOUNT - 1] * trie.bf_hash_count/Ln2;

		BF_count_change << tempBFEmptyCount << endl;

		cout<<"\n\n----------------third--method------------------------end---------"<<endl;

		trie.trieDestroy(trie.rootNode);

    }

    PL_probeNum.close();
    methods_compare.close();
    BF_count_change.close();
}


char * int2ipstr (int ip)
{
    char *buf = (char *)malloc(16*sizeof(char));
    sprintf (buf, "%u.%u.%u.%u",
    (uchar) * ((char *) &ip + 3),
    (uchar) * ((char *) &ip + 2),
    (uchar) * ((char *) &ip + 1), (uchar) * ((char *) &ip + 0));
    return buf;
}

#endif // RANDOMIPTESTMETHOD_H_INCLUDED
