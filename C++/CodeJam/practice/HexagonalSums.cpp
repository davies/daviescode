// BEGIN CUT HERE
// PROBLEM STATEMENT
// Note to plugin users: this problem contains images which 
// may not display properly with some plugins
// Figurate numbers are numbers which form a scalable pattern 
// of nested geometric figures. The hexagonal numbers 
// (1,6,15,28,45,66,91,120,153...) form the following figures 
// with points arranged in a plane as nested hexagons.
// 
// 
//  In the year 1830, the great mathematician, Legendre, 
// proved that every integer larger than 1719 can be formed 
// as the sum of exactly four hexagonal numbers (although 
// many larger integers can also be formed as the sum of 
// fewer than four hexagonal numbers). This was pretty much 
// the last word on the subject until much later, in the year 
// 1990, when this result was improved to three hexagonal 
// numbers for all "sufficiently large" integers. 
//  In this problem we ask the slightly different question: 
// "what is the minimum number of hexagonal numbers that is 
// required to form a particular sum?" Let's call this MLHS
// (n), for Minimum Length Hexagonal number Sum totaling to 
// n. Here are the first few terms of MLHS(n):
// 
// 
// n  MLHS(n)  a minimum length sum
// 1    1      1
// 2    2      1+1
// 3    3      1+1+1
// 4    4      1+1+1+1
// 5    5      1+1+1+1+1
// 6    1      6
// 7    2      1+6
// 8    3      1+1+6
// 9    4      1+1+1+6
// 10   5      1+1+1+1+6
// 11   6      1+1+1+1+1+6
// 12   2      6+6
// 
// 
// For n greater than 1719, we know MLHS(n) <= 4 due to 
// Legendre's result. But since our problem is different we 
// can actually establish much tigher bounds. Six is the 
// highest possible answer which only occurs for MLHS(11) and 
// MLHS(26).
// The largest n that has MLHS(n) = 6 is 26,  
// The largest n that has MLHS(n) = 5 is 130,  
// The largest n that has MLHS(n) = 4 is 146858.
//  
// 
// Given a number, N, return MLHS(n), the minimum number of 
// terms required to form a sum of N, when each term is a 
// hexagonal number.
// 
// 
// DEFINITION
// Class:HexagonalSums
// Method:minLength
// Parameters:int
// Returns:int
// Method signature:int minLength(int N)
// 
// 
// CONSTRAINTS
// -N will be between 1 and 1000000 inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// 26
// 
// Returns: 6
// 
// 1+1+6+6+6+6
// 
// 1)
// 130
// 
// Returns: 5
// 
// 1+28+28+28+45
// 
// 2)
// 146858
// 
// Returns: 4
// 
// 1+1+1326+145530
// 
// 3)
// 999999
// 
// Returns: 3
// 
// 6+258840+741153
// 
// 4)
// 1000000
// 
// Returns: 2
// 
// 285390+714610
// 
// 5)
// 145530
// 
// Returns: 1
// 
// 145530 is the 269th hexagonal number
// 
// END CUT HERE
#line 114 "HexagonalSums.cpp"

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

class HexagonalSums
{ 
public: 
	int minLength(int N, int k)
	{
		static VI hex(1,1);
		FOR(i,SZ(hex),k){
			while( hex.back() < N ) hex.push_back( hex.back()+SZ(hex)*4+1 );
		}
		if( N >= hex[k-1]*5 ) return (N+hex[k-1]-1)/hex[k-1];
		REP(i,k){
			if( N >= hex[i] && N < hex[i]+5 ){
				return N-hex[i]+1;
			}
		}
		map<pair<int,int>,int> re;
		if( re.find(make_pair<int,int>(N,k)) != re.end() ){
			return re[make_pair<int,int>(N,k)];
		}
		int n = N >= hex[k-1] ? min( minLength(N,k-1), minLength(N-hex[k-1],k)+1) : minLength(N,k-1);
		re[make_pair<int,int>(N,k)] = n;
		return n;
	}
	int minLength(int N) 
	{
		VI hex(1,1);
		while( hex.back() < N ) hex.push_back( hex.back()+SZ(hex)*4+1 );
		if( FOUND(hex,N) ) return 1;
		for(int i=SZ(hex)-2;i>=0;i--){
			if( hex[i]*2<N )break;
			if( FOUND(hex,N-hex[i]) ) return 2;
		}
		for(int i=SZ(hex)-2;i>=0;i--){
			if( hex[i]*3<N )break;
			for(int j=i;j>=0;j--){
				if( FOUND(hex,N-hex[i]-hex[j]) ) return 3;
			}
		}
		for(int i=SZ(hex)-2;i>=0;i--){
			if( hex[i]*4<N )break;
			for(int j=i;j>=0;j--){
				if( hex[i]+hex[j]*3 < N ) break;
				for( int k=j;k>=0;k--){
					if( FOUND(hex,N-hex[i]-hex[j]-hex[k]) ) return 4;
				}
			}
		}
		return minLength(N,SZ(hex)-1);
	} 

	// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 214; int Arg1 = 6; verify_case(0, Arg1, minLength(Arg0)); }
	void test_case_1() { int Arg0 = 130; int Arg1 = 5; verify_case(1, Arg1, minLength(Arg0)); }
	void test_case_2() { int Arg0 = 146858; int Arg1 = 4; verify_case(2, Arg1, minLength(Arg0)); }
	void test_case_3() { int Arg0 = 999999; int Arg1 = 3; verify_case(3, Arg1, minLength(Arg0)); }
	void test_case_4() { int Arg0 = 1000000; int Arg1 = 2; verify_case(4, Arg1, minLength(Arg0)); }
	void test_case_5() { int Arg0 = 145530; int Arg1 = 1; verify_case(5, Arg1, minLength(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	HexagonalSums ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
