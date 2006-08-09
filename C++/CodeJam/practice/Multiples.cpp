// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// You are to create a class Multiples with a method number, 
// which takes three ints: min, max, and factor.
// 
// 
// 
// Given a range of integers from min to max (inclusive), 
// determine how many numbers within that range are evenly 
// divisible by factor.
// 
// 
// DEFINITION
// Class:Multiples
// Method:number
// Parameters:int, int, int
// Returns:int
// Method signature:int number(int min, int max, int factor)
// 
// 
// NOTES
// -If x is evenly divisble by y, there exists some integer k 
// such that k * y = x.
// 
// 
// CONSTRAINTS
// -min will be between -1000000 and 1000000, inclusive.
// -max will be between -1000000 and 1000000, inclusive.
// -max will be greater than or equal to min.
// -factor will be between 1 and 1000, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// 0
// 14
// 5
// 
// Returns: 3
// 
// The numbers 0, 5, and 10 are evenly divisible by 5, so 
// this returns 3.
// 
// 1)
// 7
// 24
// 3
// 
// Returns: 6
// 
// The numbers 9, 12, 15, 18, 21, 24 are evenly divisible by 
// 3, so this returns 6.
// 
// 
// 2)
// -123456
// 654321
// 997
// 
// Returns: 780
// 
// 3)
// -75312
// 407891
// 14
// 
// Returns: 34515
// 
// END CUT HERE
#line 73 "Multiples.cpp"

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

typedef long long ll;
typedef vector<int> VI;
typedef vector< VI > VVI;
typedef vector<char> VC;
typedef vector<string> VS;
typedef stringstream SS;
typedef istringstream ISS;
typedef ostringstream OSS;

#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
template<class U,class T> T cast(U x){T y; OSS a; a<<x; ISS b(a.str());b>>y;return y;}
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())
#define FOUND(v,p) (find(ALL(v),p)!=v.end())
#define DV(v) REP(i,SZ(v)) cout << v[i] << " "; cout << endl
#define SUM(v) accumulate(ALL(v),0)
#define SE(i) (i)->second

class Multiples
{ 
public: 
	int number(int min, int max, int factor) 
	{ 
		while(min<=max&&min%factor!=0) min++;
		while(min<=max&&max%factor!=0) max--;
		return (max-min)/factor+1;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 0; int Arg1 = 14; int Arg2 = 5; int Arg3 = 3; verify_case(0, Arg3, number(Arg0, Arg1, Arg2)); }
	void test_case_1() { int Arg0 = 7; int Arg1 = 24; int Arg2 = 3; int Arg3 = 6; verify_case(1, Arg3, number(Arg0, Arg1, Arg2)); }
	void test_case_2() { int Arg0 = -123456; int Arg1 = 654321; int Arg2 = 997; int Arg3 = 780; verify_case(2, Arg3, number(Arg0, Arg1, Arg2)); }
	void test_case_3() { int Arg0 = -75312; int Arg1 = 407891; int Arg2 = 14; int Arg3 = 34515; verify_case(3, Arg3, number(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Multiples ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
