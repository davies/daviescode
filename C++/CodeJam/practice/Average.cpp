// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// You have acquired a list of the math and verbal test 
// scores from all the children in the county.
// Write a class Average that contains a method belowAvg that 
// takes two vector <int>,
// math and verbal, representing the math and verbal scores 
// of all of the children in the county, and returns the 
// number of children who have a composite score
// which is below average in the county.  
// 
// The composite score is defined to be the sum of a child's 
// math and verbal scores.
// 
// 
// DEFINITION
// Class:Average
// Method:belowAvg
// Parameters:vector <int>, vector <int>
// Returns:int
// Method signature:int belowAvg(vector <int> math, vector 
// <int> verbal)
// 
// 
// NOTES
// -each element of math corresponds to the element with the 
// same index in verbal.
// -the number of children is the length of math and verbal.
// 
// 
// CONSTRAINTS
// -math and verbal will contain the same number of elements.
// -math will contain between 1 and 50 elements, inclusive.
// -each value in math and in verbal is between 200 and 800, 
// inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {200,250,700,700}
// {400,400,400,400}
// 
// Returns: 2
// 
// The composite scores of the 4 children are 600, 650, 1100, 
// and 1100 and the average composite score is 862.5, so 2 of 
// the children are below average.
// 
// 1)
// {500,400}
// {300,400}
// 
// Returns: 0
// 
// Both children have the same composite score 800 which is 
// also the average. Neither are below average
// 
// 2)
// {293}
// {799}
// 
// Returns: 0
// 
// 3)
// {400,400,400,400,400,400,401}
// {400,400,400,400,400,400,400}
// 
// Returns: 6
// 
// The average composite score is just above 800, so 6 of the 
// 7 children are below average.
// 
// END CUT HERE
#line 77 "Average.cpp"

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

class Average
{ 
public: 
	int belowAvg(vector <int> math, vector <int> verbal) 
	{ 
		transform(ALL(math),verbal.begin(),math.begin(),plus<int>());
		return count_if(ALL(math),bind2nd(less<double>(),(double)SUM(math)/SZ(math)));
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {200,250,700,700}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {400,400,400,400}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 2; verify_case(0, Arg2, belowAvg(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {500,400}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {300,400}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(1, Arg2, belowAvg(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {293}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {799}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(2, Arg2, belowAvg(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {400,400,400,400,400,400,401}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {400,400,400,400,400,400,400}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 6; verify_case(3, Arg2, belowAvg(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Average ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
