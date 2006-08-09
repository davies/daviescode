// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// An accurate scale is one of the most important tools of 
// the apothecary (an old-time pharmacist).  
// To measure the weight of an object, the apothecary places 
// the object on one
// pan of the scale, along with some weights of known size, 
// and adds more weights
// of known size to the other pan until the scales balance.  
// For example, if an object
// weighs 17 grains, the apothecary could balance the scales 
// by placing a 1-grain weight
// and a 9-grain weight in the pan with the object, and a 27-
// grain weight in the other
// pan.
// 
// 
// 
// The apothecary owns weights in a range of sizes starting 
// at 1 grain.  In particular, he owns one weight for each
// power of 3: 1 grain, 3 grains, 9 grains, 27 grains, etc.  
// Determine, for an object weighing W grains, how to 
// distribute the weights among the
// pans to balance the object.  This distribution will be 
// unique.  Return a vector <int> of the weights used.  The 
// sign of each weight should be
// negative if the weight goes in the same pan as the object, 
// and positive if it goes in the other pan.  The vector 
// <int> should be arranged in increasing order.
// 
// 
// 
// DEFINITION
// Class:Apothecary
// Method:balance
// Parameters:int
// Returns:vector <int>
// Method signature:vector <int> balance(int W)
// 
// 
// CONSTRAINTS
// -W is between 1 and 1000000, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// 17
// 
// Returns: { -9,  -1,  27 }
// 
// The example above.
// 
// 1)
// 1
// 
// Returns: { 1 }
// 
// A 1-grain weight is placed in the pan opposite the object 
// being measured.
// 
// 2)
// 2016
// 
// Returns: { -243,  -9,  81,  2187 }
// 
// A 9-grain weight and a 243-grain weight are placed in the 
// pan with the object,
// and an 81-grain weight and a 2187-grain weight are placed 
// in the opposite pan.
// 
// 3)
// 1000000
// 
// Returns: { -531441,  -59049,  -6561,  -243,  -27,  1,  
// 81,  729,  2187,  1594323 }
// 
// END CUT HERE
#line 81 "Apothecary.cpp"

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

class Apothecary
{ 
public: 
	vector <int> balance(int W) 
	{ 
		VI m(1,1),p(1,1);
		while( m.back() < W ) {
			p.push_back(p.back()*3);
			m.push_back(m.back()+p.back());
		}
		vector <int> q(SZ(m),0);
		for(int i=SZ(m)-1;i>0;i--){
			if( abs(W) > m[i-1] ) {
				q[i] = W/abs(W);
				W -= p[i]*q[i];
			}
		}
		q[0] = W;
		VI re;
		REP(i,SZ(q))if(q[i]!=0)re.push_back(q[i]*p[i]);
		sort(ALL(re));
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <int> &Expected, const vector <int> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { int Arg0 = 17; int Arr1[] = { -9,  -1,  27 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(0, Arg1, balance(Arg0)); }
	void test_case_1() { int Arg0 = 1; int Arr1[] = { 1 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(1, Arg1, balance(Arg0)); }
	void test_case_2() { int Arg0 = 2016; int Arr1[] = { -243,  -9,  81,  2187 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(2, Arg1, balance(Arg0)); }
	void test_case_3() { int Arg0 = 1000000; int Arr1[] = { -531441,  -59049,  -6561,  -243,  -27,  1,  81,  729,  2187,  1594323 }; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(3, Arg1, balance(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Apothecary ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
