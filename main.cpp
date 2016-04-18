#include "randomIpTestMethod.h"
#include "fibIpTestMethod.h"

using namespace std;


int main()
{
    cout << "starting!" << endl;

	string randomIp_route_table_file = "../data4test/randomIP/route_table.tr";  //file path of route table to test random traffic
	string randomIp_query_file = "../data4test/randomIP/random_flow.tr";        //file path of random traffic
	int pivotLevel1 = 24;  // # of level to do pivot

    randomIpTest(randomIp_route_table_file, randomIp_query_file, pivotLevel1);//Random ip test

//------------------------------------------------------------------------------------------------------

	string fibIp_route_table_file = "../data4test/fibIp/route_table.tr";  //file path of route table to test prefix-based traffic
	string fibIp_query_file = "../data4test/fibIp/fib_flow.tr";           //file path of prefix-based traffic
	int pivotLevel2 = 24;  // # of level to do pivot

    fibIpTest(fibIp_route_table_file, fibIp_query_file, pivotLevel2);     //prefix-based traffic test

    return 0;
}
