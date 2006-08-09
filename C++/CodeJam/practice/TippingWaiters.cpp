// BEGIN CUT HERE
// PROBLEM STATEMENT
// In a restaurant, if you were pleased by the waiter's 
// service, you may leave him a tip -- you pay him more than 
// the actual value of the bill, and the waiter keeps the 
// excess money. In some countries, not leaving a tip for the 
// waiter is even considered impolite. 
// 
// During my recent holiday I was having dinner in a foreign 
// restaurant. The pamphlet from my travel agency informed me 
// that the proper way of tipping the waiter is the following:
// 
// The sum I pay must be round, i.e., divisible by 5.
// The tip must be between 5% and 10% of the final sum I pay, 
// inclusive.
// 
// 
// Clearly, sometimes there may be multiple "correct" ways of 
// settling the bill. I'd like to know exactly how many 
// choices I have in a given situation. I could program it 
// easily, but I was having a holiday... and so it's you who 
// has to solve this task.
// 
// You will be given:
// 
// an int bill -- the amount I have to pay for the dinner
// an int cash -- the amount of money I have in my pocket
// 
// Write a function that computes how many different final 
// sums satisfy the conditions above.
// 
// DEFINITION
// Class:TippingWaiters
// Method:possiblePayments
// Parameters:int, int
// Returns:int
// Method signature:int possiblePayments(int bill, int cash)
// 
// 
// NOTES
// -Assume that both bill and cash are in dollars.
// -All the money I have is in one-dollar banknotes.
// 
// 
// CONSTRAINTS
// -bill and cash are between 1 and 2,000,000,000, inclusive.
// -bill doesn't exceed cash.
// 
// 
// EXAMPLES
// 
// 0)
// 4
// 100
// 
// Returns: 0
// 
// 4 isn't a round sum, and 5 is too much.
// 
// 1)
// 23
// 100
// 
// Returns: 1
// 
// The only correct choice is to pay 25 dollars, thus leaving 
// a tip of 2 dollars.
// 
// 2)
// 23
// 24
// 
// Returns: 0
// 
// The same bill, but I don't have enough money to leave an 
// appropriate tip.
// 
// 3)
// 220
// 239
// 
// Returns: 1
// 
// This time, it is appropriate to pay either 235 or 240 
// dollars. Sadly, I don't have enough money for the second 
// possibility.
// 
// 4)
// 1234567
// 12345678
// 
// Returns: 14440
// 
// A large bill, but with that much money I don't care.
// 
// 5)
// 1880000000
// 1980000000
// 
// Returns: 210527
// 
// 6)
// 171000000
// 179999999
// 
// Returns: 0
// 
#line 109 "TippingWaiters.cpp"
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

class TippingWaiters
{ 
public: 
	int possiblePayments(int bill, int cash) 
	{ 
		int mi = ceil(bill/0.95);
		int ma = floor(bill/0.9);
		ma = min(ma,cash);
		while(mi%5!=0)mi++;
		while(ma%5!=0)ma--;
		return (ma-mi)/5+1;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 4; int Arg1 = 100; int Arg2 = 0; verify_case(0, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_1() { int Arg0 = 23; int Arg1 = 100; int Arg2 = 1; verify_case(1, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_2() { int Arg0 = 23; int Arg1 = 24; int Arg2 = 0; verify_case(2, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_3() { int Arg0 = 220; int Arg1 = 239; int Arg2 = 1; verify_case(3, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_4() { int Arg0 = 1234567; int Arg1 = 12345678; int Arg2 = 14440; verify_case(4, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_5() { int Arg0 = 1880000000; int Arg1 = 1980000000; int Arg2 = 210527; verify_case(5, Arg2, possiblePayments(Arg0, Arg1)); }
	void test_case_6() { int Arg0 = 171000000; int Arg1 = 179999999; int Arg2 = 0; verify_case(6, Arg2, possiblePayments(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	TippingWaiters ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
