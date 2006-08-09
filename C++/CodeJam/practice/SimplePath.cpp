// BEGIN CUT HERE
// PROBLEM STATEMENT
// A road has been laid out so that it consists of segments, 
// each of which
// runs either North-South or East-West.  We will say that 
// one segment
// "touches" another segment if the two segments have a 
// common point, other than
// the common point between two sequential segments in the 
// path. We want to check 
// the surveyor's data to make sure that it represents a 
// simple path -- specifically
// one in which no two segments touch.
// 
// We are given a string direction and a vector <int> length 
// giving the surveyor's data 
// for each segment of the road. The i-th character of
// direction tells the direction ('N','E','S', or 'W') of the 
// i-th segment, and
// the i-th element of length tells the length of that segment.
// Create a class SimplePath that contains a method trouble 
// that is given direction
// and length and returns the zero-based index of the lowest-
// indexed segment that touches
// another segment.  If no two
// segments touch, return -1.
// 
// 
// 
// DEFINITION
// Class:SimplePath
// Method:trouble
// Parameters:string, vector <int>
// Returns:int
// Method signature:int trouble(string direction, vector 
// <int> length)
// 
// 
// CONSTRAINTS
// -direction will contain between 1 and 50 characters 
// inclusive.
// -length will contain the same number of elements as 
// direction has characters.
// -Each character of direction will be an uppercase letter 
// 'N', 'E', 'S', or 'W'.
// -Each element of length will be between 1 and 10,000 
// inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// "NWSEEN"
// {5,5,3,2,5,2}
// 
// Returns: 0
// 
// 
//    The first segment goes north 5 units. The second then 
// goes 5 west. The third
//    segment goes 3 south. The fourth goes 2 east. The fifth 
// segment continues
//    east and touches (in fact intersects) the first 
// segment. Return the zero-based
//    index of the first segment.
// 
// 
// 1)
// "NWWS"
// {10,3,7,10}
// 
// Returns: -1
// 
// 
//    This road is a 10 x 10 x 10 U-shaped simple path.
// 
// 2)
// "NWES"
// {2,2,1,2}
// 
// Returns: 1
// 
// 
//    Nothing touches the first segment, but the second 
// segment touches the third segment. 
//    In fact, the third segment
//    doubles back and covers half of the second segment. The 
// zero-based index of the 
//    second segment is 1.
// 
// 3)
// "NWSE"
// {100,100,100,100}
// 
// Returns: 0
// 
// 
//     This road forms a square, so the last segment touches 
// the first segment and
//     we return the index of the first segment.
// 
// 4)
// "EEEEEW"
// {1,1,1,1,1,10}
// 
// Returns: 0
// 
// END CUT HERE
#line 110 "SimplePath.cpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <complex>
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

class SimplePath
{ 
public:
	typedef complex<int> P;
	bool interp(int a, int b, int c, int d)
	{
		if( a < b ) swap(a,b);
		if( c < d ) swap(c,d);
		if( a < c ) {swap(a,c);swap(b,d);}
		return c<=a && c>=b;
	}
	int trouble(string d, vector <int> l) 
	{ 
		vector<P> st,en;
		P cu(0,0);
		REP(i,SZ(l)){
			st.push_back(cu);
			switch(d[i]){
			case 'E': cu+=P(1,0)*l[i];break;
			case 'W': cu+=P(-1,0)*l[i];break;
			case 'N': cu+=P(0,1)*l[i];break;
			case 'S': cu+=P(0,-1)*l[i];break;
			}
			en.push_back(cu);
		}
		FOR(i,0,SZ(l)-2) 
			FOR(j,i+2,SZ(l)) 
				if( interp(st[i].real(),en[i].real(),st[j].real(),en[j].real())
					&& interp(st[i].imag(),en[i].imag(),st[j].imag(),en[j].imag()) ) return i;
		return -1;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "NWSEEN"; int Arr1[] = {5,5,3,2,5,2}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(0, Arg2, trouble(Arg0, Arg1)); }
	void test_case_1() { string Arg0 = "NWWS"; int Arr1[] = {10,3,7,10}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = -1; verify_case(1, Arg2, trouble(Arg0, Arg1)); }
	void test_case_2() { string Arg0 = "NWES"; int Arr1[] = {2,2,1,2}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 1; verify_case(2, Arg2, trouble(Arg0, Arg1)); }
	void test_case_3() { string Arg0 = "NWSE"; int Arr1[] = {100,100,100,100}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(3, Arg2, trouble(Arg0, Arg1)); }
	void test_case_4() { string Arg0 = "EEEEEW"; int Arr1[] = {1,1,1,1,1,10}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(4, Arg2, trouble(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	SimplePath ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
