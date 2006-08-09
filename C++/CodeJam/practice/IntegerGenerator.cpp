// BEGIN CUT HERE
// PROBLEM STATEMENT
// As part of a larger scale project, you need to write a 
// component which generates consecutive positive integers. 
// Only certain digits may appear in the input and in the 
// integers generated, and leading zeros aren't allowed.
// 
// You are given a vector <int> allowed containing the list 
// of allowed digits, and a string current representing the 
// current integer. Return a string representing the first 
// integer larger than current composed only of digits in 
// allowed.
// 
// If current represents an invalid integer according to the 
// first paragraph, return "INVALID INPUT" (quotes for 
// clarity).
// 
// DEFINITION
// Class:IntegerGenerator
// Method:nextInteger
// Parameters:vector <int>, string
// Returns:string
// Method signature:string nextInteger(vector <int> allowed, 
// string current)
// 
// 
// CONSTRAINTS
// -allowed will contain between 0 and 10 elements, inclusive.
// -Each element in allowed will be between 0 and 9, inclusive.
// -allowed will contain no duplicates.
// -current will contain between 1 and 10 digits ('0'-'9'), 
// inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
// "16"
// 
// Returns: "17"
// 
// With all digits available, the next number is 17.
// 
// 
// 1)
// { 0, 1, 2, 3, 4, 5, 6, 8, 9 }
// "16"
// 
// Returns: "18"
// 
// The digit 7 is no longer allowed, so the next smallest 
// valid integer is 18.
// 
// 2)
// { 3, 5, 8 }
// "548"
// 
// Returns: "INVALID INPUT"
// 
// The current number may not contain disallowed digits.
// 
// 3)
// { 5, 3, 4 }
// "033"
// 
// Returns: "INVALID INPUT"
// 
// Leading zeros aren't allowed either.
// 
// 4)
// { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }
// "999"
// 
// Returns: "1000"
// 
// 5)
// { 0, 1, 2, 3, 4, 5 }
// "0"
// 
// Returns: "INVALID INPUT"
// 
// The generator only works with positive integers.
// 
// 6)
// { 1 }
// "1"
// 
// Returns: "11"
// 
#line 92 "IntegerGenerator.cpp"
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

typedef stringstream SS;
typedef istringstream ISS;
typedef ostringstream OSS;
template<class U,class T> T cast(U x){T y; OSS a; a<<x; ISS b(a.str());b>>y;return y;}

class IntegerGenerator
{
	vector<int> allow;
	bool check(string c){
		REP(i,SZ(c)){
			if( find(ALL(allow), c[i]-'0') == allow.end() )
				return false;
		}
		return true;
	}
public: 
	string nextInteger(vector <int> allowed, string current) 
	{
		allow = allowed;
		if( current[0] == '0' || !check(current) ) return "INVALID INPUT";
		string c= cast<int,string>(cast<string,int>(current)+1);
		while( true ){
			REP(i,SZ(c)){
				if( find(ALL(allow), c[i]-'0') == allow.end() ){
					int l=c.length();
					c = cast<int,string>(cast<string,int>(c.substr(0,i+1))+1)
						+ ((l-i-1>0)?string('0',l-i-1):"");
					goto END;
				}
				return c;
			}
END:	;
		}
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "16"; string Arg2 = "17"; verify_case(0, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = { 0, 1, 2, 3, 4, 5, 6, 8, 9 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "16"; string Arg2 = "18"; verify_case(1, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = { 3, 5, 8 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "548"; string Arg2 = "INVALID INPUT"; verify_case(2, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = { 5, 3, 4 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "033"; string Arg2 = "INVALID INPUT"; verify_case(3, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_4() { int Arr0[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "999"; string Arg2 = "1000"; verify_case(4, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_5() { int Arr0[] = { 0, 1, 2, 3, 4, 5 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "0"; string Arg2 = "INVALID INPUT"; verify_case(5, Arg2, nextInteger(Arg0, Arg1)); }
	void test_case_6() { int Arr0[] = { 1 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "1"; string Arg2 = "11"; verify_case(6, Arg2, nextInteger(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	IntegerGenerator ___test; 
	___test.run_test(1); 
} 
// END CUT HERE 
