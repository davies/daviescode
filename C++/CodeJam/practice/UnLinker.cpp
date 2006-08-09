// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// 
// You are implementing the portion of an online dating site 
// where members
// display their profiles. Most of the profile content is 
// automatically
// generated from member data. Part of a profile, however, is 
// furnished
// directly by the member as free-form text. Weblinks 
// frequently crop up in
// this text, despite a site policy that forbids 
// advertisement and linking
// of any kind. Your job is to seek and destroy all weblinks 
// in a given
// piece of text.
// 
// 
// 
// 
// For the purposes of this problem, a weblink is a string 
// consisting
// of three parts. From left to right, these are the prefix,
// domain, and suffix.
// 
// 
// 
// 
// The prefix consists of one of the three following strings.
// 
// 
// 
// 
//     http://
//     http://www.
//     www.
// 
// 
// 
// The domain is a sequence of one or more characters, each 
// of which
// is a letter (a character from 'a' to 'z' or from 'A' to 
// 'Z'), a
// numeral ('0' to '9'), or a period (the character '.').
// 
// 
// 
// 
// The suffix is one of the five following strings.
// 
// 
// 
// 
//     .com
//     .org
//     .edu
//     .info
//     .tv
// 
// 
// 
// There must be no space character within the weblink. The 
// weblink may have
// any kind of character to the left and right of it. It may 
// also occur at
// the beginning of the text, at the end of the text, or it 
// may itself be
// the entire text.
// 
// 
// 
// 
// Furthermore, a weblink extends as far as possible to the 
// left
// and right. The passage below, for example, contains a 
// single weblink
// stretching between the fragments "espi" and "aladocious".
// 
//  
// 
// 
//     espihttp://www.tv.org.superwww.cali.comaladocious
// 
// 
// 
// 
// Each weblink in the text must be replaced with a string 
// consisting of
// the word "OMIT" followed by a number. The first weblink is 
// to be
// replaced with "OMIT1", the second with "OMIT2",
// and so forth. All portions of text that do not consist of 
// a weblink
// must remain intact.
// 
// 
// 
// 
// You are given a string consisting of letters, numerals ('0'
// to '9') and the characters ' ' (space), ',' (comma), '.' 
// (period), ':' (colon), and
// '/' (slash). Return a string containing the text after it
// has been cleaned in the manner described above.
// 
// 
// 
// 
// DEFINITION
// Class:UnLinker
// Method:clean
// Parameters:string
// Returns:string
// Method signature:string clean(string text)
// 
// 
// CONSTRAINTS
// -text is between 1 and 50 characters long, inclusive
// -each character in text is a letter ('a' to 'z' or 'A' to 
// 'Z'), a numeral ('0' to '9'), or one of the following: ' ' 
// (space); ',' (comma); '.' (period); ':' (colon); '/' 
// (slash).
// 
// 
// EXAMPLES
// 
// 0)
// "espihttp://www.tv.org.superwww.cali.comaladocious"
// 
// Returns: "espiOMIT1aladocious"
// 
// Remember, a weblink stretches as far as possible to the 
// left and right.
// 
// 1)
// "check www.foo.com 4 www.foo.com www.scoopz.com"
// 
// Returns: "check OMIT1 4 OMIT2 OMIT3"
// 
// Each occurrence of "www.foo.com" gets a different number.
// 
// 2)
// "check www.foo.com 4 www.foo.comwww.scoopz.com"
// 
// Returns: "check OMIT1 4 OMIT2"
// 
// What a difference a space makes!
// 
// 3)
// "check www.foo.com 4 www.foo.comhttp://scoopz.com"
// 
// Returns: "check OMIT1 4 OMIT2OMIT3"
// 
// The last two weblinks are separate again.
// 
// 4)
// "http://411.com goodz 4 www.733t.com, 2http://..com"
// 
// Returns: "OMIT1 goodz 4 OMIT2, 2OMIT3"
// 
// 5)
// "http.//say.org,www.jeeves.x.info,www.comhttp://.tv"
// 
// Returns: "http.//say.org,OMIT1,www.comhttp://.tv"
// 
// 6)
// "http://www.www.com/www"
// 
// Returns: "OMIT1/www"
// 
// END CUT HERE
#line 173 "UnLinker.cpp"

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

class UnLinker
{ 
public: 
	int isLink(string s){
		int of = 0;
		string prefix[] = {"http://","www."};
		REP(i,2) if( s.find(prefix[i]) == 0 ) of = SZ(prefix[i]);
		if( of == 0 ) return 0;
		int st=of;
		while(st<SZ(s) && (isalpha(s[st])|| isdigit(s[st]) || s[st]=='.')){
			st++;
		}
		string url=s.substr(of,st-of);
		string suffix[] = {".com",".org",".edu",".info",".tv"};
		int id=-1, p=0;
		for(int i=SZ(url)-3;i>0 && id==-1;i--){
			REP(j,5)if( url.substr(i).find(suffix[j])==0) {
				id = j;
				p = i;
				break;
			}
		}
		if( id < 0 ) 
			return 0;
		return of+p+SZ(suffix[id])-1;
	}
	string clean(string text) 
	{ 
		string re;
		int cnt=0;
		REP(i,SZ(text)){
			int l = 0;
			if( (l=isLink( text.substr(i)))>0 ){
				re += "OMIT" + cast<int,string>(++cnt);
				i += l;
			}else{
				re.push_back(text[i]);
			}
		}
		return re;
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "espihttp://www.tv.org.superwww.cali.comaladocious"; string Arg1 = "espiOMIT1aladocious"; verify_case(0, Arg1, clean(Arg0)); }
	void test_case_1() { string Arg0 = "check www.foo.com 4 www.foo.com www.scoopz.com"; string Arg1 = "check OMIT1 4 OMIT2 OMIT3"; verify_case(1, Arg1, clean(Arg0)); }
	void test_case_2() { string Arg0 = "check www.foo.com 4 www.foo.comwww.scoopz.com"; string Arg1 = "check OMIT1 4 OMIT2"; verify_case(2, Arg1, clean(Arg0)); }
	void test_case_3() { string Arg0 = "check www.foo.com 4 www.foo.comhttp://scoopz.com"; string Arg1 = "check OMIT1 4 OMIT2OMIT3"; verify_case(3, Arg1, clean(Arg0)); }
	void test_case_4() { string Arg0 = "http://411.com goodz 4 www.733t.com, 2http://..com"; string Arg1 = "OMIT1 goodz 4 OMIT2, 2OMIT3"; verify_case(4, Arg1, clean(Arg0)); }
	void test_case_5() { string Arg0 = "http.//say.org,www.jeeves.x.info,www.comhttp://.tv"; string Arg1 = "http.//say.org,OMIT1,www.comhttp://.tv"; verify_case(5, Arg1, clean(Arg0)); }
	void test_case_6() { string Arg0 = "http://www.www.com/www"; string Arg1 = "OMIT1/www"; verify_case(6, Arg1, clean(Arg0)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	UnLinker ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
