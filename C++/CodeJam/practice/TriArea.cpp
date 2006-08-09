// BEGIN CUT HERE
// PROBLEM STATEMENT
// We have a collection of possibly overlapping 45 degree 
// isosceles right triangles.
// Each triangle is oriented with its hypotenuse at the 
// bottom, parallel with the 
// x axis.  Each triangle is specified by giving the center 
// point (xCenter,yCenter)
// of its hypotenuse and the height.  For example, a triangle 
// with center at (1,3) and with height 7
// would have its three vertices at (-6,3), (8,3), and (1,10).
// 
// We need to know the area that is covered by the collection 
// of triangles. Of
// course, we don't want to count overlapping areas multiple 
// times.  Create a 
// class TriArea that contains a method area that is given 
// vector <int>s xCenter, yCenter,
// and height and returns the area covered.  The 
// specification of the i-th 
// triangle is given by the i-th element of xCenter,
// yCenter, and height.
// 
// 
// 
// DEFINITION
// Class:TriArea
// Method:area
// Parameters:vector <int>, vector <int>, vector <int>
// Returns:double
// Method signature:double area(vector <int> xCenter, vector 
// <int> yCenter, vector <int> height)
// 
// 
// CONSTRAINTS
// -xCenter will contain between 1 and 25 elements, inclusive.
// -yCenter and height will have the same number of elements 
// as xCenter.
// -Each element of xCenter and yCenter will be between -100 
// and 100, inclusive.
// -Each element of height will be between 1 and 100 inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {1}
// 
// 
// {3}
// {7}
// 
// Returns: 49.0
// 
// 
// 
//    This collection consists of just one triangle with a 
// height of 7. This
//    triangle has a base of 14 and an altitude of 7 so it 
// has area 49.
// 
// 
// 
// 1)
// {1,1,1}
// {3,3,3}
// {7,7,8}
// 
// Returns: 64.0
// 
// 
// 
//    The first two triangles in this collection are 
// identical and the third
//    triangle contains both of them, so the total area 
// covered is just the
//    area covered by the third one.
// 
// 
// 2)
// {1,2}
// {3,3}
// {7,7}
// 
// Returns: 55.75
// 
// 
//    The area covered by these 2 triangles is the sum of 
// their areas minus the
//    area of their intersection. Their intersection is a 45 
// degree right triangle
//    with height 6.5, so the result is 7*7 + 7*7 - 6.5*6.5 = 
// 55.75.
// 
// 3)
// {3, 6, 10, 12}
// {4, 11, -1, -7}
// {2, 4, 6, 8}
// 
// Returns: 116.0
// 
#line 103 "TriArea.cpp"
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

class TriArea
{ 
public: 
	double area(vector <int> xCenter, vector <int> yCenter, vector <int> height) 
	{ 
		double re=0;
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {1}

; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {3}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = {7}; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); double Arg3 = 49.0; verify_case(0, Arg3, area(Arg0, Arg1, Arg2)); }
	void test_case_1() { int Arr0[] = {1,1,1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {3,3,3}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = {7,7,8}; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); double Arg3 = 64.0; verify_case(1, Arg3, area(Arg0, Arg1, Arg2)); }
	void test_case_2() { int Arr0[] = {1,2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {3,3}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = {7,7}; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); double Arg3 = 55.75; verify_case(2, Arg3, area(Arg0, Arg1, Arg2)); }
	void test_case_3() { int Arr0[] = {3, 6, 10, 12}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {4, 11, -1, -7}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arr2[] = {2, 4, 6, 8}; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); double Arg3 = 116.0; verify_case(3, Arg3, area(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	TriArea ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
