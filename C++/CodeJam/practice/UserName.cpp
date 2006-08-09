// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// 
// You are implementing the member registration system of an 
// online dating
// site. When a new member signs up, it is possible that she 
// initially
// chooses the same username as an existing member. The 
// system must then
// inform the new member of the conflict and suggest a 
// variant of the chosen
// name with a number attached to the end.
// 
// 
// 
// 
// If an existing member is named "FunkyMonkey", for example, 
// and
// a new member wants the same username, the simplest 
// suggestion the system
// can make is "FunkyMonkey1".  If there is already a member by
// that name, the system must suggest "FunkyMonkey2", unless 
// that 
// variant is also taken. If all names from "FunkyMonkey1"
// through "FunkyMonkey9" are taken as well as the
// original "FunkyMonkey", the system moves on to consider 
// "FunkyMonkey10", and so on. The goal is to use the 
// smallest possible
// number in the variant. Note that each username consists of 
// letters (the characters
// from 'a' to 'z' and from 'A' to 'Z') and numerals ('0' to 
// '9').
// 
// 
// 
// 
// You are given a vector <string>, existingNames, containing
// all usernames that have already been registered in the 
// system. You are
// also given a single string, newName, containing the
// username that a new member wants to use. In the event of a 
// conflict, this
// member will accept the suggestion offered by your system 
// in accordance
// with the principles above. Return a string containing the
// username finally assigned to the new member.
// 
// 
// 
// 
// 
// DEFINITION
// Class:UserName
// Method:newMember
// Parameters:vector <string>, string
// Returns:string
// Method signature:string newMember(vector <string> 
// existingNames, string newName)
// 
// 
// NOTES
// -The constraints rule out names that end in a number with 
// a leading zero, such as "grokster006" and "bart0".
// 
// 
// CONSTRAINTS
// -existingNames contains between 1 and 50 elements, inclusive
// -each element of existingNames is between 1 and 50 
// characters long, inclusive
// -the only characters permitted in elements of 
// existingNames are 'a' to 'z', 'A' to 'Z', and '0' to '9'
// -no element of existingNames ends in a number that has a 
// leading zero
// -newName is between 1 and 50 characters long, inclusive
// -the only characters permitted in newName are 'a' to 'z' 
// and 'A' to 'Z'
// 
// 
// EXAMPLES
// 
// 0)
// {"MasterOfDisaster", "DingBat", "Orpheus", "WolfMan", 
// "MrKnowItAll"}
// "TygerTyger"
// 
// Returns: "TygerTyger"
// 
// "TygerTyger" is available.
// 
// 1)
// {"MasterOfDisaster", "TygerTyger1", "DingBat", "Orpheus", 
//  "TygerTyger", "WolfMan", "MrKnowItAll"}
// "TygerTyger"
// 
// Returns: "TygerTyger2"
// 
// "TygerTyger" and "TygerTyger1" are taken.
// 
// 2)
// {"TygerTyger2000", "TygerTyger1", "MasterDisaster", 
// "DingBat", 
//  "Orpheus", "WolfMan", "MrKnowItAll"}
// "TygerTyger"
// 
// Returns: "TygerTyger"
// 
// There are higher-numbered variants of "TygerTyger", but 
// the base name is available.
// 
// 3)
// {"grokster2", "BrownEyedBoy", "Yoop", "BlueEyedGirl", 
//  "grokster", "Elemental", "NightShade", "Grokster1"}
// "grokster"
// 
// Returns: "grokster1"
// 
// Note that "Grokster1" is not the same as "grokster1".
// 
// 4)
// {"Bart4", "Bart5", "Bart6", "Bart7", "Bart8", "Bart9", 
// "Bart10",
//  "Lisa", "Marge", "Homer", "Bart", "Bart1", "Bart2", 
// "Bart3",
//  "Bart11", "Bart12"}
// "Bart"
// 
// Returns: "Bart13"
// 
// END CUT HERE
#line 132 "UserName.cpp"

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

class UserName
{ 
public: 
	string newMember(vector <string> exist, string newName) 
	{ 
		string re = newName;
		if( FOUND(exist,re) ){
			int i=1;
			re=newName+cast<int,string>(i);
			while( FOUND(exist,re) ){
				i++;
				re=newName+cast<int,string>(i);
			}
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"MasterOfDisaster", "DingBat", "Orpheus", "WolfMan", "MrKnowItAll"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "TygerTyger"; string Arg2 = "TygerTyger"; verify_case(0, Arg2, newMember(Arg0, Arg1)); }
	void test_case_1() { string Arr0[] = {"MasterOfDisaster", "TygerTyger1", "DingBat", "Orpheus", 
 "TygerTyger", "WolfMan", "MrKnowItAll"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "TygerTyger"; string Arg2 = "TygerTyger2"; verify_case(1, Arg2, newMember(Arg0, Arg1)); }
	void test_case_2() { string Arr0[] = {"TygerTyger2000", "TygerTyger1", "MasterDisaster", "DingBat", 
 "Orpheus", "WolfMan", "MrKnowItAll"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "TygerTyger"; string Arg2 = "TygerTyger"; verify_case(2, Arg2, newMember(Arg0, Arg1)); }
	void test_case_3() { string Arr0[] = {"grokster2", "BrownEyedBoy", "Yoop", "BlueEyedGirl", 
 "grokster", "Elemental", "NightShade", "Grokster1"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "grokster"; string Arg2 = "grokster1"; verify_case(3, Arg2, newMember(Arg0, Arg1)); }
	void test_case_4() { string Arr0[] = {"Bart4", "Bart5", "Bart6", "Bart7", "Bart8", "Bart9", "Bart10",
 "Lisa", "Marge", "Homer", "Bart", "Bart1", "Bart2", "Bart3",
 "Bart11", "Bart12"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arg1 = "Bart"; string Arg2 = "Bart13"; verify_case(4, Arg2, newMember(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	UserName ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
