// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// The Hamming distance between two numbers is defined as the
// number of positions in their binary representations at 
// which they
// differ (leading zeros are used if necessary to make the 
// binary
// representations have the same length) - e.g., the numbers 
// "11010"
// and "01100" differ at the first, third and fourth 
// positions, so
// they have a Hamming distance of 3.
// 
// 
// You will be given a vector <string> numbers containing
// the binary representations of some numbers (all having the 
// same
// length). You are to return the minimum among the Hamming 
// distances
// of all pairs of the given numbers.
// 
// 
// 
// DEFINITION
// Class:HammingDistance
// Method:minDistance
// Parameters:vector <string>
// Returns:int
// Method signature:int minDistance(vector <string> numbers)
// 
// 
// CONSTRAINTS
// -numbers will have between 2 and 50 elements, inclusive.
// -Each element of numbers will have between 1 and 50 
// characters, inclusive.
// -All elements of numbers will have the same number of 
// characters.
// -All elements of numbers will only contain the characters 
// '0' and '1'.
// 
// 
// EXAMPLES
// 
// 0)
// {"11010", "01100"}
// 
// Returns: 3
// 
// 
// The example from the problem statement.
// 
// 
// 1)
// {"00", "01", "10", "11"}
// 
// Returns: 1
// 
// 
// A binary code that uses all possible codewords has minimum 
// Hamming distance 1.
// 
// 
// 2)
// {"000", "011", "101", "110"}
// 
// Returns: 2
// 
// 
// Adding a "parity bit" to the binary numbers of example 1 
// increases the minimum 
// Hamming distance to 2.
// 
// 
// 
// 3)
// {"01100", "01100", "10011"}
// 
// Returns: 0
// 
// 
// Note that the input may contain identical numbers (Hamming 
// distance 0).
// 
// 
// 
// 4)
// {"00000000000000000000000000000000000000000000000000",
// "11111111111111111111111111111111111111111111111111"}
// 
// Returns: 50
// 
// 5)
// {"000000", "000111", "111000", "111111"}
// 
// Returns: 3
// 
// END CUT HERE
#line 100 "HammingDistance.cpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <valarray>
#include <numeric>
#include <functional>
#include <algorithm>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
#define SZ(v) ((int)(v).size())

class HammingDistance
{ 
public: 
	int minDistance(vector <string> num) 
	{ 
		int re = SZ(num.front());
		REP(i,SZ(num)-1) FOR(j,i+1,SZ(num)) {
			int d = 0;
			REP(k,SZ(num[i])) if( num[i][k] != num[j][k] ) d ++;
			re = min(re,d);
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"11010", "01100"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; verify_case(0, Arg1, minDistance(Arg0)); }
	void test_case_1() { string Arr0[] = {"00", "01", "10", "11"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; verify_case(1, Arg1, minDistance(Arg0)); }
	void test_case_2() { string Arr0[] = {"000", "011", "101", "110"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; verify_case(2, Arg1, minDistance(Arg0)); }
	void test_case_3() { string Arr0[] = {"01100", "01100", "10011"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; verify_case(3, Arg1, minDistance(Arg0)); }
	void test_case_4() { string Arr0[] = {"00000000000000000000000000000000000000000000000000",
"11111111111111111111111111111111111111111111111111"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 50; verify_case(4, Arg1, minDistance(Arg0)); }
	void test_case_5() { string Arr0[] = {"000000", "000111", "111000", "111111"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; verify_case(5, Arg1, minDistance(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	HammingDistance ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
