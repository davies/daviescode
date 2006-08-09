// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// You will be given some two-dimensional vectors (the i-th 
// element
// of the vector <int>s dx and dy represent the
// i-th vector (dx[i], dy[i])). Using some (or all) of
// these in some order you may be able to construct a
// polygon by appending them end to end - i.e., represent 
// each vector
// as an arrow, and place the arrows such that each arrow 
// begins
// where the last one ends. In order to get a polygon,
// the last arrow must end at the beginning of the first arrow.
// The image below shows an example with three vectors.
// 
// 
// 
// 
// 
// The left image shows the three vectors as arrows. We can 
// start
// with the red vector (2, 2), append the green vector (3, 
// -4) and
// finally append the blue vector (-5, 2) and we get the 
// triangle
// in the middle image.
// We can also append them in a different order, starting 
// again with
// the red one, but appending first the blue one and finally 
// the
// green one, getting the triangle in the right image.
// 
// 
// You are to find the convex polygon that you can
// construct using some (or all) of the given vectors in some
// order that has the maximum area, and return this area. If
// you cannot construct a convex polygon using the given
// vectors, return 0.0.
// 
// 
// 
// DEFINITION
// Class:VectorPolygon
// Method:maxArea
// Parameters:vector <int>, vector <int>
// Returns:double
// Method signature:double maxArea(vector <int> dx, vector 
// <int> dy)
// 
// 
// NOTES
// -Your return value must have an absolute or relative error 
// less than 1e-9.
// -If two or more parallel (or even identical) vectors are 
// given in the input, you are allowed to use them in a row - 
// i.e., the polygon may have 180 degrees angles, see example 
// 6.
// 
// 
// CONSTRAINTS
// -dx and dy will each have between 1 and 8 elements, 
// inclusive.
// -dx and dy will have the same number of elements.
// -Each element of dx and dy will be between -100 and 100, 
// inclusive.
// -There will be no zero-vector given, i.e., dx[i] and dy[i] 
// will not be both 0 for any i.
// 
// 
// EXAMPLES
// 
// 0)
// {2, 3, -5}
// {2, -4, 2}
// 
// Returns: 7.0
// 
// 
// The example from the problem statement. Both shown 
// triangles have the
// same area.
// 
// 
// 
// 1)
// {2, -3, -5}
// {2, 4, 2}
// 
// Returns: 0.0
// 
// 
// Note that the vectors are directed: this is a similar 
// example as that
// of the problem statement, but with the green arrow 
// pointing in the opposite
// direction. We can not construct a polygon using these 
// vectors.
// 
// 
// 
// 2)
// {0, 0, 1, -1}
// {1, -1, 0, 0}
// 
// Returns: 1.0
// 
// 
// We can build a unit square using these four unit vectors.
// 
// 
// 
// 3)
// {25, 50, 75, 100, -25, -50, -75, -100}
// {100, 75, 50, 25, -100, -75, -50, -25}
// 
// Returns: 31250.0
// 
// 4)
// {100}
// {-100}
// 
// Returns: 0.0
// 
// 5)
// {1, -1, 0}
// {0, 1, -1}
// 
// Returns: 0.5
// 
// 6)
// {0, 1, 1, -2}
// {2, -1, -1, 0}
// 
// Returns: 2.0
// 
// Note that we can append parallel vectors in a row. If we 
// connect all vectors in the order they are given in the 
// input, we get a right triangle with the hypotenuse 
// consisting of the two (1, -1) vectors appended in a row.
// 
// END CUT HERE
#line 144 "VectorPolygon.cpp"

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

typedef vector<int> VI;
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())

class VectorPolygon
{ 
public: 
	double maxArea(vector <int> dx, vector <int> dy) 
	{ 
		double re = 0;
		int N=SZ(dx);
		VI p;p.reserve(10);
		REP(i,1<<N){
			p.clear();
			REP(j,N) if(i&(1<<j)) p.push_back(j);
			do{
				double area = 0;
				int cx=0,cy=0;
				REP(k,SZ(p)) {
					int nx = cx+dx[p[k]],ny=cy+dy[p[k]];
					area += cx*ny-nx*cy;
					cx = nx; cy = ny;
				}
				if( !cx && !cy ) re = max(re,area/2);
			}while( next_permutation(ALL(p)));
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {2, 3, -5}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {2, -4, 2}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 7.0; verify_case(0, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {2, -3, -5}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {2, 4, 2}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 0.0; verify_case(1, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {0, 0, 1, -1}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {1, -1, 0, 0}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 1.0; verify_case(2, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {25, 50, 75, 100, -25, -50, -75, -100}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {100, 75, 50, 25, -100, -75, -50, -25}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 31250.0; verify_case(3, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_4() { int Arr0[] = {100}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {-100}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 0.0; verify_case(4, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_5() { int Arr0[] = {1, -1, 0}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {0, 1, -1}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 0.5; verify_case(5, Arg2, maxArea(Arg0, Arg1)); }
	void test_case_6() { int Arr0[] = {0, 1, 1, -2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arr1[] = {2, -1, -1, 0}; vector <int> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); double Arg2 = 2.0; verify_case(6, Arg2, maxArea(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	VectorPolygon ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
