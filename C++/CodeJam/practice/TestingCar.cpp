// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// A test driver in a car company tests a new prototype. For 
// each test drive, he is given a list of restrictions, 
// limiting his speed at certain moments in time. The test 
// driver wants to reach as fast a speed as possible. You 
// should write a program which will find the maximal speed 
// he can reach without violating the restrictions.
// 
// 
// You will be given a vector <string> restrictions, giving 
// you the restrictions for the test-drive. Each element of 
// restrictions will contain three integers S, T and D 
// separated by single spaces. The restriction becomes active 
// T seconds after the start of the test-drive and stays 
// active for D seconds. While the restriction is active the 
// car can not go faster than S meters per second. If two or 
// more restrictions are applicable at the same time, the 
// restriction with the minimal speed should be taken into 
// account.
// 
// 
// At the start of the test drive, the car is motionless. 
// Given two ints, duration, the total duration of the test 
// drive in seconds, and acceleration, the maximal possible 
// acceleration of the car in meters per second squared, 
// return the maximal speed allowed by the test-drive 
// restrictions. Please note that acceleration applies both 
// to increasing and decreasing speed (braking).
// 
// 
// DEFINITION
// Class:TestingCar
// Method:maximalSpeed
// Parameters:vector <string>, int, int
// Returns:double
// Method signature:double maximalSpeed(vector <string> 
// restrictions, int duration, int acceleration)
// 
// 
// NOTES
// -The car does not have to come to a complete stop at the 
// final moment of the test drive.  It can still be moving.
// -For the purposes of the problem you may assume the car 
// has no speed limit at all.
// -Your return value must have an absolute or relative error 
// less than 1e-9.
// 
// 
// CONSTRAINTS
// -restrictions will contain between 0 and 50 elements, 
// inclusive.
// -Each element of restrictions will be formatted as "S T D".
// -In each element of restrictions, S, T and D will be 
// separated by single spaces and contain no leading zeroes.
// -In each element of restrictions, S will be an integer 
// between 0 and 100, inclusive.
// -In each element of restrictions, T will be an integer 
// between 0 and duration, inclusive.
// -In each element of restrictions, D will be an integer 
// between 0 and 1000, inclusive.
// -duration will be between 1 and 1000, inclusive.
// -acceleration will be between 1 and 25, inclusive.
// 
// 
// EXAMPLES
// 
// 0)
// {"30 0 200"}
// 100
// 25
// 
// Returns: 30.0
// 
// The restriction doesn't allow us to go faster than 30 
// meters per second.
// 
// 1)
// {"30 0 200"}
// 4
// 5
// 
// Returns: 20.0
// 
// We only have enough time to reach 20 meters per second.
// 
// 2)
// {"50 0 40", "50 60 50"}
// 100
// 10
// 
// Returns: 150.0
// 
// The car can go above 50 meters per second only between the 
// 40th and 60th second. Accelerating from the 40th to 50th 
// second, and braking from the 50th to 60th allows you to 
// reach 150 meters per second.
// 
// 3)
// {"50 30 10", "50 60 50"}
// 100
// 10
// 
// Returns: 175.0
// 
// 4)
// {"0 0 100", "0 200 100", "0 400 100", "0 600 100", "0 800 
// 100"}
// 1000
// 20
// 
// Returns: 2000.0
// 
// 5)
// {"0 0 100", "0 200 100", "0 400 100", "0 600 100", "0 800 
// 100", "0 1000 100"}
// 1000
// 20
// 
// Returns: 1000.0
// 
// 6)
// {"44 422 129", "45 1 29", "72 290 80", "2 1 331", "76 445 
// 16", 
// "76 204 429", "8 372 737", "21 159 538", "71 266 707", "99 
// 73 933",
//  "38 457 879", "42 24 299", "54 349 882", "6 352 909", "26 
// 419 428",
//  "51 327 311", "10 52 898", "75 10 702", "54 263 762", "75 
// 404 223",
//  "43 383 127", "86 433 521", "58 394 306", "33 379 514", 
// "58 239 973",
//  "89 301 765", "47 235 777", "75 355 190", "52 425 38", 
// "59 140 347",
//  "89 220 810", "47 72 724", "3 398 283", "0 224 266", "88 
// 222 615",
//  "25 149 85", "59 221 838", "14 87 86", "44 227 252", "73 
// 330 936",
//  "71 198 138", "54 186 141", "6 128 454", "5 123 719", "7 
// 442 930",
//  "59 174 505", "37 0 581", "9 198 168", "40 391 692", "49 
// 320 419"}
// 462
// 13
// 
// Returns: 7.5
// 
#line 150 "TestingCar.cpp"
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

class TestingCar
{ 
public: 
	double maximalSpeed(vector <string> rest, int duration, int acc) 
	{
		VI time(2001,0xfffffff);
		REP(i,SZ(rest)){
			int s,t,d;
			ISS in(rest[i]); in >> s>>t>>d;
			FOR(j,t,t+d+1) time[j] = min(time[j],s);
		}
		vector< pair<int,int> > res(1,make_pair(0,time[0]));
		FOR(i,1,2001)	if( time[i] != time[i-1] ) res.push_back(make_pair(i,time[i]));
		res.push_back(make_pair(2000,time[2000]));
		VI speed;speed.push_back(0);
		FOR(i,1,SZ(res)) speed.push_back(min(res[i-1].second,res[i].second));
		list<int> up;
		REP(i,SZ(speed)) up.push_back(i);
		while(!up.empty()){
			int p = up.front();
			up.pop_front();
			if( p<SZ(res)-1 ) {
				int d = (res[p+1].first-res[p].first)*acc;
				if( speed[p]> speed[p+1]+d ) {
					speed[p]= speed[p+1]+d;
					up.push_back(p);
				}else if(speed[p]+d < speed[p+1]){
					speed[p+1]= speed[p]+d;
					up.push_back(p+1);
				}
			}
			if( p>0 ) {
				int d = (res[p].first-res[p-1].first)*acc;
				if( speed[p]> speed[p-1]+d ) {
					speed[p]= speed[p-1]+d;
					up.push_back(p);
				}else if(speed[p]+d < speed[p-1]){
					speed[p-1]= speed[p]+d;
					up.push_back(p-1);
				}
			}
		}
		double re = 0;
		REP(i,SZ(speed)-1){
			if( res[i+1].first <= duration ){
				re = max(re, min((double)res[i].second, (acc*(res[i+1].first-res[i].first)+speed[i]+speed[i+1])/2.0));
			}else{
				re = max(re, (double)min(res[i].second, min(acc*(duration-res[i].first)+speed[i],acc*(res[i+1].first-duration)+speed[i+1])));
				break;
			}
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"30 0 200"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 100; int Arg2 = 25; double Arg3 = 30.0; verify_case(0, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_1() { string Arr0[] = {"30 0 200"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; int Arg2 = 5; double Arg3 = 20.0; verify_case(1, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_2() { string Arr0[] = {"50 0 40", "50 60 50"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 100; int Arg2 = 10; double Arg3 = 150.0; verify_case(2, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_3() { string Arr0[] = {"50 30 10", "50 60 50"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 100; int Arg2 = 10; double Arg3 = 175.0; verify_case(3, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_4() { string Arr0[] = {"0 0 100", "0 200 100", "0 400 100", "0 600 100", "0 800 100"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1000; int Arg2 = 20; double Arg3 = 2000.0; verify_case(4, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_5() { string Arr0[] = {"0 0 100", "0 200 100", "0 400 100", "0 600 100", "0 800 100", "0 1000 100"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1000; int Arg2 = 20; double Arg3 = 1000.0; verify_case(5, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }
	void test_case_6() { string Arr0[] = {"44 422 129", "45 1 29", "72 290 80", "2 1 331", "76 445 16", 
"76 204 429", "8 372 737", "21 159 538", "71 266 707", "99 73 933",
 "38 457 879", "42 24 299", "54 349 882", "6 352 909", "26 419 428",
 "51 327 311", "10 52 898", "75 10 702", "54 263 762", "75 404 223",
 "43 383 127", "86 433 521", "58 394 306", "33 379 514", "58 239 973",
 "89 301 765", "47 235 777", "75 355 190", "52 425 38", "59 140 347",
 "89 220 810", "47 72 724", "3 398 283", "0 224 266", "88 222 615",
 "25 149 85", "59 221 838", "14 87 86", "44 227 252", "73 330 936",
 "71 198 138", "54 186 141", "6 128 454", "5 123 719", "7 442 930",
 "59 174 505", "37 0 581", "9 198 168", "40 391 692", "49 320 419"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 462; int Arg2 = 13; double Arg3 = 7.5; verify_case(6, Arg3, maximalSpeed(Arg0, Arg1, Arg2)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	TestingCar ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
