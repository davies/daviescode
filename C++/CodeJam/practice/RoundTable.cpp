// BEGIN CUT HERE
// PROBLEM STATEMENT
// We define a permutation of an integer N as an integer that 
// has the exact same digits as N, but possibly in a 
// different order. Two permutations of N are considered 
// different if the numbers they represent are not the same. 
// For example, the set of all different permutations of the 
// number N = 313 is {133, 313, 331}.
// Given a string N and an int M, determine the number of 
// different permutations of N that are divisible by M.
// 
// 
// DEFINITION
// Class:DivisiblePermutations
// Method:count
// Parameters:string, int
// Returns:long long
// Method signature:long long count(string N, int M)
// 
// 
// CONSTRAINTS
// -N will contain between 1 and 15 non-zero digits 
// ('1'-'9'), inclusive.
// -M will be between 1 and 50, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// "133"
// 7
// 
// Returns: 1
// 
// There are three permutations of 133 (133, 313, 331), but 
// only 133 is divisible by 7.
// 
// 1)
// "2753"
// 5
// 
// Returns: 6
// 
// The permutations of 2753 that are divisible by 5 are 2375, 
// 2735, 3275, 3725, 7235 and 7325.
// 
// 2)
// "1112225"
// 5
// 
// Returns: 20
// 
// 3)
// "123456789"
// 17
// 
// Returns: 21271
// 
// 4)
// "987654321999999"
// 39
// 
// Returns: 19960896
// 
// END CUT HERE
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
template<class U,class T> T cast(U x){T y; ostringstream a; a<<x; istringstream b(a.str());b>>y;return y;}
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())
#define FOUND(v,p) (find(ALL(v),p)!=v.end())
#define SUM(v) accumulate(ALL(v),0)



class DivisiblePermutations
 {
	 map<string,vector<int> > am;
	 void fill(string s, int M)
	 {
		 if( am.find(s) != am.end() ) return ;
		 vector<int> res(M,0);
		 bool any = false;
		 REP(i,10) if( s[i] > 'A' ){
			 any = true;
			 string ns = s;
			 ns[i]--;
			 fill(ns,M);
			 vector<int> &v = am[ns];
			 REP(j,M)  res[ (j*10+i)%M ] += v[j];
		 }
		 if( !any){
			 res[0] = 1;
		 }
		 am[s] = res;
	 }
 public:
         long long count(string N, int M) 
        { 
			am.clear();
			string s(10,'A');
			REP(i,SZ(N)) s[N[i]-'0']++;
			fill(s,M);
			return am[s][0];
        } 
        
// BEGIN CUT HERE
        public:
        void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
        private:
        template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
        void verify_case(int Case, const long long &Expected, const long long &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
        void test_case_0() { string Arg0 = "133"; int Arg1 = 7; long long Arg2 = 1LL; verify_case(0, Arg2, count(Arg0, Arg1)); }
        void test_case_1() { string Arg0 = "2753"; int Arg1 = 5; long long Arg2 = 6LL; verify_case(1, Arg2, count(Arg0, Arg1)); }
        void test_case_2() { string Arg0 = "1112225"; int Arg1 = 5; long long Arg2 = 20LL; verify_case(2, Arg2, count(Arg0, Arg1)); }
        void test_case_3() { string Arg0 = "123456789"; int Arg1 = 17; long long Arg2 = 21271LL; verify_case(3, Arg2, count(Arg0, Arg1)); }
        void test_case_4() { string Arg0 = "987654321999999"; int Arg1 = 39; long long Arg2 = 19960896LL; verify_case(4, Arg2, count(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
 {
	 vector<int> v1(5,1);
	 vector<int> v2(5,1);
	 map< vector<int>, int> m;
	cout << (v1 == v2) ;
        DivisiblePermutations ___test; 
        ___test.run_test(-1); 
} 
// END CUT HERE 
