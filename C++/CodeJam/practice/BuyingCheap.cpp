// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// Steve would like to buy a new car. He isn't wealthy, so he 
// would prefer a reasonably cheap car. The only problem is 
// that the quality of the cheapest cars is... let's say 
// questionable.
// 
// 
// 
// Thus Steve decided to make a list of car prices and to buy 
// a car with the third lowest price.
// 
// 
// 
// You will be given a vector <int> prices. 
// The same price may occur multiple times in prices, but it 
// should count only once in the ordering of available 
// prices. See Example 1 for further clarification.
// 
// 
// 
// Write a function that returns the third lowest price in 
// this list. If there are less than three different car 
// prices in prices, your method should return -1.
// 
// 
// DEFINITION
// Class:BuyingCheap
// Method:thirdBestPrice
// Parameters:vector <int>
// Returns:int
// Method signature:int thirdBestPrice(vector <int> prices)
// 
// 
// CONSTRAINTS
// -prices contains between 1 and 50 elements, inclusive.
// -Each element in prices will be between 1 and 1000, 
// inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {10, 40, 50, 20, 70,
//  80, 30, 90, 60}
// 
// Returns: 30
// 
// 1)
// {10, 10, 10, 10, 20,
//  20, 30, 30, 40, 40}
// 
// Returns: 30
// 
// The lowest price is 10, the second lowest is 20 and the 
// third lowest is 30.
// 
// 2)
// {10}
// 
// Returns: -1
// 
// 3)
// {80, 90, 80, 90, 80}
// 
// Returns: -1
// 
#line 70 "BuyingCheap.cpp"
// END CUT HERE

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <valarray>
#include <numeric>
#include <functional>
#include <algorithm>
using namespace std;

#define SZ(v) ((int)(v).size())
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define ALL(v) (v).begin(),(v).end()

class BuyingCheap
{ 
public: 
	int thirdBestPrice(vector <int> prices) 
	{ 
		int re = -1;
		int cnt=1;
		sort(ALL(prices));
		FOR(i,1,SZ(prices)) {
			if( prices[i] != prices[i-1] ) cnt++;
			if( cnt == 3 ) {
				re = prices[i];
				break;
			}
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {10, 40, 50, 20, 70,
 80, 30, 90, 60}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 30; verify_case(0, Arg1, thirdBestPrice(Arg0)); }
	void test_case_1() { int Arr0[] = {10, 10, 10, 10, 20,
 20, 30, 30, 40, 40}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 30; verify_case(1, Arg1, thirdBestPrice(Arg0)); }
	void test_case_2() { int Arr0[] = {10}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = -1; verify_case(2, Arg1, thirdBestPrice(Arg0)); }
	void test_case_3() { int Arr0[] = {80, 90, 80, 90, 80}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = -1; verify_case(3, Arg1, thirdBestPrice(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	BuyingCheap ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
