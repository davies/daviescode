// BEGIN CUT HERE
// PROBLEM STATEMENT
// A teacher has graded a test, and you are given a vector 
// <int> scores representing each student's raw score on the 
// test.  The teacher wants to convert the scores into grades 
// such that a raw score of 0 receives a grade of 0, and the 
// highest raw score achieved by any student receives a grade 
// of 100.  All other scores in between are calculated 
// proportionally, always rounded down.  You are to return a 
// vector <int> (with the same number of elements as scores) 
// containing the calculated grade for each student.  Element 
// i of the return should be the grade for scores[i].
// 
// DEFINITION
// Class:TestCurve
// Method:determineGrades
// Parameters:vector <int>
// Returns:vector <int>
// Method signature:vector <int> determineGrades(vector <int> 
// scores)
// 
// 
// CONSTRAINTS
// -scores will contain between 1 and 50 elements, inclusive.
// -Each element of scores will be between 0 and 10000, 
// inclusive.
// -At least one element of scores will be greater than 0.
// 
// 
// EXAMPLES
// 
// 0)
// {15, 27, 8, 33, 19, 50}
// 
// Returns: {30, 54, 16, 66, 38, 100 }
// 
// Since the highest score is a 50, which gets scaled to a 
// grade of 100, everyone's grade is exactly twice their raw 
// score.
// 
// 1)
// {0, 0, 0, 3}
// 
// Returns: {0, 0, 0, 100 }
// 
// All but one person did poorly on this test.
// 
// 2)
// {67, 89, 72, 100, 95, 88}
// 
// Returns: {67, 89, 72, 100, 95, 88 }
// 
// With a highest score of 100, the grade is exactly the raw 
// score.
// 
// 3)
// {1234, 3483, 234, 5738, 3421, 5832, 4433}
// 
// Returns: {21, 59, 4, 98, 58, 100, 76 }
// 
// 4)
// {8765}
// 
// Returns: {100 }
// 
#line 67 "TestCurve.cpp"
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

class TestCurve
{ 
public: 

	vector <int> determineGrades(vector <int> scores) 
	{ 
		vector <int> re;
		int m = *max_element(ALL(scores));
		double sc = 100.0 / m;
		REP(i,SZ(scores)) re.push_back(scores[i]*sc);
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <int> &Expected, const vector <int> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { int Arr0[] = {15, 27, 8, 33, 19, 50}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {30, 54, 16, 66, 38, 100 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(0, Arg1, determineGrades(Arg0)); }
	void test_case_1() { int Arr0[] = {0, 0, 0, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {0, 0, 0, 100 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(1, Arg1, determineGrades(Arg0)); }
	void test_case_2() { int Arr0[] = {67, 89, 72, 100, 95, 88}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {67, 89, 72, 100, 95, 88 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(2, Arg1, determineGrades(Arg0)); }
	void test_case_3() { int Arr0[] = {1234, 3483, 234, 5738, 3421, 5832, 4433}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {21, 59, 4, 98, 58, 100, 76 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(3, Arg1, determineGrades(Arg0)); }
	void test_case_4() { int Arr0[] = {8765}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {100 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(4, Arg1, determineGrades(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	TestCurve ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
