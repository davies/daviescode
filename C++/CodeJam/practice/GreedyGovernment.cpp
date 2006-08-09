// BEGIN CUT HERE
// PROBLEM STATEMENT
// You live in a town which is divided into sectors, numbered 
// 0 through N-1. In addition, some sectors are connected by 
// roads. You must pay a toll to move between sectors. The 
// government of your town is rather greedy, and it has 
// decided to increase the toll along one of these roads. In 
// particular, they are going to increase the toll along a 
// road by tollHike dollars, such that the average cost of 
// travelling from sector 0 to sector N-1 is maximized. This 
// average cost is determined as the average cost over all 
// distinct valid paths from sector 0 to sector N-1. Two 
// paths from sector 0 to sector N-1 are distinct if they 
// either visit a different number of sectors or visit 
// sectors in a different order. A path is valid if it does 
// not take you through any sector more than once while 
// travelling from sector 0 to N-1.
// 
// 
// Create a class GreedyGovernment which contains a method 
// maxAverageCost. You will be given a vector <string> tolls 
// and an int tollHike as arguments. The j'th character in 
// the i'th element of tolls indicates the toll to travel 
// between sectors i and j. If the j'th character in the i'th 
// element of tolls is an 'X', then it is not possible to 
// travel from sector i to sector j (although you may still 
// be able to travel from sector j to sector i). If there is 
// no way to travel from sector 0 to sector N-1, your method 
// should return 0. Otherwise, the method should return a 
// double corresponding to the maximum average cost that the 
// government can expect.
// 
// DEFINITION
// Class:GreedyGovernment
// Method:maxAverageCost
// Parameters:vector <string>, int
// Returns:double
// Method signature:double maxAverageCost(vector <string> 
// tolls, int tollHike)
// 
// 
// NOTES
// -Your return value must have an absolute or relative error 
// less than 1e-9.
// 
// 
// CONSTRAINTS
// -tolls will contain between 2 and 10 elements, inclusive.
// -Each element of tolls will contain the same number of 
// characters as the number of elements in tolls.
// -Each element of tolls will contain only the characters 
// '1'-'9', inclusive, or the character 'X'.
// -The i'th character of the i'th element of tolls will be 
// 'X' for all i. 
// -tollHike will be between 1 and 100, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {"X324", "XXX2", "12X5", "991X"}
// 9
// 
// Returns: 10.0
// 
// Note that there are 4 ways to travel from sector 0 to 
// sector 3:
// 
// sector 0 --> sector 3
// 
// sector 0 --> sector 1 --> sector 3
// 
// sector 0 --> sector 2 --> sector 3
// 
// sector 0 --> sector 2 --> sector 1 --> sector 3
// 
// 
// Any other path from sector 0 to sector 3 (for example, 
// sector 0 --> sector 2 --> sector 0 --> sector 3) visits a 
// sector more than once, which is not allowed in your town.
// 
// 1)
// {"X324", "5X22", "12X5", "991X"}
// 57
// 
// Returns: 29.2
// 
// 2)
// {"X11", "2X1", "37X"}
// 76
// 
// Returns: 39.5
// 
// 3)
// {"X32X", "XXXX", "XXXX", "XXXX"}
// 99
// 
// Returns: 0.0
// 
// There is no way to travel from sector 0 to sector 3.
// 
// END CUT HERE
#line 104 "GreedyGovernment.cpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <stack>
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

class GreedyGovernment
{ 
public: 
	double maxAverageCost(vector <string> tolls, int tollHike) 
	{ 
		VVI map(SZ(tolls),VI(SZ(tolls),0));
		REP(i,SZ(tolls)) REP(j,SZ(tolls[i])) if(tolls[i][j] != 'X') map[i][j] = tolls[i][j]-'0';
		VI st,sp;
		VI mk(SZ(tolls),0);
		VVI cnt(SZ(tolls),VI(SZ(tolls),0));
		st.push_back(0);sp.push_back(0);
		int toll = 0;
		int sum=0,count=0;
		while(!st.empty()){
			int p = st.back();
			int s = sp.back();
			mk[p]=1;
			FOR(i,s+1,SZ(tolls)) if(mk[i]==0&&map[p][i]>0){
				if( i==SZ(tolls)-1){
					sum += toll+map[p][i];
					REP(j,SZ(st)-1)cnt[st[j]][st[j+1]]++;
					cnt[p][i]++;
					count++;
				}else{
					toll += map[p][i];
					st.push_back(i);
					sp.push_back(0);
					goto END;
				}
			}
			mk[p] = 0;
			st.erase(st.end()-1);
			sp.erase(sp.end()-1);
			if( !sp.empty() ){
				toll-= map[st.back()][p];
				sp.back()=p;
			}
END:	
			int a=0;
		}
		int m=0;
		REP(i,SZ(tolls))m=max(m,*max_element(ALL(cnt[i])));
		return count==0?0:(double)(m*tollHike+sum)/count;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"X324", "XXX2", "12X5", "991X"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 9; double Arg2 = 10.0; verify_case(0, Arg2, maxAverageCost(Arg0, Arg1)); }
	void test_case_1() { string Arr0[] = {"X324", "5X22", "12X5", "991X"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 57; double Arg2 = 29.2; verify_case(1, Arg2, maxAverageCost(Arg0, Arg1)); }
	void test_case_2() { string Arr0[] = {"X11", "2X1", "37X"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 76; double Arg2 = 39.5; verify_case(2, Arg2, maxAverageCost(Arg0, Arg1)); }
	void test_case_3() { string Arr0[] = {"X32X", "XXXX", "XXXX", "XXXX"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 99; double Arg2 = 0.0; verify_case(3, Arg2, maxAverageCost(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	GreedyGovernment ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
