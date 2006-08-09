// BEGIN CUT HERE
// PROBLEM STATEMENT
// Given a test point, (testPointX, testPointY), and the 
// vertices of a simple polygon, vertices, determine if the 
// test point is in the interior, in the exterior or on the 
// boundary of the polygon.  Return the string "INTERIOR", 
// "EXTERIOR", or "BOUNDARY".
// 
// For simplicity, all sides of the polygon will be 
// horizontal or vertical, and the vertices and the test
// point will all be at integer coordinates. The x and y 
// coordinates of the vertices will given in the vector 
// <string> vertices where each element represents a single 
// vertex, formatted as "<x> <y>". There is an edge from 
// vertices[i] to vertices[i+1] for i from 0 to n-2, and an 
// edge from vertices[n-1] to vertices[0] where n is the 
// number of vertices (and the number of edges) in the polygon.
// 
// DEFINITION
// Class:PointInPolygon
// Method:testPoint
// Parameters:vector <string>, int, int
// Returns:string
// Method signature:string testPoint(vector <string> 
// vertices, int testPointX, int testPointY)
// 
// 
// NOTES
// -A simple polygon is a polygon that may or may not be 
// convex, but self-intersection is not allowed. Not even at 
// a single point.
// 
// 
// CONSTRAINTS
// -vertices will contain an even number of elements between 
// 4 and 50 inclusive.
// -Each element of vertices is formatted as follows "<x> 
// <y>" (quotes for clarity). With exactly one space between 
// <x> and <y> and no leading or trailing spaces.
// -<x> and <y> will consist of an optional minus sign 
// followed by between 1 and 4 digit characters inclusive. 
// There will be no leading zeros.
// -Each <x> and <y> value in each element of vertices will 
// be between -1000 and 1000 inclusive.
// -The elements of vertices taken in order will specify the 
// vertices of a valid simple polygon.
// -Each edge of this polygon will be either exactly 
// horizontal or exactly vertical.
// -No three consecutive vertices will be colinear.
// -No two elements of vertices will be the same.
// -No edges will overlap or intersect, except where adjacent 
// edges meet pairwise at vertices.
// -testPointX and testPointY will both be between -1000 and 
// 1000 inclusive
// 
// 
// EXAMPLES
// 
// 0)
// {"0 0",
//  "0 10",
//  "10 10",
//  "10 0"}
// 5
// 5
// 
// Returns: "INTERIOR"
// 
// A simple example of a square of side 10.
// 
// 1)
// {"0 0",
//  "0 10",
//  "10 10",
//  "10 0"}
// 10
// 15
// 
// Returns: "EXTERIOR"
// 
// Outside the same square.
// 
// 2)
// {"0 0",
//  "0 10",
//  "10 10",
//  "10 0"}
// 5
// 10
// 
// Returns: "BOUNDARY"
// 
// On an edge of the square
// 
// 3)
// {"-100 -90", "-100 100","100 100", "100 -100",
//  "-120 -100","-120 100","-130 100","-130 -110",
//  "110 -110", "110 110", "-110 110","-110 -90"}
// 0
// 0
// 
// Returns: "EXTERIOR"
// 
//  A more complex geometry
// 
// 4)
// {"0 0","0 1000","1000 1000","1000 800",
//  "200 800","200 600","600 600","600 400",
//  "200 400","200 200","1000 200","1000 0"}
// 100
// 500
// 
// Returns: "INTERIOR"
// 
// 
// 
// 5)
// {"0 1000","1000 1000","1000 800",
//  "200 800","200 600","600 600","600 400",
//  "200 400","200 200","1000 200","1000 0","0 0"}
// 322
// 333
// 
// Returns: "EXTERIOR"
// 
// 6)
// {"500 0","500 100","400 100","400 200","300 200",
//  "300 300","200 300","200 400","100 400","100 500",
//  "0 500","0 400","-100 400","-100 300","-200 300",
//   "-200 200","-300 200","-300 100","-400 100","-400 0",
//   "-500 0","-500 -100","-400 -100","-400 -200","-300 -200",
//   "-300 -300","-200 -300","-200 -400","-100 -400","-100 
// -500",
//   "0 -500","0 -400","100 -400","100 -300","200 -300",
//   "200 -200","300 -200","300 -100","400 -100","400 0"}
// 200
// 200
// 
// Returns: "INTERIOR"
// 
// 
// 
// 7)
// {"1 0","2 0","2 1","3 1","3 0","4 0","4 -1","5 -1","5 0",
//  "6 0","6 2","0 2","0 3","-1 3","-1 4","0 4","0 6","1 6",
//  "1 7","0 7","0 8","-2 8","-2 2","-8 2","-8 0","-7 0",
//  "-7 -1","-6 -1","-6 0","-4 0","-4 1","-3 1","-3 0",
//  "-2 0","-2 -6","0 -6","0 -5","1 -5","1 -4","0 -4",
//  "0 -3","-1 -3","-1 -2","0 -2","0 -1","1 -1"} 
// 0
// 0
// 
// Returns: "INTERIOR"
// 
// 
// 
// END CUT HERE
#line 159 "PointInPolygon.cpp"

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

class PointInPolygon
{ 
public: 
	string testPoint(vector <string> vertices, int testPointX, int testPointY) 
	{ 
		vertices.push_back(vertices[0]);
		VI vx(SZ(vertices)),vy(SZ(vertices));
		REP(i,SZ(vertices)){
			ISS in(vertices[i]);
			in >>vx[i]>>vy[i];
		}
		const int OFF = 1001;
		const int SI = OFF*2+1;
		VVI map(SI,VI(SI,0));
		REP(i,SZ(vertices)-1){
			int x=vx[i],y=vy[i];
			int xx=vx[i+1],yy=vy[i+1];
			if( x==xx) {
				if( y > yy ) swap(y,yy);
				FOR(j,y,yy+1) map[x+OFF][j+OFF] = 1;
			}else{
				if( x > xx ) swap(x,xx);
				FOR(j,x,xx+1) map[j+OFF][y+OFF] = 1;
			}
		}
		if( map[testPointX+OFF][testPointY+OFF] == 1 ) return "BOUNDARY";
		map[testPointX+OFF][testPointY+OFF] = 2;
		REP(i,SI-1){
			REP(j,SI)if( map[i][j] == 1){
				if( map[i+1][j] == 2 ) return "INTERIOR";
				map[i+1][j] = 1 - map[i+1][j];
			}
		}

		return "EXTERIOR";
	} 
	
// BEGIN CUT HERE 
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); if ((Case == -1) || (Case == 7)) test_case_7(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"0 0", "10 0", "10 -10", "20 -10", "20 10", "-10 10", "-10 -30", "40 -30", "40 30", "-30 30", "-30 -50", "60 -50", "60 50", "-50 50", "-50 -70", "80 -70", "80 70", "-70 70", "-70 -90", "100 -90", "100 90", "-90 90", "-90 -110", "120 -110", "120 110", "-120 110", "-120 120", "130 120", "130 -120", "-100 -120", "-100 100", "110 100", "110 -100", "-80 -100", "-80 80", "90 80", "90 -80", "-60 -80", "-60 60", "70 60", "70 -60", "-40 -60", "-40 40", "50 40", "50 -40", "-20 -40", "-20 20", "30 20", "30 -20", "0 -20"};
	vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 15; int Arg2 = 0; string Arg3 = "INTERIOR"; verify_case(0, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_1() { string Arr0[] = {"0 0",
 "0 10",
 "10 10",
 "10 0"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 10; int Arg2 = 15; string Arg3 = "EXTERIOR"; verify_case(1, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_2() { string Arr0[] = {"0 0",
 "0 10",
 "10 10",
 "10 0"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 5; int Arg2 = 10; string Arg3 = "BOUNDARY"; verify_case(2, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_3() { string Arr0[] = {"-100 -90", "-100 100","100 100", "100 -100",
 "-120 -100","-120 100","-130 100","-130 -110",
 "110 -110", "110 110", "-110 110","-110 -90"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; int Arg2 = 0; string Arg3 = "EXTERIOR"; verify_case(3, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_4() { string Arr0[] = {"0 0","0 1000","1000 1000","1000 800",
 "200 800","200 600","600 600","600 400",
 "200 400","200 200","1000 200","1000 0"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 100; int Arg2 = 500; string Arg3 = "INTERIOR"; verify_case(4, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_5() { string Arr0[] = {"0 1000","1000 1000","1000 800",
 "200 800","200 600","600 600","600 400",
 "200 400","200 200","1000 200","1000 0","0 0"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 322; int Arg2 = 333; string Arg3 = "EXTERIOR"; verify_case(5, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_6() { string Arr0[] = {"500 0","500 100","400 100","400 200","300 200",
 "300 300","200 300","200 400","100 400","100 500",
 "0 500","0 400","-100 400","-100 300","-200 300",
  "-200 200","-300 200","-300 100","-400 100","-400 0",
  "-500 0","-500 -100","-400 -100","-400 -200","-300 -200",
  "-300 -300","-200 -300","-200 -400","-100 -400","-100 -500",
  "0 -500","0 -400","100 -400","100 -300","200 -300",
  "200 -200","300 -200","300 -100","400 -100","400 0"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 200; int Arg2 = 200; string Arg3 = "INTERIOR"; verify_case(6, Arg3, testPoint(Arg0, Arg1, Arg2)); }
	void test_case_7() { string Arr0[] = {"1 0","2 0","2 1","3 1","3 0","4 0","4 -1","5 -1","5 0",
 "6 0","6 2","0 2","0 3","-1 3","-1 4","0 4","0 6","1 6",
 "1 7","0 7","0 8","-2 8","-2 2","-8 2","-8 0","-7 0",
 "-7 -1","-6 -1","-6 0","-4 0","-4 1","-3 1","-3 0",
 "-2 0","-2 -6","0 -6","0 -5","1 -5","1 -4","0 -4",
 "0 -3","-1 -3","-1 -2","0 -2","0 -1","1 -1"} ; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; int Arg2 = 0; string Arg3 = "INTERIOR"; verify_case(7, Arg3, testPoint(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	PointInPolygon ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
