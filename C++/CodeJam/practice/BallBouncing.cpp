// BEGIN CUT HERE
// PROBLEM STATEMENT
// A ball is moving diagonally on a rectangular board 
// composed of unit squares in rows rows and columns columns. 
// Each square is identified by its row and column number. 
// The lowermost row is marked row 0 and the leftmost column 
// is column 0.
// 
// The ball is initially located in the middle of a starting 
// square given by two integers, startrow and startcol, and 
// is moving diagonally up-right at an angle of 45 degrees. 
// Whenever it reaches a wall, it bounces off it at a right 
// angle (an angle of 90 degrees) and continues moving. If 
// the ball runs into a corner, it bounces back in the 
// opposite direction from which it came..
// 
// 
// 
// A number of holes have been drilled in the board and the 
// ball will fall in upon reaching a square with a hole. 
// Given the board size, starting location and locations of 
// holes, return the number of times the ball will bounce off 
// a wall before falling into a hole, or -1 if the ball will 
// continue bouncing indefinitely.
// 
// DEFINITION
// Class:BallBouncing
// Method:bounces
// Parameters:int, int, int, int, vector <string>
// Returns:int
// Method signature:int bounces(int rows, int columns, int 
// startrow, int startcol, vector <string> holes)
// 
// 
// NOTES
// -Bouncing back from a corner counts as only one bounce.
// 
// 
// CONSTRAINTS
// -rows and columns will be between 1 and 1000000, inclusive.
// -startrow will be between 0 and rows-1, inclusive.
// -startcol will be between 0 and columns-1, inclusive.
// -holes will contain between 0 and 50 elements, inclusive.
// -Each element of holes will be formatted as "row column" 
// (quotes for clarity), where row and column will be 
// integers with no leading zeros representing a square 
// inside the board.
// -There will be no holes at the starting position of the 
// ball.
// -No two holes will occupy the same position on the board.
// 
// 
// EXAMPLES
// 
// 0)
// 8
// 11
// 2
// 1
// { "1 5", "5 3", "4 4" }
// 
// Returns: 3
// 
// This example corresponds to the image in the problem 
// statement.
// 
// 1)
// 6
// 5
// 5
// 3
// { "1 3" }
// 
// Returns: 7
// 
// Bouncing back from a corner counts as only one bounce.
// 
// 2)
// 6
// 7
// 4
// 4
// { }
// 
// Returns: -1
// 
// With no holes, the ball is bound to bounce around for 
// quite a while.
// 
// 3)
// 3
// 3
// 1
// 1
// { "2 2" }
// 
// Returns: 0
// 
// 4)
// 6
// 6
// 0
// 5
// { "4 1", "3 2", "4 3", "2 1", "3 0", "5 2" }
// 
// Returns: -1
// 
// 5)
// 1000000
// 999999
// 66246
// 84332
// { "854350 4982" }
// 
// Returns: 1662562
// 
// 6)
// 5
// 7
// 3
// 4
// { "0 6", "2 3" }
// 
// Returns: 5
// 
// 7)
// 1
// 5
// 0
// 1
// { "0 3" }
// 
// Returns: 2
// 
#line 136 "BallBouncing.cpp"
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

class BallBouncing
{ 
public: 
	int bounces(int rows, int columns, int startrow, int startcol, vector <string> holes) 
	{ 
		if( holes.empty() ) return -1;

		vector<complex<int> > hole;
		REP(i,SZ(holes)){
			ISS in(holes[i]);
			int r,c;in >> r >> c;
			hole.push_back( complex<int>(r*2,c*2));
		}
		vector<pair<complex<int>,complex<int> > > his;
		int re = 0;
		complex<int> p = complex<int>(startrow*2,startcol*2);
		complex<int> dir = complex<int>(2,2);
		while(true){
			complex<int> q = p + dir;
			complex<int> n = q;
			if( q.real() < 0 ) n = complex<int>(q.real()+2,q.imag());
			if( q.real() > rows*2 ) n = complex<int>(q.real()-2,q.imag());
			if( q.imag() < 0 ) n = complex<int>(q.real(),q.imag()+2);
			if( q.imag() > columns*2 ) n = complex<int>(q.real(),q.imag()-2);
			if( find(ALL(hole),n) != hole.end() ) return re;
			if( n != q ){
				complex<int> c = (q+p)/2;
				pair<complex<int>,complex<int> > s = make_pair(p,n);
				if( find(ALL(his), s) != his.end() ){
					return -1;
				}else{
					his.push_back( s );
					re ++;
					dir = (n-c)*2;
				}
			}
			p=n;
		}
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); if ((Case == -1) || (Case == 7)) test_case_7(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 8; int Arg1 = 11; int Arg2 = 2; int Arg3 = 1; string Arr4[] = { "1 5", "5 3", "4 4" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 3; verify_case(0, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_1() { int Arg0 = 6; int Arg1 = 5; int Arg2 = 5; int Arg3 = 3; string Arr4[] = { "1 3" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 7; verify_case(1, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }//
//	void test_case_2() { int Arg0 = 6; int Arg1 = 7; int Arg2 = 4; int Arg3 = 4; string Arr4[] = { }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = -1; verify_case(2, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_3() { int Arg0 = 3; int Arg1 = 3; int Arg2 = 1; int Arg3 = 1; string Arr4[] = { "2 2" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 0; verify_case(3, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_4() { int Arg0 = 6; int Arg1 = 6; int Arg2 = 0; int Arg3 = 5; string Arr4[] = { "4 1", "3 2", "4 3", "2 1", "3 0", "5 2" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = -1; verify_case(4, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_5() { int Arg0 = 1000000; int Arg1 = 999999; int Arg2 = 66246; int Arg3 = 84332; string Arr4[] = { "854350 4982" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 1662562; verify_case(5, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_6() { int Arg0 = 5; int Arg1 = 7; int Arg2 = 3; int Arg3 = 4; string Arr4[] = { "0 6", "2 3" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 5; verify_case(6, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_7() { int Arg0 = 1; int Arg1 = 5; int Arg2 = 0; int Arg3 = 1; string Arr4[] = { "0 3" }; vector <string> Arg4(Arr4, Arr4 + (sizeof(Arr4) / sizeof(Arr4[0]))); int Arg5 = 2; verify_case(7, Arg5, bounces(Arg0, Arg1, Arg2, Arg3, Arg4)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	BallBouncing ___test; 
	___test.run_test(1); 
} 
// END CUT HERE 
