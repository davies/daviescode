// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// Two phrases are anagrams if they are permutations of each 
// other, ignoring spaces and capitalization.
// For example, "Aaagmnrs" is an anagram of "anagrams", and 
// "TopCoder" is an anagram
// of "Drop Cote".  Given a vector <string> phrases, remove 
// each phrase that is an anagram
// of an earlier phrase, and return the remaining phrases in 
// their original order.
// 
// 
// DEFINITION
// Class:Aaagmnrs
// Method:anagrams
// Parameters:vector <string>
// Returns:vector <string>
// Method signature:vector <string> anagrams(vector <string> 
// phrases)
// 
// 
// CONSTRAINTS
// -phrases contains between 2 and 50 elements, inclusive.
// -Each element of phrases contains between 1 and 50 
// characters, inclusive.
// -Each element of phrases contains letters ('a'-'z' and 
// 'A'-'Z') and spaces (' ') only.
// -Each element of phrases contains at least one letter.
// 
// 
// EXAMPLES
// 
// 0)
// { "Aaagmnrs", "TopCoder", "anagrams", "Drop Cote" }
// 
// Returns: { "Aaagmnrs",  "TopCoder" }
// 
// The examples above.
// 
// 1)
// { "SnapDragon vs tomek", "savants groped monk", "Adam 
// vents prongs ok" }
// 
// Returns: { "SnapDragon vs tomek" }
// 
// 2)
// { "Radar ghost jilts Kim", "patched hers first",
//   "DEPTH FIRST SEARCH", "DIJKSTRAS ALGORITHM" }
// 
// Returns: { "Radar ghost jilts Kim",  "patched hers first" }
// 
// END CUT HERE
#line 55 "Aaagmnrs.cpp"

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

class Aaagmnrs
{ 
public: 
	vector <string> anagrams(vector <string> phrases) 
	{ 
		vector <string> re;
		VS tmp;
		REP(i,SZ(phrases)){
			string s=phrases[i];
			REP(j,SZ(s))if(isalpha(s[j])) s[j]=tolower(s[j]);
			sort(ALL(s));
			while( s[0] ==' ') s=s.substr(1);
			if( !FOUND(tmp,s)) {
				tmp.push_back(s);
				re.push_back(phrases[i]);
			}
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <string> &Expected, const vector <string> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { string Arr0[] = { "Aaagmnrs", "TopCoder", "anagrams", "Drop Cote" }; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { "Aaagmnrs",  "TopCoder" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(0, Arg1, anagrams(Arg0)); }
	void test_case_1() { string Arr0[] = { "SnapDragon vs tomek", "savants groped monk", "Adam vents prongs ok" }; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { "SnapDragon vs tomek" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(1, Arg1, anagrams(Arg0)); }
	void test_case_2() { string Arr0[] = { "Radar ghost jilts Kim", "patched hers first",
  "DEPTH FIRST SEARCH", "DIJKSTRAS ALGORITHM" }; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = { "Radar ghost jilts Kim",  "patched hers first" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(2, Arg1, anagrams(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Aaagmnrs ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
