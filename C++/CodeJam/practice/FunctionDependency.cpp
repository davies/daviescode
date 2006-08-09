// BEGIN CUT HERE
// PROBLEM STATEMENT
// You are developing a database platform that supports, 
// among other things, user defined functions.  User-defined 
// functions may call internal functions, other user-defined 
// functions, or both.  A user-defined function, FuncA, is 
// said to be dependent on another function, FuncB, if it 
// calls FuncB.
// 
// You have been tasked with writing a module that will 
// "script" (list out the code for) all of the functions in a 
// given database.  However, because of dependency issues, 
// they must be scripted in a specific order that meets the 
// following criteria:
// 
// 
// A function cannot be scripted until all of its dependent 
// functions have already been scripted.
// Whenever more than one function could be scripted (subject 
// to the first requirement), the one whose name occurs first 
// lexicographically will be scripted first.
// 
// 
// You are given a vector <string> funcs, each element of 
// which is the name of a function in the database.  You are 
// also given a vector <string> depends, a list describing 
// the dependency of each function.  Each element of depends 
// is a space-delimited list of integers.  Each element of 
// depends corresponds to the element of funcs with the same 
// index.  Each number represented in each element of depends 
// refers to the 0-based index of another function.
// 
// For instance, suppose a database has two functions, A and 
// B, each of which is dependent on a third function, C.  The 
// input might then be given by:
// 
// {"B", "C", "A"}
// {"1", "", "1"}
// 
// 
// Now, since functions A and B both depend upon C, we have 
// to script C first.  After 
// that, either A or B could be scripted, since C has already 
// been scripted.  We use the second 
// rule to determine that we should script A first.  Our 
// return is thus:
// 
// {"C", "A", "B"}
// 
// 
// You are to return a String[] containing the names of the 
// functions in the order in which they should be scripted.
// 
// DEFINITION
// Class:FunctionDependency
// Method:scriptingOrder
// Parameters:vector <string>, vector <string>
// Returns:vector <string>
// Method signature:vector <string> scriptingOrder(vector 
// <string> funcs, vector <string> depends)
// 
// 
// CONSTRAINTS
// -funcs will contain between 1 and 50 elements, inclusive.
// -depends will contain the same number of elements as funcs.
// -Each element of funcs will contain between 1 and 50 upper 
// case ('A'-'Z') character, inclusive.
// -Each element of depends will be a space-delimited list of 
// integers, with no leading zeroes.
// -Each integer represented in depends will be between 0 and 
// n-1, inclusive, where n is the number of elements in funcs.
// -There will be no circular dependencies.
// 
// 
// EXAMPLES
// 
// 0)
// {"B", "C", "A"}
// {"1", "", "1"}
// 
// Returns: {"C", "A", "B" }
// 
// The example from the problem statement.
// 
// 1)
// {"B", "C", "A"}
// {"1", "", "0"}
// 
// Returns: {"C", "B", "A" }
// 
// 2)
// {"K", "A", "B", "C", "D", "E", "F", "G", "H", "I"}
// {"", "", "1 1", "2", "3", "4", "5", "6", "7", "8"}
// 
// Returns: {"A", "B", "C", "D", "E", "F", "G", "H", "I", "K" }
// 
// Careful!  It is permissible for the same dependency to be 
// listed twice.
// 
#line 101 "FunctionDependency.cpp"
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

typedef vector<string> VS;


#define SZ(v) ((int)(v).size())
#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)


typedef istringstream ISS;

class FunctionDependency
{ 
public: 
	vector <string> scriptingOrder(vector <string> funcs, vector <string> depends) 
	{ 
		vector <string> re;
		int N=SZ(funcs);

		map<string,set<string> > dep;
		REP(i,N){
			ISS in(depends[i]);
			int d=0;
			dep[funcs[i]];
			while( in>>d ) dep[funcs[i]].insert(funcs[d]);
		}
		REP(i,N){
			set<string> avl;
			for(map<string,set<string> >::iterator it=dep.begin();it!=dep.end();it++){
				if ( it->second.empty() )avl.insert( it->first );
			}
			if( avl.empty() ) return VS();
			string b = *avl.begin();
			dep.erase(b);
			for(map<string,set<string> >::iterator it=dep.begin();it!=dep.end();it++){
				it->second.erase(b);
			}
			re.push_back(b);
		}

		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <string> &Expected, const vector <string> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { string Arr0[] = {"B", "C", "A"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"1", "", "1"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"C", "A", "B" }; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(0, Arg2, scriptingOrder(Arg0, Arg1)); }
	void test_case_1() { string Arr0[] = {"B", "C", "A"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"1", "", "0"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"C", "B", "A" }; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(1, Arg2, scriptingOrder(Arg0, Arg1)); }
	void test_case_2() { string Arr0[] = {"K", "A", "B", "C", "D", "E", "F", "G", "H", "I"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"", "", "1 1", "2", "3", "4", "5", "6", "7", "8"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "K" }; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(2, Arg2, scriptingOrder(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	FunctionDependency ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
