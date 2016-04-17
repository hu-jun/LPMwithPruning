#include "randomIpTestMethod.h"
#include "fibIpTestMethod.h"

using namespace std;


int main()
{
    cout << "starting!" << endl;

	string randomIp_route_table_file = "../data4test/randomIP/route_table.tr";
	string randomIp_query_file = "../data4test/randomIP/random_flow.tr";

    randomIpTest(randomIp_route_table_file, randomIp_query_file);

	string fibIp_route_table_file = "../data4test/fibIp/route_table.tr";
	string fibIp_query_file = "../data4test/fibIp/fib_flow.tr";

    fibIpTest(fibIp_route_table_file, fibIp_query_file);     //fib ip test

    return 0;
}
