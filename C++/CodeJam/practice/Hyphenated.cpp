// BEGIN CUT HERE
// PROBLEM STATEMENT
// We want to be able to judge whether text is suitable for a 
// particular age 
// group. We will base our judgment on the average length of 
// a word in the text, so we 
// need to define what a "word" is.
// 
// We define a "word" to be a maximal sequence of letters 
// ('A'-'Z' and/or 'a-z') within a
// single line. (Maximal means that if 2 letters are adjacent 
// within a line, they are in the same
// word.) But if a line ends with a sequence of one or more 
// letters immediately 
// followed by a hyphen ('-') and the next line starts with a 
// sequence of one or more letters,
// all these letters are considered to be in the same word. 
// It is even possible that a hyphenated
// word could extend across several lines (see Example 2). 
// 
// Create a class Hyphenated that contains a method avgLength 
// that is given a 
// vector <string> lines containing the lines of text and 
// that returns the average length 
// of the words within the text.
// 
// 
// DEFINITION
// Class:Hyphenated
// Method:avgLength
// Parameters:vector <string>
// Returns:double
// Method signature:double avgLength(vector <string> lines)
// 
// 
// NOTES
// -A return value with either an absolute or relative error 
// of less than 1.0E-9 is considered correct.
// 
// 
// CONSTRAINTS
// -lines will contain between 1 and 50 elements inclusive.
// -Each element of lines will contain between 1 and 50 
// characters inclusive.
// -Each character in each element of lines will be a letter 
// ('A-'Z' or 'a'-'z') or will be one of these:  ' ', '-', '.'
// -At least one element of lines will contain a letter.
// 
// 
// EXAMPLES
// 
// 0)
// {"  now is the ex-", "ample.  "} 
// 
// Returns: 3.75
// 
//    There are 4  words:  now, is, the, example
// 
// 
// 1)
// {"  now is the ex-", " ample.  "}
// 
// Returns: 3.0
// 
// 
//    There are 5 words:  now, is, the, ex, ample
//    Note that the leading blank prevents the joining of ex 
// and ample. Also
//    note that words only consist of letters, so the hyphen 
// is never a part of
//    a word.
// 
// 
// 2)
// {"inter-","national-","ization.."}
// 
// Returns: 20.0
// 
// 
// 
//    There is only one word.
// 
// 3)
// {"All the time I have well-defined  "," trouble."}
// 
// Returns: 4.125
// 
// Note that well-defined consists of 2 separate words.
// 
// 4)
// {"hello-","-","-","-","great"}
// 
// Returns: 5.0
// 
// END CUT HERE
#line 97 "Hyphenated.cpp"

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

class Hyphenated
{ 
public: 
	double avgLength(vector <string> s) 
	{ 
		int l=0,sum=0,cnt=0;
		REP(i,SZ(s)) REP(j,SZ(s[i])) {
			if( isalpha(s[i][j]) ){
				l++;
				if( j==SZ(s[i])-1 ){
					sum+=l;
					cnt++;
					l=0;
				}
			}else if( !(s[i][j] == '-' && j==SZ(s[i])-1 && j-1>=0 && isalpha(s[i][j-1]) &&
				i+1<SZ(s) && SZ(s[i+1])>=1 && isalpha(s[i+1][0])) ) {
				if( l > 0){
					sum+=l;
					cnt++;
					l=0;
				}
			}
		}
		if( l > 0 ) {sum += l; cnt++;}
		return (double)sum/cnt;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"  now is the ex-", "ample.  "} ; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 3.75; verify_case(0, Arg1, avgLength(Arg0)); }
	void test_case_1() { string Arr0[] = {"  now is the ex-", " ample.  "}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 3.0; verify_case(1, Arg1, avgLength(Arg0)); }
	void test_case_2() { string Arr0[] = {"inter-","national-","ization.."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 20.0; verify_case(2, Arg1, avgLength(Arg0)); }
	void test_case_3() { string Arr0[] = {"All the time I have well-defined  "," trouble."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 4.125; verify_case(3, Arg1, avgLength(Arg0)); }
	void test_case_4() { string Arr0[] = {"hello-","-","-","-","great"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); double Arg1 = 5.0; verify_case(4, Arg1, avgLength(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	Hyphenated ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
