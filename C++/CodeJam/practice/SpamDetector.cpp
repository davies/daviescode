// BEGIN CUT HERE
// PROBLEM STATEMENT
// You are writing part of a spam detection system. Your job 
// is to analyze the subject lines of e-mail messages and 
// return a count of known spam signalling keywords in the 
// subject lines. Your task is made more difficult by the 
// spammers who try to hide the keywords in several ways. 
// Here we will consider just one obfuscation technique: 
// duplicating characters. Duplicating characters means 
// taking an existing character in a word and inserting more 
// copies of that character into the same place in the word. 
// This process can then be repeated on a different character 
// in the word. The spam signalling keyword "credit" might be 
// modified to "creddiT", "CredittT" or "ccrreeeddiitt", 
// etc., but not "credict".
// 
//  For the purposes of this problem we will consider subject 
// lines which contain only letters and spaces. The "words" 
// in the subject line are delimited by spaces. A word in the 
// subject line is considered a "match" if the entire word is 
// the same as at least one entire keyword, after possibly 
// removing some duplicated characters from the subject word. 
// A keyword that matches only part of a subject word or a 
// subject word that matches only part of a keyword does not 
// count. Note that if a keyword contains a double letter, 
// the subject word must also contain (at least) a double 
// letter in the same position to match ("double letter" 
// means two consecutive letters in the word that are the 
// same). For this application, all matches (and the use of 
// the term "same") are case insensitive.
// 
// Given a subject line and a list of keywords, return the 
// count of words in the subject line which "match" words in 
// the keyword list. If multiple words in the subject line 
// match the same keyword, they are each counted, but a word 
// in the subject line that matches multiple keywords is only 
// counted once.
// 
// 
// 
// DEFINITION
// Class:SpamDetector
// Method:countKeywords
// Parameters:string, vector <string>
// Returns:int
// Method signature:int countKeywords(string subjectLine, 
// vector <string> keywords)
// 
// 
// CONSTRAINTS
// -subjectLine will contain between 0 and 50 characters, 
// inclusive.
// -subjectLine will include only letter ('a' to 'z' and 'A' 
// to 'Z') and space (' ') characters.
// -keywords will have between 0 and 50 elements, inclusive.
// -each element of keywords will contain between 1 and 50 
// characters, inclusive.
// -each element of keywords will consist of only letters 
// ('a' to 'z' and 'A' to 'Z').
// -The same letter (ignoring case) never appears more than 
// twice consecutively in any element of keywords.  (ie. 
// "aabbAAbb" is ok, but "aaAbb" is not allowed.)
// 
// 
// EXAMPLES
// 
// 0)
// "LoooW INTEREST RATES available dont BE slow"
// {"interest","rates","loan","available","LOW"}
// 
// Returns: 4
// 
// "INTEREST" , "RATES" , "available", and "LoooW" match. 
// Note that "slow" does not match, even though it contains 
// the substring "low" which is a keyword.
// 
// 1)
// "Dear Richard Get Rich Quick            no risk"
// {"rich","risk","Quicken","wealth","SAVE"}
// 
// Returns: 2
// 
// Don't match "Richard"
// 
// 2)
// "in debbtt againn and aAgain and AGAaiIN"
// {"AGAIN","again","Again","again"}
// 
// Returns: 3
// 
// 3)
// "PlAyy ThEE Lottto     get Loottoo feever"
// {"play","lotto","lottery","looser"}
// 
// Returns: 3
// 
// 4)
// "                                   "
// {"empty","space","does","not","match"}
// 
// Returns: 0
// 
// END CUT HERE
#line 105 "SpamDetector.cpp"

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

class SpamDetector
{ 
public:
	int countKeywords(string line, vector <string> key) 
	{ 
		vector<VC> kc(SZ(key));VVI kn(SZ(key));
		REP(i,SZ(key)){
			REP(j,SZ(key[i])){ 
				char c = tolower(key[i][j]);
				if( kc[i].empty() || kc[i].back() != c){
					kc[i].push_back(c);kn[i].push_back(1);
				}else{
					kn[i].back()++;
				}
			}
		}
		ISS in(line);
		string word;
		int cnt=0;
		while(in>>word){
			VC cc;VI cn;
			REP(j,SZ(word)){ 
				char c = tolower(word[j]);
				if( cc.empty() || cc.back() != c){
					cc.push_back(c);cn.push_back(1);
				}else{
					cn.back()++;
				}
			}
			REP(i,SZ(kc)) if(SZ(cc)==SZ(kc[i])) {
				if( equal(ALL(cc),kc[i].begin() ) && equal(ALL(cn),kn[i].begin(),greater_equal<int>()) ) {
					cnt++;break;
				}
			}
		}
		return cnt;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "LoooW INTEREST RATES available dont BE slow"; string Arr1[] = {"interest","rates","loan","available","LOW"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 4; verify_case(0, Arg2, countKeywords(Arg0, Arg1)); }
	void test_case_1() { string Arg0 = "Dear Richard Get Rich Quick            no risk"; string Arr1[] = {"rich","risk","Quicken","wealth","SAVE"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 2; verify_case(1, Arg2, countKeywords(Arg0, Arg1)); }
	void test_case_2() { string Arg0 = "in debbtt againn and aAgain and AGAaiIN"; string Arr1[] = {"AGAIN","again","Again","again"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 3; verify_case(2, Arg2, countKeywords(Arg0, Arg1)); }
	void test_case_3() { string Arg0 = "PlAyy ThEE Lottto     get Loottoo feever"; string Arr1[] = {"play","lotto","lottery","looser"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 3; verify_case(3, Arg2, countKeywords(Arg0, Arg1)); }
	void test_case_4() { string Arg0 = "                                   "; string Arr1[] = {"empty","space","does","not","match"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); int Arg2 = 0; verify_case(4, Arg2, countKeywords(Arg0, Arg1)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	SpamDetector ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
