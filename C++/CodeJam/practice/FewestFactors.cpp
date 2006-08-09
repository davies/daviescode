// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// You will be given some decimal digits in a vector <int>
// digits. Build an integer with the minimum possible number
// of factors, using each of the digits exactly once (be sure 
// to
// count all factors, not only the prime factors). If more 
// than one number
// has the same (minimum) number of factors, return the
// smallest one among them.
// 
// 
// 
// DEFINITION
// Class:FewestFactors
// Method:number
// Parameters:vector <int>
// Returns:int
// Method signature:int number(vector <int> digits)
// 
// 
// NOTES
// -A factor of an integer n is an integer k, such that n % k 
// = 0 (% being the modulo operator).
// -The digit 0 can also be used as a leading zero, see 
// example 1.
// 
// 
// CONSTRAINTS
// -digits will have between 1 and 5 elements, inclusive.
// -Each element of digits will be between 0 and 9, inclusive.
// -At least one element of digits will be non-zero.
// 
// 
// EXAMPLES
// 
// 0)
// {1, 2}
// 
// Returns: 21
// 
// 
// Using the digits 1 and 2 we can build the numbers 12
// (which has 6 factors: 1, 2, 3, 4, 6 and 12) and 21
// (which has 4 factors: 1, 3, 7 and 21). So we return
// 21, which is the number among them having the smallest
// number of factors.
// 
// 
// 
// 1)
// {6, 0}
// 
// Returns: 6
// 
// 
// Note that we can use 0 as a leading zero, giving the
// number 6 that has 4 factors (1, 2, 3 and 6), less than
// the alternative 60 that has 12 factors.
// 
// 
// 2)
// {4, 7, 4}
// 
// Returns: 447
// 
// 
// Note that digits may contain duplicate digits.
// We have to use each digit as many times as it appears
// in the input.
// 
// 
// 
// 3)
// {1, 3, 7, 9}
// 
// Returns: 1973
// 
// 4)
// {7, 5, 4, 3, 6}
// 
// Returns: 36457
// 
// 5)
// {1,2,4}
// 
// Returns: 241
// 
// Both 241 and 421 are prime numbers, and thus have exactly 
// 2 factors (241 has the factors 1 and 241, while 421 has 
// the factors 1 and 421). All other numbers that we can 
// build from these digits (124, 142, 214 and 412) have more 
// than 2 factors. We have to use the tie-breaker here and 
// return the smaller of (241, 421).
// 
// END CUT HERE
#line 99 "FewestFactors.cpp"

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
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())

class FewestFactors
{ 
public: 
	int number(vector <int> digits) 
	{ 
		int re=0,min=10000;
		string s;
		REP(i,SZ(digits)) s.push_back(digits[i]+'0');
		sort(ALL(digits));
		do{
			int n=0;
			REP(i,SZ(digits))n= n*10+digits[i];
			int cnt=0;
			for(int i=1;i*i<=n;i++){
				if( n%i == 0) cnt += 2;
				if( i*i==n ) cnt --;
			}
			if( cnt < min ) {
				min = cnt;
				re = n;
			}
		}while( next_permutation(ALL(digits)) );
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {1, 2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 21; verify_case(0, Arg1, number(Arg0)); }
	void test_case_1() { int Arr0[] = {6, 0}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 6; verify_case(1, Arg1, number(Arg0)); }
	void test_case_2() { int Arr0[] = {4, 7, 4}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 447; verify_case(2, Arg1, number(Arg0)); }
	void test_case_3() { int Arr0[] = {1, 3, 7, 9}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1973; verify_case(3, Arg1, number(Arg0)); }
	void test_case_4() { int Arr0[] = {7, 5, 4, 3, 6}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 36457; verify_case(4, Arg1, number(Arg0)); }
	void test_case_5() { int Arr0[] = {1,2,4}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 241; verify_case(5, Arg1, number(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	FewestFactors ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
