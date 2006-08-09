// BEGIN CUT HERE
// PROBLEM STATEMENT
// When a customer buys large quantities of a product, 
// frequently the seller will offer a volume discount.  For 
// instance, one unit might cost 10 dollars, but might be 
// offered in packages of 5 for 45 dollars.  In such a case, 
// it always makes sense buy the bulk lots to save money.  In 
// some other cases, however, it might not always make 
// sense.  Suppose a single unit were on sale for 8 dollars.  
// In such a case, purchasing single units would be less 
// expensive than purchasing a 5-pack.
// 
// You are given a vector <string> priceList describing the 
// number of units available in each bundle, and the cost of 
// the bundle.  Each element is of the form "units cost" 
// (quotes added for clarity).  You are also given an int 
// quantity, the number of units you wish to purchase.
// 
// Return an int indicating the best possible cost to 
// purchase at least the desired quantity of units.
// 
// DEFINITION
// Class:VolumeDiscount
// Method:bestDeal
// Parameters:vector <string>, int
// Returns:int
// Method signature:int bestDeal(vector <string> priceList, 
// int quantity)
// 
// 
// CONSTRAINTS
// -priceList will contain between 1 and 5 elements, inclusive.
// -Each element of priceList will be formatted as described 
// in the problem statement.
// -units will be an integer between 1 and 99, inclusive, 
// with no leading zeroes
// -cost will be an integer between 1 and 999, inclusive, 
// with no leading zeroes.  
// -No two values of units will be the same.
// -quantity will be between 1 and 99, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {"1 10", "5 45"}
// 10
// 
// Returns: 90
// 
// The first example suggested in the problem statement.
// 
// 1)
// {"1 8", "5 45"}
// 10
// 
// Returns: 80
// 
// The second example suggested in the problem statement.
// 
// 2)
// {"99 913", "97 173", "50 464", "80 565"} 	
// 18
// 
// Returns: 173
// 
// Here, every package has more units than we need, so we 
// pick the cheapest one.
// 
// 
// 3)
// {"2 272","1 166","10 993"}
// 81
// 
// Returns: 8110
// 
#line 78 "VolumeDiscount.cpp"
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


typedef vector<int> VI;
#define SZ(v) ((int)(v).size())
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)

typedef istringstream ISS;


class VolumeDiscount
{ 
	int N;
	VI units;
	VI costs;
public: 
	int minCost(int k, int q)
	{
		if( q<=0)return 0;
		if( k == N-1){
			return (q+units[k]-1)/units[k]*costs[k];
		}else{
			int m=999999;
			int c=q+units[k]-1/units[k];
			REP(i,c+1){
				m=min(m,i*costs[k]+minCost(k+1,q-units[k]*i));
			}
			return m;
		}
	}
	int bestDeal(vector <string> priceList, int quantity) 
	{
		N = SZ(priceList);
		costs = units =VI(N,0);
		REP(i,SZ(priceList)){
			ISS in(priceList[i]); in >> units[i] >> costs[i];
		}
		int i=max_element(ALL(units));
		if( i > 0)
		int re = minCost(0,quantity);
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"1 10", "5 45"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 10; int Arg2 = 90; verify_case(0, Arg2, bestDeal(Arg0, Arg1)); }
	void test_case_1() { string Arr0[] = {"1 8", "5 45"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 10; int Arg2 = 80; verify_case(1, Arg2, bestDeal(Arg0, Arg1)); }
	void test_case_2() { string Arr0[] = {"99 913", "97 173", "50 464", "80 565"} 	; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 18; int Arg2 = 173; verify_case(2, Arg2, bestDeal(Arg0, Arg1)); }
	void test_case_3() { string Arr0[] = {"1 1","2 2","3 3","4 4","5 5"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 99; int Arg2 = 99; verify_case(3, Arg2, bestDeal(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	VolumeDiscount ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
