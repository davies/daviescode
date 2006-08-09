// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// In the new boardgame Medici, you play an aristocrat in 
// 15th-century Florence.  On each turn, you gather
// points in three categories: fame, fortune, and dirty 
// secrets with which you can blackmail other players.  The
// winner is the player with the most points in his weakest 
// category at the end of the game.  
// For example, suppose there are three players with the 
// following points:
// 
// 
//     NAME       FAME  FORTUNE  SECRETS
//     ---------------------------------
//     Salvestro   20      60       40
//     Giovanni    30      80       30
//     Cosimo      50      40       50
// 
// Salvestro's weakest category is Fame, in which he has 20 
// points.  Giovanni's weakest categories are Fame and 
// Secrets, with 30 points in both.  Cosimo's weakest 
// category is Fortune, with 40 points.  Cosimo is the winner.
// 
// 
// You will be given three vector <int>s, fame, fortune, and 
// secrets.  The points for player i are 
// given as the i-th elements of the three vector <int>s.  
// You are to return the (zero-based) index of the winning 
// player.  If there
// is a tie for the winning score, the game is declared a 
// draw and replayed, in which case you should return -1.
// 
// 
// DEFINITION
// Class:Medici
// Method:winner
// Parameters:vector <int>, vector <int>, vector <int>
// Returns:int
// Method signature:int winner(vector <int> fame, vector 
// <int> fortune, vector <int> secrets)
// 
// 
// CONSTRAINTS
// -fame contains between 2 and 20 elements, inclusive.
// -fortune and secrets each contain the same number of 
// elements as fame.
// -All elements of fame, fortune, and secrets are between 0 
// and 100, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// { 20, 30, 50 }
// { 60, 80, 40 }
// { 40, 30, 50 }
// 
// Returns: 2
// 
// The example above.  Salvestro is player 0, Giovanni is 
// player 1, and Cosimo is player 2.
// 
// 1)
// {   0, 100, 100, 100 }
// { 100,   0, 100,  50 }
// {  50,  50,  50, 100 }
// 
// Returns: -1
// 
// Players 2 and 3 both have 50 points in their weakest 
// categories (Secrets and Fortune,
// respectively).  The game is a draw.
// 
// 2)
// { 1, 0 }
// { 1, 99 }
// { 1, 99 }
// 
// Returns: 0
// 
// Player 0 wins even though he scored 195 fewer total points 
// than player 1.
// Total points is irrelevant in determining the winner.
// 
// 3)
// { 39, 42, 57 }
// { 42, 39, 57 }
// { 57, 39, 42 }
// 
// Returns: 2
// 
// Players 0 and 1 tie for second place, but that does
// not cause the game to be declared a draw.
// 
// 4)
// { 3, 98, 0, 71, 81, 85, 99, 58, 93, 79, 100, 98, 73, 83, 
// 52, 100, 37, 93, 39, 27 }
// { 96, 34, 45, 20, 72, 71, 7, 18, 47, 35, 54, 51, 52, 21, 
// 49, 20, 44, 72, 90, 93 }
// { 15, 72, 50, 84, 41, 49, 35, 71, 20, 29, 73, 18, 35, 61, 
// 70, 34, 59, 68, 90, 18 }
// 
// Returns: 17
// 
// END CUT HERE
#line 108 "Medici.cpp"

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

class Medici
{ 
public: 
	int winner(vector <int> fame, vector <int> fortune, vector <int> secrets) 
	{ 
		VI v(SZ(fame));
		REP(i,SZ(fame)) v[i] = min(min(fame[i],fortune[i]),secrets[i]);
		int winner = max_element(ALL(v))-v.begin();
		if( count(ALL(v),*max_element(ALL(v))) > 1 ) return -1;
		return winner;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = { 20, 30, 50 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = { 60, 80, 40 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = { 40, 30, 50 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); int Arg3 = 2; verify_case(0, Arg3, winner(Arg0, Arg1, Arg2)); }
	void test_case_1() { int Arr0[] = {   0, 100, 100, 100 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = { 100,   0, 100,  50 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = {  50,  50,  50, 100 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); int Arg3 = -1; verify_case(1, Arg3, winner(Arg0, Arg1, Arg2)); }
	void test_case_2() { int Arr0[] = { 1, 0 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = { 1, 99 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = { 1, 99 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); int Arg3 = 0; verify_case(2, Arg3, winner(Arg0, Arg1, Arg2)); }
	void test_case_3() { int Arr0[] = { 39, 42, 57 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = { 42, 39, 57 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = { 57, 39, 42 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); int Arg3 = 2; verify_case(3, Arg3, winner(Arg0, Arg1, Arg2)); }
	void test_case_4() { int Arr0[] = { 3, 98, 0, 71, 81, 85, 99, 58, 93, 79, 100, 98, 73, 83, 52, 100, 37, 93, 39, 27 }; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = { 96, 34, 45, 20, 72, 71, 7, 18, 47, 35, 54, 51, 52, 21, 49, 20, 44, 72, 90, 93 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = { 15, 72, 50, 84, 41, 49, 35, 71, 20, 29, 73, 18, 35, 61, 70, 34, 59, 68, 90, 18 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); int Arg3 = 17; verify_case(4, Arg3, winner(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Medici ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
