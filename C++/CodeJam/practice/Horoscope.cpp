// BEGIN CUT HERE
// PROBLEM STATEMENT
// Recently, you went to a fair where one of the attractions 
// was an oracle who would give you your horoscope for the 
// next several days. She wasn't very precise though; all she 
// would say is whether you would have a Good ('G') or Bad 
// ('B') day on a particular day. Besides not being very 
// precise, she was very inconsistent: she could be right AT 
// MOST R times in a row, and she could be wrong AT MOST W 
// times in a row.
// 
// 
// Create a class Horoscope, which contains a method 
// maxGoodDays. The method takes a vector <string> 
// predictions, an int R and an int W as input. The elements 
// of predictions are strings containing only the characters 
// 'G' or 'B', indicating whether your horoscope for that day 
// was predicted to be good or bad. (Note that you should 
// concatenate the elements of predictions and consider the 
// entire string the predictions). The method should return 
// the maximum number of Good days that you can experience 
// given the predictions, and values for R and W.
// 
// DEFINITION
// Class:Horoscope
// Method:maxGoodDays
// Parameters:vector <string>, int, int
// Returns:int
// Method signature:int maxGoodDays(vector <string> 
// predictions, int R, int W)
// 
// 
// CONSTRAINTS
// -predictions will contain between 1 and 50 elements 
// inclusive.
// -Each element of predictions will contain between 1 and 50 
// characters inclusive.
// -Each element of predictions will contain only the 
// characters 'G' or 'B'.
// -R will be between 1 and 50 inclusive.
// -W will be between 1 and 50 inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {"GGGG"}
// 4
// 1
// 
// Returns: 4
// 
// Since R is 4, the oracle can be Right for all 4 predicted 
// days. In this case, the maximum
// number of good days you would experience is 4.
// 
// 1)
// {"GGGG"}
// 2
// 2
// 
// Returns: 3
// 
// Here, the oracle can't be Right for all 4 days. One way of 
// achieving 3 Good days (which is the
// maximum possible in this case) is for the oracles 
// predictions to be Right, Wrong, Right, Right 
// for the 4 days respectively. Thus, the return value here 
// is 3.
// 
// 2)
// {"GBGBBB"}
// 3
// 4
// 
// Returns: 6
// 
// 3)
// {"GGGBBBGBGGGB", 
// "GGBBBBBBBBBGBGBGBGBGBGBGBGBBBBBBBBBBBBBBGGGG", "G"}
// 4
// 35
// 
// Returns: 56
// 
// END CUT HERE
#line 88 "Horoscope.cpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <bitset>
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
typedef stringstream SS;
typedef istringstream ISS;
typedef ostringstream OSS;

#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
template<class U,class T> T cast(U x){T y; OSS a; a<<x; ISS b(a.str());b>>y;return y;}
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())
#define FOUND(v,p) (find(ALL(v),p)!=v.end())
#define DV(v) REP(i,SZ(v)) cout << v[i] << ""; cout << endl
#define SUM(v) accumulate(ALL(v),0)
#define SE(i) (i)->second

class Horoscope
{ 
public: 
	bool check(const VI &r, const VI &p, int R, int W){
		int cnt = 0;
		REP(j,SZ(r)) if(p[j] == r[j] ) {
			if( cnt >= 0 ) {
				cnt++;
				if( cnt > R ) return false;
			}else{
				cnt = 1;
			}
		}else{
			if( cnt <= 0 ) {
				cnt--;
				if( cnt < -W ) return true;
			}else{
				cnt = -1;
			}
		}
		return true;
	}
	int maxGoodDays(vector <string> pred, int R, int W) 
	{ 
		int cnt=0,ri=0;
		REP(i,SZ(pred)) REP(j,SZ(pred[i]))
			if( pred[i][j] == 'G' && ri<R || ri <= -W){
				cnt++;
				ri=max(0,ri)+1;
			}else {
				if( pred[i][j] != 'G' ) cnt ++;
				ri=min(0,ri)-1;
			}
		return cnt;
		//VI p;
		//REP(i,SZ(pred)) REP(j,SZ(pred[i])) p.push_back(pred[i][j] == 'G');
		//VI real(SZ(p),1);
		//int m=SZ(p);
		//while( m>=0 ){
		//	do{
		//		if( check(real,p,R,W) ) return m;
		//	}while( next_permutation(ALL(real)) );
		//	if( m==0) break;
		//	real[SZ(p)-m]=0;
		//	m --;
		//}
		//return m;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"GGGG"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arg2 = 1; int Arg3 = 4; verify_case(0, Arg3, maxGoodDays(Arg0, Arg1, Arg2)); }
	void test_case_1() { string Arr0[] = {"GGGG"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; int Arg2 = 2; int Arg3 = 3; verify_case(1, Arg3, maxGoodDays(Arg0, Arg1, Arg2)); }
	void test_case_2() { string Arr0[] = {"GBGBBB"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; int Arg2 = 4; int Arg3 = 6; verify_case(2, Arg3, maxGoodDays(Arg0, Arg1, Arg2)); }
	void test_case_3() { string Arr0[] = {"GGGBBBGBGGGB", "GGBBBBBBBBBGBGBGBGBGBGBGBGBBBBBBBBBBBBBBGGGG", "G"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arg2 = 35; int Arg3 = 56; verify_case(3, Arg3, maxGoodDays(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Horoscope ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
