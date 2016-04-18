#include "fibIpTestMethod.h"

using namespace std;


int main()
{
    cout << "starting!" << endl;

	string fibIp_route_table_file = "./data4test/route_table.tr";  //file path of route table to test prefix-based traffic
	string fibIp_query_file = "./data4test/fib_flow.tr";           //file path of prefix-based traffic
	int pivotLevel2 = 24;  // # of level to do pivot
	int hashcount = 6;     //bloom filter use # hash functions

    fibIpTest(fibIp_route_table_file, fibIp_query_file, pivotLevel2, hashcount);     //traffic test

    return 0;
}
