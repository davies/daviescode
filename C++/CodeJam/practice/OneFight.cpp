// BEGIN CUT HERE
// PROBLEM STATEMENT
// In some computer game, you are surrounded by monsters, and 
// you are to kill them all. Each monster has some number of 
// life points and damage points, and you also have some 
// number of damage points. Life points represent the amount 
// of life left in a monster. As long as a monster has one or 
// more life points, it remains alive. Damage points 
// represent the number of life points that are taken away 
// from an enemy by a single attack. At every turn, you are 
// attacked by each of the living monsters, and then you 
// attack exactly one of them (at the same turn).
// 
// You are given a vector <int> life, the number of life 
// points that each monster starts out with, a vector <int> 
// damage, the number of damage points each monster has, and 
// an int yourDamage, the number of damage points you have. 
// The ith elements of life and damage represent the life 
// points and damage points for the ith monster. Return the 
// minimal number of life points you must start out with in 
// order to kill all the monsters. At the end of the fight, 
// you must have one or more life points remaining.
// 
// DEFINITION
// Class:OneFight
// Method:monsterKilling
// Parameters:vector <int>, vector <int>, int
// Returns:int
// Method signature:int monsterKilling(vector <int> life, 
// vector <int> damage, int yourDamage)
// 
// 
// CONSTRAINTS
// -life will have between 1 and 10 elements, inclusive.
// -life and damage will have the same number of elements.
// -All elements of life will be between 1 and 100, inclusive.
// -All elements of damage will be between 0 and 100, 
// inclusive.
// -yourDamage will be between 1 and 100, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {3}
// {10}
// 2
// 
// Returns: 21
// 
// You must hit the monster twice. You lose 20 life points in 
// the fight. Since you must have at least 1 life point 
// remaining, the answer is 21.
// 
// 1)
// {5,1}
// {5,3}
// 3
// 
// Returns: 19
// 
// If you kill the first monster and then the second one, you 
// will lose (3+5) + (3+5) + 3 = 19 life points. If you kill 
// the second monster and then first one, you will lose (3+5) 
// + 5 + 5 = 18 life points. You choose the latter option.
// 
// 2)
// {5,5,5,5,5,5,5,5,5,5}
// {10,20,30,40,50,60,70,80,90,100}
// 100
// 
// Returns: 2201
// 
// Kill all the monsters in reverse order from last to first.
// 
// 3)
// {6,34,21,79,31,5,8,23,9,100}
// {1,65,95,32,48,9,3,13,100,4}
// 4
// 
// Returns: 6554
// 
#line 84 "OneFight.cpp"
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

class OneFight
{ 
public: 
	int monsterKilling(vector <int> _life, vector <int> _damage, int yourDamage) 
	{ 
		int re=100000;
		int N=SZ(life);
		VI order(N);
		REP(i,N) order[i] = i;
		do{
			int ex=0;
			int l=1;
			REP(i,N){
				ex += (life[order[i]]+yourDamage-1)/yourDamage;
				l += ex * damage[order[i]];
			}
			re = min(re,l);
		}while(next_permutation(ALL(order)));
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {10}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 2; int Arg3 = 21; verify_case(0, Arg3, monsterKilling(Arg0, Arg1, Arg2)); }
	void test_case_1() { int Arr0[] = {5,1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {5,3}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 3; int Arg3 = 19; verify_case(1, Arg3, monsterKilling(Arg0, Arg1, Arg2)); }
	void test_case_2() { int Arr0[] = {5,5,5,5,5,5,5,5,5,5}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {10,20,30,40,50,60,70,80,90,100}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 100; int Arg3 = 2201; verify_case(2, Arg3, monsterKilling(Arg0, Arg1, Arg2)); }
	void test_case_3() { int Arr0[] = {6,34,21,79,31,5,8,23,9,100}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {1,65,95,32,48,9,3,13,100,4}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 4; int Arg3 = 6554; verify_case(3, Arg3, monsterKilling(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{

	OneFight ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
