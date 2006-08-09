// BEGIN CUT HERE
// PROBLEM STATEMENT
// A series of brackets is complete if we can pair off each 
// left bracket '[' with a right bracket ']' that occurs 
// later in the series. Every bracket must participate in 
// exactly one such pair.
// 
// Given a string text add the minimal number of brackets to 
// the beginning and/or end of text to make it complete. 
// Return the result.
// 
// DEFINITION
// Class:CompletingBrackets
// Method:complete
// Parameters:string
// Returns:string
// Method signature:string complete(string text)
// 
// 
// CONSTRAINTS
// -text will have between 1 and 50 characters inclusive.
// -text will contain only the characters '[' and ']'.
// 
// 
// EXAMPLES
// 
// 0)
// "[["
// 
// Returns: "[[]]"
// 
// Add two ']' to the end to make this complete.
// 
// 1)
// "]["
// 
// Returns: "[][]"
// 
// Add one to the beginning and one to the end.
// 
// 2)
// "[[[[]]]]"
// 
// Returns: "[[[[]]]]"
// 
// This is already complete.
// 
#line 49 "CompletingBrackets.cpp"
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

class CompletingBrackets
{ 
	string drop(string text)
	{
		REP(i,SZ(text)-1){
			if( text[i] == '[' && text[i+1]==']' ){
				return text.substr(0,i)+text.substr(i+2,text.length()-i-2);
			}
		}
		return text;
	}
public: 
	string complete(string text) 
	{ 
		string re=text;
		while( drop(re).length() != re.length() ){
			re = drop(re);
		}
		REP(i,SZ(re)){
			if(re[i]==']') text = "["+text;
			else text+="]";
		}
		return text;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "[["; string Arg1 = "[[]]"; verify_case(0, Arg1, complete(Arg0)); }
	void test_case_1() { string Arg0 = "]["; string Arg1 = "[][]"; verify_case(1, Arg1, complete(Arg0)); }
	void test_case_2() { string Arg0 = "[[[[]]]]"; string Arg1 = "[[[[]]]]"; verify_case(2, Arg1, complete(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	CompletingBrackets ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
