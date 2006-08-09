// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// An unnamed international contest just finished. There were 
// exactly four contestants from each of the participating 
// countries.
// During the contest each of the contestants achieved a non-
// negative integer score (the higher, the better).
// The contestants were sorted according to their scores and 
// the first part of the overall results (i.e., the best few 
// contestants)
// was announced during the final ceremony.
// The organizers of the contest decided not to publish the 
// remaining, lower part of the results.
// 
// 
// 
// In the Countries Ranklist the countries are ordered (in 
// decreasing order) by the total score of their four 
// contestants. 
// If two or more countries have the same score, they are 
// tied for the best place from the corresponding interval,
// and the places of the lower ranked countries remain 
// unaffected. For example, if the total scores of countries 
// A, B, C and D
// are 100, 90, 90 and 80, respectively, then B and C are 
// tied for second place, and D is fourth. For further 
// clarification, see examples 2 and 4.
// 
// 
// 
// A vector <string> knownResults will represent the 
// published part of the results, with each of the elements 
// describing one of the announced contestants.
// The elements have the form "COUNTRY CONTESTANT SCORE", 
// where COUNTRY is the name of the country, CONTESTANT is 
// the name of the contestant 
// and SCORE is his score. 
// 
// 
// 
// Your task will be to compute the best and the worst 
// possible placement in the Countries Ranklist
// for each of the participating countries.
// You shall assume that from each country at least one 
// contestant was announced and that 
// all contestants not in the available part of the results 
// scored strictly less 
// than the worst contestant in the available part of 
// rankings. (For example, if the worst announced
// contestant scored 47 points, then each of the not 
// announced contestants from each of the participating
// countries could have scored at most 46 points.)
// 
// 
// 
// You are to return a vector <string> with one element for 
// each country. 
// The form of each element must be "COUNTRY BEST WORST", 
// where COUNTRY is the name of the country, 
// BEST and WORST are the best and the worst position this 
// country could possibly have in the Countries Ranklist.
// Order this list so that the country names are given in 
// alphabetical order. Note that country names are case 
// sensitive, and that in alphabetical order all uppercase 
// letters come before lowercase letters. The numbers BEST 
// and WORST mustn't contain leading zeroes.
// 
// 
// DEFINITION
// Class:CountriesRanklist
// Method:findPlaces
// Parameters:vector <string>
// Returns:vector <string>
// Method signature:vector <string> findPlaces(vector 
// <string> knownPoints)
// 
// 
// NOTES
// -All scores (even the unknown ones) are non-negative 
// integers.
// 
// 
// CONSTRAINTS
// -knownResults contains between 1 and 50 elements, inclusive.
// -Each of the elements in knownResults is of the form 
// "COUNTRY CONTESTANT SCORE".
// -Each COUNTRY and CONTESTANT in knownResults are strings 
// containing between 1 and 10 letters ('a'-'z', 'A'-'Z').
// -For each country knownResults contains at most 4 
// contestants.
// -Each SCORE in knownResults is an integer between 1 and 
// 600, inclusive, with no leading zeroes.
// 
// 
// EXAMPLES
// 
// 0)
// {"Poland Krzysztof 101", "Ukraine Evgeni 30", "Ukraine 
// Ivan 24"}
// 
// Returns: {"Poland 1 1", "Ukraine 2 2" }
// 
// The worst announced contestant is Ivan with 24 points. 
// Each of the contestants that weren't announced had to 
// score strictly less, i.e., at most 23 points. Thus the 
// total score of Ukraine is at most 30+24+23+23 = 100 and 
// Poland surely wins.
// 
// 1)
// {"Poland Krzysztof 100", "CzechRep Martin 30", "CzechRep 
// Jirka 25"}
// 
// Returns: {"CzechRep 1 2", "Poland 1 2" }
// 
// This time, if the two missing Czech competitors scored 24 
// points each (and the remaining three from Poland scored 
// 0), Czech Republic could still win. Note the order in 
// which the countries are reported in the output.
// 
// 2)
// {"Slovakia Marian 270", "Hungary Istvan 24", "Poland 
// Krzysztof 100", 
//  "Hungary Gyula 30", "Germany Tobias 27", "Germany Juergen 
// 27"}
// 
// Returns: {"Germany 2 4", "Hungary 2 4", "Poland 2 2", 
// "Slovakia 1 1" }
// 
// This is an interesting case. Slovakia is sure to win, and 
// Poland is sure to be second. But it is possible that 
// Germany, Hungary and Poland have an equal total score of 
// 100. In this case they are all tied for second place.
// 
// 3)
// {"usa Jack 14", "USA Jim 10", "USA Jim 10", "USA Jim 10", 
//  "USA Jim 10", "usa Jack 14", "usa Jack 14", "Zimbabwe 
// Jack 10"}
// 
// Returns: {"USA 2 2", "Zimbabwe 3 3", "usa 1 1" }
// 
// Case matters, "USA" and "usa" are two different countries. 
// Contestant names don't matter, i.e., from "USA" there are 
// four different contestants, all named "Jim". 
// 
// 4)
// {"A a 9", "A b 9", "A c 9", "A d 9", 
//  "B e 9", "B f 9", "B g 8", "B h 8",
//  "C i 9", "C j 9", "C k 9", "C l 7",
//  "D m 1", "D n 1", "D o 1", "D p 1"}
// 
// Returns: {"A 1 1", "B 2 2", "C 2 2", "D 4 4" }
// 
// All results have been announced, so everything is clear. A 
// is first, B and C are tied for second, and D is fourth.
// 
#line 158 "CountriesRanklist.cpp"
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

class CountriesRanklist
{ 
public: 
	vector <string> findPlaces(vector <string> knownPoints) 
	{ 
		vector <string> re;
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <string> &Expected, const vector <string> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { string Arr0[] = {"Poland Krzysztof 101", "Ukraine Evgeni 30", "Ukraine Ivan 24"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"Poland 1 1", "Ukraine 2 2" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(0, Arg1, findPlaces(Arg0)); }
	void test_case_1() { string Arr0[] = {"Poland Krzysztof 100", "CzechRep Martin 30", "CzechRep Jirka 25"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"CzechRep 1 2", "Poland 1 2" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(1, Arg1, findPlaces(Arg0)); }
	void test_case_2() { string Arr0[] = {"Slovakia Marian 270", "Hungary Istvan 24", "Poland Krzysztof 100", 
 "Hungary Gyula 30", "Germany Tobias 27", "Germany Juergen 27"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"Germany 2 4", "Hungary 2 4", "Poland 2 2", "Slovakia 1 1" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(2, Arg1, findPlaces(Arg0)); }
	void test_case_3() { string Arr0[] = {"usa Jack 14", "USA Jim 10", "USA Jim 10", "USA Jim 10", 
 "USA Jim 10", "usa Jack 14", "usa Jack 14", "Zimbabwe Jack 10"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"USA 2 2", "Zimbabwe 3 3", "usa 1 1" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(3, Arg1, findPlaces(Arg0)); }
	void test_case_4() { string Arr0[] = {"A a 9", "A b 9", "A c 9", "A d 9", 
 "B e 9", "B f 9", "B g 8", "B h 8",
 "C i 9", "C j 9", "C k 9", "C l 7",
 "D m 1", "D n 1", "D o 1", "D p 1"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"A 1 1", "B 2 2", "C 2 2", "D 4 4" }; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); verify_case(4, Arg1, findPlaces(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	CountriesRanklist ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
