// BEGIN CUT HERE
// PROBLEM STATEMENT
// The organization of an array A is computed as follows: 
// Create an array B containing the exact same elements as A, 
// but sorted in non-descending order.  Count the number of 
// distinct values for i such that A[i] is equal to B[i].  
// This value is the organization of the array A. For 
// example, the organization of { 2, 1, 1, 3 } is 2 because 
// the second and fourth elements are not changed after 
// sorting.
// Two elements may be swapped only if the organization of 
// the array would increase as a result of the swap.
// You will be given a vector <int> arrayData. Return the 
// maximal number of the swap operations that can be performed.
// 
// 
// DEFINITION
// Class:SwapSorter
// Method:maximizeSwaps
// Parameters:vector <int>
// Returns:int
// Method signature:int maximizeSwaps(vector <int> arrayData)
// 
// 
// CONSTRAINTS
// -arrayData will have between 1 and 50 elements, inclusive. 
// -Each element of arrayData will be between 1 and 1000, 
// inclusive. 
// 
// 
// EXAMPLES
// 
// 0)
// {2, 1, 1, 3}
// 
// Returns: 1
// 
// The only possible swap is {2, 1, 1, 3} -> {1, 1, 2, 3}
// 
// 1)
// {7, 5, 3, 4}
// 
// Returns: 3
// 
// {7, 5, 3, 4} -> {3, 5, 7, 4} -> {3, 4, 7, 5} -> {3, 4, 5, 7}
// 
// 2)
// {2, 1, 4, 3}
// 
// Returns: 2
// 
// {2, 1, 4, 3} -> {1, 2, 4, 3} -> {1, 2, 3, 4}
// 
// 3)
// {1, 7, 8, 12, 17, 19, 21, 23, 24, 25, 26, 27, 35}
// 
// Returns: 0
// 
// The array is already sorted.
// 
// 4)
// {2, 3, 3, 1, 1, 2}
// 
// Returns: 5
// 
// 5)
// {2, 3, 4, 1, 7, 7, 5, 5, 8, 7, 10, 10, 10, 9, 9, 9}
// 
// Returns: 11
// 
#line 72 "SwapSorter.cpp"
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
#define REP(i,n) FOR(i,0,n)
#define ALL(v) (v).begin(),(v).end()


class SwapSorter
{ 
	vector<short> sorted;
	map< vector<short>, int> m;
	int solve( vector<short> &v){
		if( m.find( v) != m.end() ) return m[v];
		int re= 0;
		if( v != sorted ){
			REP(i,SZ(v)-1 ) if( v[i] != sorted[i] ){
				FOR(j,i+1,SZ(v)) if( v[j] != sorted[j] && v[j] != v[i]) {
					if( v[i] == sorted[j] || v[j] == sorted[i] ) {
						swap(v[i],v[j]);
						re = max(re, solve(v)+1);
						swap(v[i],v[j]);
					}
				}
			}
		}
		m[v] = re;
		return re;
	}
public: 
	int maximizeSwaps(vector <int> arrayData) 
	{
		m.clear();
		vector<short> v;
		REP(i,SZ(arrayData)) v.push_back(arrayData[i]);
		sorted = v;
		sort(ALL(sorted));
		return solve(v);
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {2, 1, 1, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; verify_case(0, Arg1, maximizeSwaps(Arg0)); }
	void test_case_1() { int Arr0[] = {7, 5, 3, 4}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; verify_case(1, Arg1, maximizeSwaps(Arg0)); }
	void test_case_2() { int Arr0[] = {2, 1, 4, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; verify_case(2, Arg1, maximizeSwaps(Arg0)); }
	void test_case_3() { int Arr0[] = {1, 7, 8, 12, 17, 19, 21, 23, 24, 25, 26, 27, 35}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; verify_case(3, Arg1, maximizeSwaps(Arg0)); }
	void test_case_4() { int Arr0[] = {2, 3, 3, 1, 1, 2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 5; verify_case(4, Arg1, maximizeSwaps(Arg0)); }
	void test_case_5() { int Arr0[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 25; verify_case(5, Arg1, maximizeSwaps(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	SwapSorter ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
