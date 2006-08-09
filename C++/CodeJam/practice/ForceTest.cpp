// BEGIN CUT HERE
// PROBLEM STATEMENT
// Your company produces components used in construction.  
// From time to time, it is necessary to run a series of 
// tests on several of the components, to determine their 
// breaking force.  The goal of the testing is to determine 
// the point at which the unit breaks, up to a given 
// threshold. (Only integer forces are applied.)
// 
// You have a set of components to be used for testing.  You 
// are to find an optimal testing plan, minimizing the number 
// of tests that will have to be performed (in the worst 
// case).  In the process of testing, you may destroy some or 
// all of the test components, provided that in the end, the 
// breaking force is known.
// 
// Unfortunately, no manufacturing process is perfect, so it 
// is possible that one of your test components is 
// defective.  A defective component is defined as one that 
// breaks under a lesser force than a typically produced 
// component.
// 
// You are given int maxForce, the highest force at which you 
// need to test the components.  (They may have a higher 
// breaking force than this, but you are not concerned with 
// testing any higher force.)  You are also given int 
// testUnits, the number of units that are available for 
// testing.  You are to return the fewest number of tests 
// necessary to conclusively determine the breaking force of 
// a non-defective component.
// 
// 
// DEFINITION
// Class:ForceTest
// Method:fewestTests
// Parameters:int, int
// Returns:int
// Method signature:int fewestTests(int maxForce, int 
// testUnits)
// 
// 
// NOTES
// -Assume that the (non-defective) component has a fixed 
// breaking point.  That is, if it breaks when tested at a 
// given force, it will break whenever tested with a greater 
// force.  Likewise, if it passes testing for a given force, 
// it will pass for any lower force.
// -The testing strategy may destroy all of the components 
// that were made available for testing, as long as it finds 
// the exact breaking force of the non-defective component.
// -All of the test components, except for possibly a single 
// defective one, are identical, and will break under the 
// same force.
// -It is not necessarily the case that one of the test 
// components is defective, however, there will never be more 
// than one defective component.
// -A defective component will always break under a lower 
// force than a normal component.
// 
// 
// CONSTRAINTS
// -maxForce will be between 1 and 100, inclusive.
// -testUnits will be between 2 and 20, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// 1
// 2
// 
// Returns: 2
// 
// We have to do a maximum of two tests.  If the first test 
// should fail, we need to test the second unit to determine 
// if the first was defective.
// 
// 1)
// 2
// 2
// 
// Returns: 3
// 
// Here, we have to start by testing a force of 1.  If the 
// unit breaks, we have to retest with a force of 1 to 
// determine if the first was a defect.  Then, if the second 
// test passes, we need a third test to determine if the good 
// unit can withstand a force of 2.
// 
// Notice that if we had tested the first component with a 
// force of 2, and it had failed, we would have only had a 
// single unit left, and would not have been able to 
// determine if a defective unit was present.
// 
// 
// 2)
// 10
// 4
// 
// Returns: 6
// 
// 3)
// 100
// 2
// 
// Returns: 101
// 
#line 109 "ForceTest.cpp"
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

typedef long long ll;
typedef vector<int> VI;
typedef vector< VI > VVI;
typedef vector<char> VC;
typedef vector<string> VS;
typedef map<int,int> MII;
typedef map<string,int> MSI;

#define SZ(v) ((int)(v).size())
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
#define ALL(v) (v).begin(),(v).end()
#define FOUND(v,p) (find(ALL(v),p)!=v.end())
#define SUM(v) accumulate(ALL(v),0)
#define SE(i) (i)->second
#define DV(v) REP(i,SZ(v)) cout << v[i] << " "; cout << endl

typedef stringstream SS;
typedef istringstream ISS;
typedef ostringstream OSS;
template<class U,class T> T cast(U x){T y; OSS a; a<<x; ISS b(a.str());b>>y;return y;}

class ForceTest
{ 
public: 
	int fewestTests(int maxForce, int testUnits) 
	{ 
		int re;
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 1; int Arg1 = 2; int Arg2 = 2; verify_case(0, Arg2, fewestTests(Arg0, Arg1)); }
	void test_case_1() { int Arg0 = 2; int Arg1 = 2; int Arg2 = 3; verify_case(1, Arg2, fewestTests(Arg0, Arg1)); }
	void test_case_2() { int Arg0 = 10; int Arg1 = 4; int Arg2 = 6; verify_case(2, Arg2, fewestTests(Arg0, Arg1)); }
	void test_case_3() { int Arg0 = 100; int Arg1 = 2; int Arg2 = 101; verify_case(3, Arg2, fewestTests(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	ForceTest ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
