// BEGIN CUT HERE
// PROBLEM STATEMENT
// There are some circles on the plane. Circles may lie 
// completely inside other circles, but may not touch or 
// intersect other circles in any other way. Initially, the 
// plane is entirely black. Then circles are drawn in order 
// of non-increasing radii. If a circle is drawn on a black 
// background, it is filled with the color white. If it is 
// drawn on a white background, it is filled black. Your task 
// is to compute the total white area on the plane after 
// drawing all the circles.
// 
// You are given a vector <string> circles containing the 
// circles on the plane. Each element will be formatted as "X 
// Y R", where (X, Y) are the coordinates of the center of 
// the circle, and R is the radius.
// 
// DEFINITION
// Class:BlackWhitePlane
// Method:area
// Parameters:vector <string>
// Returns:double
// Method signature:double area(vector <string> circles)
// 
// 
// NOTES
// -A circle can lie completely inside another circle only if 
// it has a smaller radius than the outer circle.
// -The returned value must be accurate to 1e-9 relative or 
// absolute.
// 
// 
// CONSTRAINTS
// -circles will contain between 1 and 50 elements, inclusive.
// -Each element of circles will be formatted as "X Y R", 
// where X, Y, and R are integers with no leading zeroes.
// -Each X and Y will be between 0 and 10000, inclusive.
// -Each R will be between 1 and 100, inclusive.
// -No two circles in circles will touch or intersect (unless 
// one lies completely inside the other).
// 
// 
// EXAMPLES
// 
// 0)
// {"1 1 1"}
// 
// Returns: 3.141592653589793
// 
// The only circle is drawn on the black plane, so it is 
// white. Its area is pi*R2.
// 
// 1)
// {"4 3 1", "3 2 3", "8 1 1"}
// 
// Returns: 28.274333882308138
// 
// The first circle is drawn inside of the second, so it is 
// black. 
// The total white area is pi*32 + pi*12 - pi*12 = pi*9. 
// 
// 
// 2)
// {"15 15 4", "15 25 4", "25 25 4", "25 15 4", "25 25 100"}
// 
// Returns: 31214.86460606818
// 
// The first four circles are all inside the fifth one.
// 
// 3)
// {"2549 8482 11", "9175 5927 35", "2747 6177 93", "5512 
// 8791 81", "4487 8456 60",
//  "6899 610 77", "9716 2202 3", "9312 5625 79", "4020 9851 
// 85", "1640 7179 54", 
//  "5761 4348 51","5917 3436 88","6547 386 33","182 7676 1","
// 6329 4496 89"}
// 
// Returns: 194250.95695676407
// 
#line 81 "BlackWhitePlane.cpp"
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


typedef vector<int> VI;

#define SZ(v) ((int)(v).size())
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
#define ALL(v) (v).begin(),(v).end()

typedef stringstream SS;
typedef istringstream ISS;

bool cmp(const string& a,const string& b)
{
	ISS in(a);int x;in >> x >> x >>x;
	ISS inn(b);int y;inn >> y >> y >> y;
	return x > y ;
}
class BlackWhitePlane
{ 
public: 
	double area(vector <string> cir) 
	{ 
		int N=SZ(cir);
		sort(ALL(cir),cmp);
		VI vx(N),vy(N),vd(N);
		REP(i,N) {
			ISS in(cir[i]);
			in >> vx[i] >> vy[i] >> vd[i];
		}
		double re = 0;
		VI bw(N,0);
		REP(i,SZ(cir)){
			bw[i] = 1;
			for(int j=i-1;j>=0;j--){
				if( (vx[j]-vx[i])*(vx[j]-vx[i])+(vy[j]-vy[i])*(vy[j]-vy[i]) < vd[j]*vd[j] ){
					bw[i] = 1-bw[j];
				}
			}
			re += (bw[i]*2-1)*3.141592653589793L*vd[i]*vd[i];
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"1 1 1"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 3.141592653589793; verify_case(0, Arg1, area(Arg0)); }
	void test_case_1() { string Arr0[] = {"4 3 1", "3 2 3", "8 1 1"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 28.274333882308138; verify_case(1, Arg1, area(Arg0)); }
	void test_case_2() { string Arr0[] = {"15 15 4", "15 25 4", "25 25 4", "25 15 4", "25 25 100"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 31214.86460606818; verify_case(2, Arg1, area(Arg0)); }
	void test_case_3() { string Arr0[] = {"2549 8482 11", "9175 5927 35", "2747 6177 93", "5512 8791 81", "4487 8456 60",
 "6899 610 77", "9716 2202 3", "9312 5625 79", "4020 9851 85", "1640 7179 54", 
 "5761 4348 51","5917 3436 88","6547 386 33","182 7676 1","6329 4496 89"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 194250.95695676407; verify_case(3, Arg1, area(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	BlackWhitePlane ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
