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

class TheTournament{
public:
	string findLeader(vector<string> vs){
		string ans;
		return ans;
	}
public:
	void run_test(int Case) { 
		test_case_0(); 
	}
private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() {
		string Arr0[] = {"Ted defeats Kate", "Kate defeats Billy", "Ted defeats Billy"};
		vector<string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); 

		string Result = "Ted";
		verify_case(0, Result, findLeader(Arg0)); 
	}
};

int main()
{
	TheTournament ___test; 
	___test.run_test(-1); 
} 
