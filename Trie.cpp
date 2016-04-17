#include "Trie.h"
#include <iostream>
#include<sstream>


#define NODELEN				sizeof(struct Node)		//size of each Trie node
#define HIGHTBIT			2147483648					//Binary: 1000 0000 0000 0000 0000 0000 0000 0000


Trie::Trie(unsigned int pivotLevelNumber, unsigned int bfhashcount,string route_table, string queryfile)
{
    createNewNode(rootNode);
    allNodeCount = 0;
    solidNodeCount = 0;

    pivotLevelNum = pivotLevelNumber;
    bf_hash_count = bfhashcount;

	route_table_file = route_table;
	query_file = queryfile;

    memory_access = 0;
    unodMap = new std::tr1::unordered_map<std::string,uint32_t>[LEVELCOUNT];

    for(int i = 0; i < BFCOUNT; ++i){
        bloomFilter[i] = NULL;
        elementCount[i] = 0;
    }

    for(int i = 0; i < LEVELCOUNT; ++i)
    {
        hit_count[i] = 0;

        probe_count[i] = 0;
    }
}


Trie::~Trie()
{
    cout << "\tDestroying the trie......" << endl;
    if(rootNode == NULL)
    {
        trieDestroy(rootNode);
    }

	cout << "\tThe trie has been destroyed!" << endl;
	delete[] unodMap;
    for(int i = 0; i < BFCOUNT; ++i){
        if(bloomFilter[i] != NULL){
			delete bloomFilter[i];
			bloomFilter[i] = NULL;
		}
    }
}

void Trie::createNewNode(Node* &node)
{
    node = (struct Node*)malloc(NODELEN);

    node->parent = NULL;
    node->lchild = NULL;
    node->rchild = NULL;
    node->prefix = (char*)malloc(1*sizeof(char));
	node->prefix[0] = '\0'; 
    node->nodeLevel = 0;
    node->next_hop = EMPTYHOP;

}

char* Trie::ipToLongThenToChar(const string &ip, int &iPrefixLen)
{
    const char* ipArray = ip.c_str();
    int iLen=(int)strlen(ipArray);

    unsigned long lPrefix =0;
    int iStart=0;				//the start point of PREFIX
    int iEnd=0;
    int iFieldIndex = 3;

    for (int i=0; i<iLen; i++ )
    {
        //get the first three sub-items
        if ( ipArray[i] == '.' ) {
            iEnd = i;
            string strVal(ipArray+iStart,iEnd-iStart);
            lPrefix += atol(strVal.c_str()) << (8 * iFieldIndex);
            iFieldIndex--;
            iStart = i+1;
            i++;
        }
        if ( ipArray[i] == '/' )
        {
            //get the prefix length
            iEnd = i;
            string strVal(ipArray+iStart,iEnd-iStart);
            lPrefix += atol(strVal.c_str());
            iStart = i+1;

            i++;

            strVal= string(ipArray+iStart,iLen-1);
            iPrefixLen=atoi(strVal.c_str());
        }

    }

    char *insert_B = (char *)malloc(36*sizeof(char));
    memset(insert_B,0,sizeof(insert_B));
    //insert the current node into Trie tree
    for (unsigned int yi = 0; yi < iPrefixLen; yi++)
    {
        //turn right
        if(((lPrefix << yi) & HIGHTBIT) == HIGHTBIT)
            insert_B[yi]='1';
        else insert_B[yi]='0';
    }
    insert_B[iPrefixLen] = '\0';

    return insert_B;

}


unsigned int Trie::buildTrieFromFile(string FileName, Node * root)
{

	unsigned int iNextHop;

	ifstream fin(FileName.c_str());

	while (!fin.eof()) {
        string Prefix;
		iNextHop = EMPTYHOP;
		fin >> Prefix >> iNextHop;

        if(Prefix.empty())
            continue;

		int iLen = -1;
        char * sPrefix = ipToLongThenToChar(Prefix, iLen);
		Node * current = root;


		if (iLen == 0) {
			if (!iNextHop)
				continue;
			root->next_hop = iNextHop;
		}

		for (int i = 0; i < iLen; i++) {
			if (sPrefix[i] == '0') {
				if (current->lchild != NULL) {
					current = current->lchild;
				} else {
					Node * tmp;
					createNewNode(tmp);
					tmp->nodeLevel = i + 1;
					current->lchild = tmp;
					tmp->parent = current;

                    if(NULL != tmp->prefix)
                    {
                        free(tmp->prefix);
                    }
                    tmp->prefix = (char *)malloc((i+2)*sizeof(char));
                    memcpy(tmp->prefix, sPrefix, i+1);
                    tmp->prefix[i+1] = '\0';

					current = tmp;
				}
			} else {
				if (current->rchild != NULL) {
					current = current->rchild;
				} else {
					Node * tmp;
					createNewNode(tmp);
					tmp->nodeLevel = i + 1;
					tmp->parent = current;
					current->rchild = tmp;

                    if(NULL != tmp->prefix)
                    {
                        free(tmp->prefix);
                    }
                    tmp->prefix = (char *)malloc((i+2)*sizeof(char));
					memcpy(tmp->prefix, sPrefix, i+1);
                    tmp->prefix[i+1] = '\0';

					current = tmp;
				}
			}
		}
		current->next_hop = iNextHop;

		free(sPrefix);
		//printf("Prefix = %s,  iLen = %d, iNextHop = %d \n",Prefix.c_str(),iLen,iNextHop);

	}
	fin.close();
	printf("buildTrieFromFile\n");
	return 0;
}




void Trie::trieDestroy(Node *root)
{
	if (root == NULL)
		return;

	trieDestroy(root->lchild);
	trieDestroy(root->rchild);

    if(root->prefix != NULL){
        free(root->prefix);
        root->prefix = NULL;
    }
	free(root);
	root = NULL;
}


bool Trie::isLeaf(Node* node)
{
    if(node->lchild == NULL && node->rchild == NULL)
    {
        return true;
    }

    return false;
}

void Trie::pretraversal(Node* root)
{
    if(NULL == root) return;

    allNodeCount++;

 //   cout<< root->prefix<<endl;

    if(root->next_hop != EMPTYHOP)
    {
        solidNodeCount++;
        elementCount[root->nodeLevel] = elementCount[root->nodeLevel] + 1;
    }

    if(root->nodeLevel == pivotLevelNum && EMPTYHOP == root->next_hop)
    {
        elementCount[BFCOUNT - 1] = elementCount[BFCOUNT - 1] + 1;
    }

    pretraversal(root->lchild);
    pretraversal(root->rchild);
}


unsigned int Trie::GetAncestorHop(Node* node)
{
    if(NULL == node)
    {
        return EMPTYHOP;
    }

    if(EMPTYHOP != node->next_hop)
    {
        return node->next_hop;
    }

    return GetAncestorHop(node->parent);
}

void Trie::pivotLevel(Node *root, int levelNum)
{
    if(NULL == root)
        return;

    if(root->nodeLevel > levelNum)
        return;

    if(root->nodeLevel == levelNum)
    {
        if(EMPTYHOP == root->next_hop)
        {
            root->next_hop = GetAncestorHop(root);
        }

        if(EMPTYHOP != root->next_hop)
        {
            if(NULL != root->lchild && NULL == root->rchild)
            {
                if(EMPTYHOP == root->lchild->next_hop)
                {
                    root->lchild->next_hop = root->next_hop;
                }

                Node * tmp;
                createNewNode(tmp);
                tmp->nodeLevel = root->nodeLevel + 1;
                root->rchild = tmp;
                tmp->parent = root;

                if(NULL != tmp->prefix)
                {
                    free(tmp->prefix);
                }
                tmp->prefix = (char *)malloc((strlen(root->prefix)+2)*sizeof(char));
                memcpy(tmp->prefix, root->prefix, strlen(root->prefix));
                tmp->prefix[strlen(root->prefix)] = '1';
                tmp->prefix[strlen(root->prefix) + 1] = '\0';


                tmp->next_hop = root->next_hop;
                root->next_hop = EMPTYHOP;
            }

            else if(NULL == root->lchild && NULL != root->rchild)
            {
                if(EMPTYHOP == root->rchild->next_hop)
                {
                    root->rchild->next_hop = root->next_hop;
                }

                Node * tmp;
                createNewNode(tmp);
                tmp->nodeLevel = root->nodeLevel + 1;
                root->lchild = tmp;
                tmp->parent = root;

                if(NULL != tmp->prefix)
                {
                    free(tmp->prefix);
                }
                tmp->prefix = (char *)malloc((strlen(root->prefix)+2)*sizeof(char));
                memcpy(tmp->prefix, root->prefix, strlen(root->prefix));
                tmp->prefix[strlen(root->prefix)] = '0';
                tmp->prefix[strlen(root->prefix) + 1] = '\0';

                tmp->next_hop = root->next_hop;
                root->next_hop = EMPTYHOP;
            }

            else if(NULL != root->lchild && NULL != root->rchild)
            {

                if(EMPTYHOP == root->lchild->next_hop)
                {
                    root->lchild->next_hop = root->next_hop;
                }

                if(EMPTYHOP == root->rchild->next_hop)
                {
                    root->rchild->next_hop = root->next_hop;
                }


                root->next_hop = EMPTYHOP;
            }
            else
            {
                //when left and right all are empty,do nothing
            }
        }
    }


    pivotLevel(root->lchild, levelNum);
    pivotLevel(root->rchild, levelNum);
}

void Trie::initBloomFilters()
{
    for(int i = 0; i < BFCOUNT; i++)
    {
        if(NULL != bloomFilter[i])
        {
           delete bloomFilter[i];
           bloomFilter[i] = NULL;
        }

          unsigned int bf_length = bf_hash_count/Ln2 * elementCount[i];  // from formula k=(m/n)* ln2

          if(elementCount[i] > 0 && elementCount[i] < 50)
          {
              bf_length = bf_hash_count/Ln2 * 50;
          }



 //       cout<< i << " ----> elementCount  " << elementCount[i]  <<" ------> bf_length  " << bf_length <<endl;

        if(0 != bf_length)
        {
            if(i < BFCOUNT - 1) //init bloom filter to check empty nodes at pivot level.
            {
                bloomFilter[i] = new StandardBF(bf_length, bf_hash_count);
            }
            else if(i == BFCOUNT - 1)
            {
                bloomFilter[i] = new StandardBF(bf_length , bf_hash_count);
            }
        }



    }

    //at pivot level, there must be having a bloom filter to check solid nodes.
    if(0 == elementCount[pivotLevelNum])
    {
        bloomFilter[pivotLevelNum] = new StandardBF(26, bf_hash_count);
    }

    //at pivot level, there must be having a bloom filter to check empty nodes.
    if(0 == elementCount[BFCOUNT - 1])
    {
        bloomFilter[BFCOUNT - 1] = new StandardBF(26, bf_hash_count);
    }
}


void Trie::reset_hit_and_probe_count()
{
    for(int i = 0; i < LEVELCOUNT; i ++)
    {
        hit_count[i] = 0;

        probe_count[i] = 0;
    }
}




void Trie::getNodesCounts()
{

    for(int i = 0; i < BFCOUNT; i++)
    {
        elementCount[i] = 0;
    }

    allNodeCount = 0;
    solidNodeCount = 0;
    pretraversal(rootNode);


    initBloomFilters();

}

void Trie::resetMapsAndBloomFilters(std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs, int bfCount)
{
    if(NULL == maps || NULL == bfs)
    {
        return;
    }

    for(int i = 0; i < mapCount; i++)
    {
        maps[i].clear();
    }

    for(int i = 0; i < bfCount; i++)
    {
        if(0 == elementCount[i])
        {
            continue;
        }

        bfs[i]->reset();
    }
}

void Trie::buildMapsAndBloomFilters(Node *root, std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs, int bfCount ,int pivotLevelNum)
{
    if(NULL == root)
    {
        return;
    }

    if(EMPTYHOP != root->next_hop)
    {
        maps[root->nodeLevel][root->prefix] = root->next_hop;
        bfs[root->nodeLevel]->insert((unsigned char *)root->prefix, strlen(root->prefix));
    }

    if(root->nodeLevel == pivotLevelNum && EMPTYHOP == root->next_hop)
    {
        bfs[bfCount - 1]->insert((unsigned char *)root->prefix, strlen(root->prefix));
    }

    buildMapsAndBloomFilters(root->lchild, maps, mapCount, bfs, bfCount, pivotLevelNum);
    buildMapsAndBloomFilters(root->rchild, maps, mapCount, bfs, bfCount, pivotLevelNum);
}

void Trie::resetThenBuildMapsAndBloomFilters(Node *root, std::tr1::unordered_map<std::string,uint32_t>* maps, int mapCount, StandardBF **bfs,int bfCount,int pivotLevelNum)
{
    //reset first.
    resetMapsAndBloomFilters(maps, mapCount, bfs, bfCount);
    //then rebulid
    buildMapsAndBloomFilters(root, maps, mapCount, bfs, bfCount, pivotLevelNum);
}



string Trie::formateIP(const string &ip, int levelNum)
{
    ostringstream num;
    num<<levelNum;
    string tempIp = ip;
    tempIp.append("/");
    tempIp.append(num.str());

    return tempIp;
}



int Trie::queryNextHopFromLevelNum(const string &ip, int levelNum, int& queryCount)
{
    if(levelNum > 32)
    {
        cout<<"levelNum can not bigger than 32"<<endl;
        return -1;
    }
    for(int i = levelNum; i >= 0; i--)
    {
        queryCount++;

        string tempIp = formateIP(ip,i);

        int len;
        char * ipstr = ipToLongThenToChar(tempIp, len);

        if(unodMap[i].find(ipstr) != unodMap[i].end())
        {
            int result = unodMap[i][ipstr];
            free(ipstr);
            return result;
        }
        free(ipstr);
    }
    return EMPTYHOP;
}



int Trie::queryNextHopAtLevelNum(const string &ip, int levelNum)
{
    if(levelNum > 32)
    {
        cout<<"levelNum can not bigger than 32"<<endl;
        return -1;
    }

    string tempIp = formateIP(ip, levelNum);

    int len;
    char * ipstr = ipToLongThenToChar(tempIp, len);


    if(unodMap[levelNum].find(ipstr) != unodMap[levelNum].end())
    {
        int result = unodMap[levelNum][ipstr];
        free(ipstr);
        return result;
    }

    free(ipstr);

    return EMPTYHOP;
}

int Trie::queryNextHopByBFFromLevel1ToLevel2(const string &ip, int levelNumFrom, int levelNumTo, int& queryCount)
{
    if(levelNumFrom > 32)
    {
        cout<<"levelNumFrom can not bigger than 32"<<endl;
        return -1;
    }

    if(levelNumTo < 0)
    {
        cout<<"levelNumTo can not less than 0"<<endl;
        return -1;
    }

    int result = EMPTYHOP;

    for(int i = levelNumFrom; i >= 0; i--)
    {
        if(0 == elementCount[i])
        {
            continue;
        }


        if(i < levelNumTo)
        {
            return result;
        }

        string tempIp = formateIP(ip, i);

        int len;
        char * ipstr = ipToLongThenToChar(tempIp, len);

        int bfQueryResult = bloomFilter[i]->query((unsigned char *)ipstr, strlen(ipstr));

        if(0 == bfQueryResult)
        {
            free(ipstr);
            continue;
        }

        else if(1 == bfQueryResult)
        {
            queryCount++;

            probe_count[i]++;

            result = queryNextHopAtLevelNum(ip, i);
            if(EMPTYHOP == result)
            {
                free(ipstr);
                continue;
            }
            else
            {

                hit_count[i]++;

                free(ipstr);
                return result;
            }
        }

        free(ipstr);
    }

    return result;
}


int Trie::queryNextHopByBFFromLevelNum(const string &ip, int levelNum, int& queryCount)
{
    return queryNextHopByBFFromLevel1ToLevel2(ip, levelNum, 0, queryCount);
}

int Trie::queryNextHopByBFPivotLevel(const string &ip, int pivotLevelNum, int& queryCount)
{
    if(pivotLevelNum > 31)
    {
        cout<<"pivotLevelNum can not bigger than 31"<<endl;
        return -1;
    }

    if(pivotLevelNum < 0)
    {
        cout<<"pivotLevelNum can not less than 0"<<endl;
        return -1;
    }

    int result = EMPTYHOP;

    // Transform a ip to the form as "255.255.1.1/4"
    string tempIp = formateIP(ip, pivotLevelNum);

    int len;
    char * ipstr = ipToLongThenToChar(tempIp, len);

    //The last bloom filter is used to check empty nodes at pivot level
    int bfQueryEmptyResult = bloomFilter[BFCOUNT -1]->query((unsigned char *)ipstr, strlen(ipstr));
    if(0 == bfQueryEmptyResult)
    {
        result = queryNextHopByBFFromLevel1ToLevel2(ip, pivotLevelNum, 0, queryCount);

        free(ipstr);
        return result;
    }

    else
    {
        int bfQuerySolidResult = bloomFilter[pivotLevelNum]->query((unsigned char *)ipstr, strlen(ipstr));
        if(0 == bfQuerySolidResult)
        {
            result = queryNextHopByBFFromLevel1ToLevel2(ip, LEVELCOUNT -1, pivotLevelNum +1, queryCount);
            if(EMPTYHOP == result)
            {
                result = queryNextHopByBFFromLevel1ToLevel2(ip, pivotLevelNum - 1, 0, queryCount);
            }
        }
        else
        {
            result = queryNextHopByBFFromLevel1ToLevel2(ip, LEVELCOUNT -1, 0, queryCount);
        }

        free(ipstr);
        return result;
    }
}

