// BEGIN CUT HERE
// PROBLEM STATEMENT
// You are given a string code containing a message composed 
// entirely of decimal digits ('0'-'9'). Each digit consists 
// of some number of dashes (see diagram below). A "check 
// function" of a message is defined as the total number of 
// dashes in the message. Return the value of the check 
// function for the message represented in code.
// 
// 
// 
// 
// DEFINITION
// Class:CheckFunction
// Method:newFunction
// Parameters:string
// Returns:int
// Method signature:int newFunction(string code)
// 
// 
// NOTES
// -0 consists of 6 dashes, 1 consists of 2 dashes, 2 
// consists of 5 dashes, 3 consists of 5 dashes, 4 consists 
// of 4 dashes, 5 consists of 5 dashes, 6 consists of 6 
// dashes, 7 consists of 3 dashes, 8 consists of 7 dashes, 9 
// consists of 6 dashes.
// 
// 
// CONSTRAINTS
// -code will contain between 1 and 50 characters, inclusive.
// -Each character in code will be a digit ('0'-'9').
// 
// 
// EXAMPLES
// 
// 0)
// "13579"
// 
// Returns: 21
// 
// 1 consists of 2 dashes;
// 3 consists of 5 dashes;
// 5 consists of 5 dashes;
// 7 consists of 3 dashes;
// 9 consists of 6 dashes;
// 2 + 5 + 5 + 3 + 6 = 21.
// 
// 1)
// "02468"
// 
// Returns: 28
// 
// 2)
// "73254370932875002027963295052175"
// 
// Returns: 157
// 
#line 59 "CheckFunction.cpp"
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

class CheckFunction
{ 
public: 
	int newFunction(string code) 
	{ 
		int re=0;
		REP(i,SZ(code)) switch(code[i]){
		case '0': re += 6;break;
		case '1': re += 2;break;
		case '2': re += 5;break;
		case '3': re += 5;break;
		case '4': re += 4;break;
		case '5': re += 5;break;
		case '6': re += 6;break;
		case '7': re += 3;break;
		case '8': re += 7;break;
		case '9': re += 6;break;
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "13579"; int Arg1 = 21; verify_case(0, Arg1, newFunction(Arg0)); }
	void test_case_1() { string Arg0 = "02468"; int Arg1 = 28; verify_case(1, Arg1, newFunction(Arg0)); }
	void test_case_2() { string Arg0 = "73254370932875002027963295052175"; int Arg1 = 157; verify_case(2, Arg1, newFunction(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	CheckFunction ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
