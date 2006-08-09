// BEGIN CUT HERE
// PROBLEM STATEMENT
// The Haar wavelet transform is possibly the earliest 
// wavelet transform, introduced by Haar in 1909. The 1-
// dimensional version
// of this transform operates on a sequence of integer data 
// as follows: First separate the sequence into pairs of 
// adjacent values, 
// starting with the first and second values, then the third 
// and fourth values, etc. Next, calculate the sums of each 
// of these pairs, 
// and place the sums in order into a new sequence. Then, 
// calculate the differences of each of the pairs (subtract 
// the second value of 
// each pair from the first value), and append the 
// differences in order to the end of the new sequence. The 
// resulting sequence is a level-1 transform.
// Note that this requires the input sequence to have an even 
// number of elements.
// 
// 
// The above describes a level-1 transform. To perform a 
// level-2 transform, we repeat the above procedure on the 
// first half of the sequence obtained from the level-1 
// transform. 
// The second half of the sequence remains unchanged from the 
// previous level. This pattern continues for higher level 
// transforms (i.e., a level-3 
// transform operates with the first quarter of the sequence, 
// and so on). Note that this is always possible when the 
// number of elements is a power of 2.
// 
// 
// Given the output of a level-L 1D Haar transform in vector 
// <int> transformedData, create a class InverseHaar1D with a 
// method transform which returns the original data. That is, 
// when a level-L 1D Haar transform is applied to your return 
// value, one should obtain transformedData.
// 
// 
// See the examples for clarification.
// 
// 
// DEFINITION
// Class:InverseHaar1D
// Method:transform
// Parameters:vector <int>, int
// Returns:vector <int>
// Method signature:vector <int> transform(vector <int> 
// transformedData, int L)
// 
// 
// CONSTRAINTS
// -transformedData will contain exactly 2, 4, 8, 16 or 32 
// elements.
// -Each element of transformedData will be between -10000 
// and 10000, inclusive.
// -L will be between 1 and log2(# of elements in 
// transformedData) inclusive.
// -The elements of transformedData will constitute a valid 
// Haar transform.
// 
// 
// EXAMPLES
// 
// 0)
// {101, -53}
// 1
// 
// Returns: {24, 77 }
// 
// Consider the sequence {24, 77}.
// Then, the level-1 Haar transform is simply {24+77, 24-77} 
// = {101, -53}, which is exactly transformedData.
// 
// 1)
// {3, 8, -1, -2}
// 1
// 
// Returns: {1, 2, 3, 5 }
// 
// Consider the sequence {1, 2, 3, 5}.
// 
// Start by forming 3=1+2, the sum of the first pair; 8=3+5, 
// the sum of the second pair; -1=1-2, the difference of the 
// first pair; and finally, -2=3-5, the difference of the 
// second pair. To form the output, we create a sequence of 
// the sums in order, and the differences in order.
// 
// Then, the output for the Haar wavelet transform would be 
// {3, 8, -1, -2}, which is exactly transformedData.
// 
// 2)
// {11, -5, -1, -2}
// 2
// 
// Returns: {1, 2, 3, 5 }
// 
// From the previous example, the level-1 Haar transform of 
// {1, 2, 3, 5} gives {3, 8, -1, -2}.
// Then, the level-2 transform of {1, 2, 3, 5} is simply {11, 
// -5, -1, -2} (11=3+8, -5=3-8), which is once again
// exactly transformedData.
// 
// 3)
// {369, 477, 451, 262, 47, 135, 
//  -125, -2, 18, -23, 30, 101, 
//  -5, -18, 54, -20, 11, 45, -5, 
//  70, -24, 2, -50, 15, 55, -62, 
//  -23, -17, 44, -8, -44, -52}
// 3
// 
// Returns: {62, 51, 70, 25, 32, 37, 81, 11, 72, 96, 70, 68, 
// 43, 93, 25, 10, 67, 12, 11, 73, 56, 79, 68, 85, 68, 24, 
// 15, 23, 6, 50, 12, 64 }
// 
// END CUT HERE
#line 119 "InverseHaar1D.cpp"

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
typedef map<int,int> MII;
typedef map<string,int> MSI;
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

class InverseHaar1D
{ 
public: 
	vector <int> transform(vector <int> data, int L) 
	{ 
		vector <int> re(data);
		while(L>0){
			int half=SZ(data)>>L;
			REP(i,half) re[i*2] = (data[i]+data[half+i])>>1;
			REP(i,half) re[i*2+1] = (data[i]-data[half+i])>>1;
			data=re;
			L--;
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const vector <int> &Expected, const vector <int> &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: " << print_array(Expected) << endl; cerr << "\tReceived: " << print_array(Received) << endl; } }
	void test_case_0() { int Arr0[] = {101, -53}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; int Arr2[] = {24, 77 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(0, Arg2, transform(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {3, 8, -1, -2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; int Arr2[] = {1, 2, 3, 5 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(1, Arg2, transform(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {11, -5, -1, -2}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; int Arr2[] = {1, 2, 3, 5 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(2, Arg2, transform(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {369, 477, 451, 262, 47, 135, 
 -125, -2, 18, -23, 30, 101, 
 -5, -18, 54, -20, 11, 45, -5, 
 70, -24, 2, -50, 15, 55, -62, 
 -23, -17, 44, -8, -44, -52}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; int Arr2[] = {62, 51, 70, 25, 32, 37, 81, 11, 72, 96, 70, 68, 43, 93, 25, 10, 67, 12, 11, 73, 56, 79, 68, 85, 68, 24, 15, 23, 6, 50, 12, 64 }; vector <int> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); verify_case(3, Arg2, transform(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	InverseHaar1D ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
