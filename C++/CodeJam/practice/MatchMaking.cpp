// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
// 
// 
// You are developing the matchmaking component of an online 
// dating site. Prospective
// members must fill out a questionnaire consisting of
// binary questions such as Do you prefer to vacation (a)
// in the mountains or (b) at the seaside? and Would you 
// rather 
// travel (a) by plane or (b) by train?
// 
// 
// 
// 
// You are to match up men with women by maximizing the 
// number of answers
// each couple has in common. A man and a woman have an 
// answer in common
// whenever they give the same answer to the same question. 
// Conflicts
// can easily arise due to numerical ties,
// but you will be able to resolve all such conflicts using 
// the following procedure. Note that there will be equal 
// numbers of men and
// women, with names being unique in each sex.
// 
// 
// 
// 
// Take the woman whose name comes earliest
// in lexicographic order, and consider the men with whom she 
// has the greatest number of
// answers in common. Among these men, pick the one whose 
// name comes earliest in lexicographic
// order. You have found the woman's best match. Remove this 
// couple from
// the dating pool, and repeat the matching procedure until 
// there are no
// more singles left. 
// 
// 
// 
// 
// You are given a vector <string>, namesWomen, containing
// the names of single women, and another vector <string>,
// answersWomen, containing their answers. The kth
// element of answersWomen lists the answers of the woman 
// whose name
// is the kth element of namesWomen. If there are n
// questions in the questionnaire, then every element of 
// answersWomen
// consists of n characters, each of which is either 'a' or 
// 'b'. The answers are always given in the fixed 
// questionnaire order. You
// are similarly given the vector <string>s namesMen and
// answersMen for the single men. Lastly, you are given
// a string, queryWoman, containing the name of a woman.
// Return the name of the man to whom she is matched after 
// you have formed
// all couples according to the above rules.
// 
// 
// 
// 
// DEFINITION
// Class:MatchMaking
// Method:makeMatch
// Parameters:vector <string>, vector <string>, vector 
// <string>, vector <string>, string
// Returns:string
// Method signature:string makeMatch(vector <string> 
// namesWomen, vector <string> answersWomen, vector <string> 
// namesMen, vector <string> answersMen, string queryWoman)
// 
// 
// NOTES
// -Lexicographic order is like dictionary order, with the 
// difference that case matters. All uppercase letters take 
// precedence over all lowercase letters. Thus, "boolean" 
// comes before "boot"; "boo" comes before "boolean"; "Boot" 
// comes before "boo"; "Zoo" comes before "boo".
// 
// 
// CONSTRAINTS
// -namesWomen contains between 1 and 50 elements, inclusive
// -if namesWomen consists of n elements, then answersWomen, 
// namesMen, and answersMen consist of n elements each
// -each element of namesWomen and each element of namesMen 
// is between 1 and 50 characters long, inclusive
// -the only characters that may appear in namesMen and 
// namesWomen are 'a' to 'z' and 'A' to 'Z'
// -no two elements of namesWomen are alike
// -no two elements of namesMen are alike
// -the first element of answersWomen is between 1 and 50 
// characters long, inclusive
// -if the first element of answersWomen consists of m 
// characters, then each element of answersWomen and of 
// answersMen is m characters long
// -the only characters that may appear in answersWomen and 
// answersMen are 'a' and 'b'
// -queryWoman is one of the strings in namesWomen
// 
// 
// EXAMPLES
// 
// 0)
// {"Constance", "Bertha", "Alice"}
// {"aaba", "baab", "aaaa"}
// {"Chip", "Biff", "Abe"}
// {"bbaa", "baaa", "aaab"}
// "Bertha"
// 
// Returns: "Biff"
// 
// Alice has two answers in common with Chip and three 
// answers in common with both Abe and Biff; Abe gets 
// lexicographic preference. Bertha also has two answers in 
// common with Chip and three answers in common with both Abe 
// and Biff. Since Abe has already been matched to Alice, 
// Bertha lands Biff.
// 
// 1)
// {"Constance", "Bertha", "Alice"}
// {"aaba", "baab", "aaaa"}
// {"Chip", "Biff", "Abe"}
// {"bbaa", "baaa", "aaab"}
// "Constance"
// 
// Returns: "Chip"
// 
// We are dealing with the same names and answers as before. 
// Constance is the last to go. Although she has two answers 
// in common with Abe and Biff, they are both taken. She ends 
// up with Chip, with whom she has only one answer in common.
// 
// 2)
// {"Constance", "Alice", "Bertha", "Delilah", "Emily"}
// {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}
// {"Ed", "Duff", "Chip", "Abe", "Biff"}
// {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}
// "Constance"
// 
// Returns: "Duff"
// 
// 3)
// {"Constance", "Alice", "Bertha", "Delilah", "Emily"}
// {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}
// {"Ed", "Duff", "Chip", "Abe", "Biff"}
// {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}
// "Delilah"
// 
// Returns: "Chip"
// 
// 4)
// {"Constance", "Alice", "Bertha", "Delilah", "Emily"}
// {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}
// {"Ed", "Duff", "Chip", "Abe", "Biff"}
// {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}
// "Emily"
// 
// Returns: "Ed"
// 
// 5)
// {"anne", "Zoe"}
// {"a", "a"}
// {"bob", "chuck"}
// {"a", "a"}
// "Zoe"
// 
// Returns: "bob"
// 
// 6)
// {"F", "M", "S", "h", "q", "g", "r", "N", "U", "x", "H", "P",
//  "o", "E", "R", "z", "L", "m", "e", "u", "K", "A", "w", "Q",
//  "O", "v", "j", "a", "t", "p", "C", "G", "k", "c", "V", "B",
//  "D", "s", "n", "i", "f", "T", "I", "l", "d", "J", "y", "b"}
// {"abaabbbb", "bbaabbbb", "aaabaaab", "aabbaaaa", "baabbaab",
//  "aaababba", "bbabbbbb", "bbbabbba", "aaabbbba", "aabbbaaa",
//  "abbabaaa", "babbabbb", "aaaaabba", "aaaabbaa", "abbbabaa",
//  "babababa", "abbaaaaa", "bbababba", "baaaaaba", "baaaaabb",
//  "bbbbabba", "ababbaaa", "abbbabab", "baabbbaa", "bbbaabbb",
//  "aababbab", "ababbabb", "abbaabba", "baabbabb", "aaabaaab",
//  "aabbbaba", "aabaaabb", "abababba", "aabbaaaa", "aabbabaa",
//  "bababaaa", "aabaaaab", "bbbbaabb", "baaababb", "abaabbab",
//  "aabbbaaa", "baabbaba", "bbabbbaa", "aabbbbaa", "abbbaaab",
//  "abababbb", "ababaaba", "bababaaa"}
// {"f", "C", "v", "g", "Q", "z", "n", "c", "B", "o", "M", "F",
//  "u", "x", "I", "T", "K", "L", "E", "U", "w", "A", "d", "t",
//  "e", "R", "D", "s", "p", "q", "m", "r", "H", "j", "J", "V",
//  "l", "a", "k", "h", "G", "y", "i", "P", "O", "N", "b", "S"}
// {"bbbaabab", "bbabaabb", "ababbbbb", "bbbababb", "baababaa",
//  "bbaaabab", "abbabbaa", "bbbabbbb", "aabbabab", "abbababa",
//  "aababbbb", "bababaab", "aaababbb", "baabbaba", "abaaaaab",
//  "bbaababa", "babaabab", "abbabbba", "ababbbab", "baabbbab",
//  "babbaaab", "abbbbaba", "bbabbbba", "baaabaab", "ababbabb",
//  "abbbaabb", "bbbbaabb", "bbbaaabb", "baabbaba", "bbabaaab",
//  "aabbbaab", "abbbbabb", "bbaaaaba", "bbbababa", "abbaabba",
//  "bababbbb", "aabaaabb", "babbabab", "baaaabaa", "ababbaba",
//  "aaabaabb", "bbaaabaa", "baaaaabb", "bbaabaab", "bbababab",
//  "aabaaaab", "aaaaabab", "aabbaaba"}
// "U"
// 
// Returns: "x"
// 
// 7)
// {"q", "M", "w", "y", "p", "N", "s", "r", "a", "H", "o", "n",
//  "F", "m", "l", "b", "D", "j", "C", "u", "f", "I", "g", "L",
//  "i", "x", "A", "G", "O", "k", "h", "d", "c", "E", "B", "v",
//  "J", "z", "K", "e", "t"}
// {"aabbaaabb", "baabababb", "bbaababba", "bbbaaaaaa", 
// "abaaaabaa",
//  "bababbbab", "abbaabbaa", "aabababbb", "bababaaaa", 
// "abbababaa",
//  "aabbbbbba", "bbabbabab", "babaabbba", "babbabbbb", 
// "baaabbbbb",
//  "baaabaaaa", "aaabbaaab", "abbaabbbb", "abbabbbab", 
// "bbaaaabba",
//  "babbaaabb", "aabbabbab", "baaababba", "ababaabab", 
// "bbbaabbab",
//  "aaaabbabb", "babaaaaaa", "abbbbaaab", "aabaaabba", 
// "bbbaaaaba",
//  "bbbbbbaab", "aabbaaabb", "aabaabbab", "aababaaba", 
// "bbabbbbab",
//  "abbabaaab", "babaaabbb", "bababbaaa", "aabbaabaa", 
// "baaabbabb",
//  "bbbbbbbbb"}
// {"m", "k", "n", "q", "L", "E", "M", "l", "w", "x", "g", "e",
//  "i", "z", "F", "r", "a", "h", "f", "D", "J", "K", "j", "v",
//  "A", "t", "N", "y", "s", "c", "o", "p", "d", "b", "B", "G",
//  "O", "I", "u", "C", "H"}
// {"bbaaabbba", "bbaaaaaab", "abaaababb", "baaaabbbb", 
// "abbbababa",
//  "baaaaaaaa", "aabbbbbab", "aaaaabbba", "baabababb", 
// "babaaabab",
//  "baaababaa", "bbbbaabba", "bbaabbabb", "bbaaababb", 
// "abbabbaba",
//  "aababaaab", "abbbbbbaa", "aabbaabaa", "bbbaabbba", 
// "abbabbaba",
//  "aaabbbaaa", "bbaabaaaa", "aabababbb", "abbbbabab", 
// "baaabbbba",
//  "bababbbba", "aababbaab", "bbaabbaab", "bbbaaabbb", 
// "babbbbabb",
//  "ababababb", "babaaabab", "bbaaaaaba", "aaaaabaaa", 
// "abbaaabbb",
//  "bbbbababb", "baabababb", "bbaabaaaa", "aaababbbb", 
// "abbbbbbba",
//  "bbaabbaaa"}
// "o"
// 
// Returns: "C"
// 
// END CUT HERE
#line 256 "MatchMaking.cpp"

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

class MatchMaking
{ 
public: 
	string makeMatch(vector <string> namesWomen, vector <string> answersWomen, 
		vector <string> namesMen, vector <string> answersMen, string queryWoman) 
	{ 
		map<string,string> w;
		REP(i,SZ(namesWomen)) w[namesWomen[i]] = answersWomen[i];
		for(map<string,string>::iterator it=w.begin();it!=w.end();it++){
			map<int,set<int> > avl;
			REP(i,SZ(namesMen)) {
				int n=0;
				REP(k,SZ(answersMen[i]))if(answersMen[i][k]==it->second[k])n++;
				avl[n].insert(i);
			}
			map<string,int> pair;
			set<int> bs = avl.rbegin()->second;
			for(set<int>::iterator i=bs.begin();i!=bs.end();i++)
				pair[namesMen[*i]] = *i;
			it->second = pair.begin()->first;
			int best = pair.begin()->second;
			namesMen.erase( namesMen.begin()+best);
			answersMen.erase(answersMen.begin()+best);
		}
		return w[queryWoman];
	} 
	
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); if ((Case == -1) || (Case == 7)) test_case_7(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {"Constance", "Bertha", "Alice"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"aaba", "baab", "aaaa"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"Chip", "Biff", "Abe"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"bbaa", "baaa", "aaab"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Bertha"; string Arg5 = "Biff"; verify_case(0, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_1() { string Arr0[] = {"Constance", "Bertha", "Alice"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"aaba", "baab", "aaaa"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"Chip", "Biff", "Abe"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"bbaa", "baaa", "aaab"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Constance"; string Arg5 = "Chip"; verify_case(1, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_2() { string Arr0[] = {"Constance", "Alice", "Bertha", "Delilah", "Emily"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"Ed", "Duff", "Chip", "Abe", "Biff"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Constance"; string Arg5 = "Duff"; verify_case(2, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_3() { string Arr0[] = {"Constance", "Alice", "Bertha", "Delilah", "Emily"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"Ed", "Duff", "Chip", "Abe", "Biff"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Delilah"; string Arg5 = "Chip"; verify_case(3, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_4() { string Arr0[] = {"Constance", "Alice", "Bertha", "Delilah", "Emily"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"baabaa", "ababab", "aaabbb", "bababa", "baabba"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"Ed", "Duff", "Chip", "Abe", "Biff"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"aabaab", "babbab", "bbbaaa", "abbbba", "abaaba"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Emily"; string Arg5 = "Ed"; verify_case(4, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_5() { string Arr0[] = {"anne", "Zoe"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"a", "a"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"bob", "chuck"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"a", "a"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "Zoe"; string Arg5 = "bob"; verify_case(5, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_6() { string Arr0[] = {"F", "M", "S", "h", "q", "g", "r", "N", "U", "x", "H", "P",
 "o", "E", "R", "z", "L", "m", "e", "u", "K", "A", "w", "Q",
 "O", "v", "j", "a", "t", "p", "C", "G", "k", "c", "V", "B",
 "D", "s", "n", "i", "f", "T", "I", "l", "d", "J", "y", "b"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"abaabbbb", "bbaabbbb", "aaabaaab", "aabbaaaa", "baabbaab",
 "aaababba", "bbabbbbb", "bbbabbba", "aaabbbba", "aabbbaaa",
 "abbabaaa", "babbabbb", "aaaaabba", "aaaabbaa", "abbbabaa",
 "babababa", "abbaaaaa", "bbababba", "baaaaaba", "baaaaabb",
 "bbbbabba", "ababbaaa", "abbbabab", "baabbbaa", "bbbaabbb",
 "aababbab", "ababbabb", "abbaabba", "baabbabb", "aaabaaab",
 "aabbbaba", "aabaaabb", "abababba", "aabbaaaa", "aabbabaa",
 "bababaaa", "aabaaaab", "bbbbaabb", "baaababb", "abaabbab",
 "aabbbaaa", "baabbaba", "bbabbbaa", "aabbbbaa", "abbbaaab",
 "abababbb", "ababaaba", "bababaaa"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"f", "C", "v", "g", "Q", "z", "n", "c", "B", "o", "M", "F",
 "u", "x", "I", "T", "K", "L", "E", "U", "w", "A", "d", "t",
 "e", "R", "D", "s", "p", "q", "m", "r", "H", "j", "J", "V",
 "l", "a", "k", "h", "G", "y", "i", "P", "O", "N", "b", "S"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"bbbaabab", "bbabaabb", "ababbbbb", "bbbababb", "baababaa",
 "bbaaabab", "abbabbaa", "bbbabbbb", "aabbabab", "abbababa",
 "aababbbb", "bababaab", "aaababbb", "baabbaba", "abaaaaab",
 "bbaababa", "babaabab", "abbabbba", "ababbbab", "baabbbab",
 "babbaaab", "abbbbaba", "bbabbbba", "baaabaab", "ababbabb",
 "abbbaabb", "bbbbaabb", "bbbaaabb", "baabbaba", "bbabaaab",
 "aabbbaab", "abbbbabb", "bbaaaaba", "bbbababa", "abbaabba",
 "bababbbb", "aabaaabb", "babbabab", "baaaabaa", "ababbaba",
 "aaabaabb", "bbaaabaa", "baaaaabb", "bbaabaab", "bbababab",
 "aabaaaab", "aaaaabab", "aabbaaba"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "U"; string Arg5 = "x"; verify_case(6, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }
	void test_case_7() { string Arr0[] = {"q", "M", "w", "y", "p", "N", "s", "r", "a", "H", "o", "n",
 "F", "m", "l", "b", "D", "j", "C", "u", "f", "I", "g", "L",
 "i", "x", "A", "G", "O", "k", "h", "d", "c", "E", "B", "v",
 "J", "z", "K", "e", "t"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); string Arr1[] = {"aabbaaabb", "baabababb", "bbaababba", "bbbaaaaaa", "abaaaabaa",
 "bababbbab", "abbaabbaa", "aabababbb", "bababaaaa", "abbababaa",
 "aabbbbbba", "bbabbabab", "babaabbba", "babbabbbb", "baaabbbbb",
 "baaabaaaa", "aaabbaaab", "abbaabbbb", "abbabbbab", "bbaaaabba",
 "babbaaabb", "aabbabbab", "baaababba", "ababaabab", "bbbaabbab",
 "aaaabbabb", "babaaaaaa", "abbbbaaab", "aabaaabba", "bbbaaaaba",
 "bbbbbbaab", "aabbaaabb", "aabaabbab", "aababaaba", "bbabbbbab",
 "abbabaaab", "babaaabbb", "bababbaaa", "aabbaabaa", "baaabbabb",
 "bbbbbbbbb"}; vector <string> Arg1(Arr1, Arr1 + (sizeof(Arr1) / sizeof(Arr1[0]))); string Arr2[] = {"m", "k", "n", "q", "L", "E", "M", "l", "w", "x", "g", "e",
 "i", "z", "F", "r", "a", "h", "f", "D", "J", "K", "j", "v",
 "A", "t", "N", "y", "s", "c", "o", "p", "d", "b", "B", "G",
 "O", "I", "u", "C", "H"}; vector <string> Arg2(Arr2, Arr2 + (sizeof(Arr2) / sizeof(Arr2[0]))); string Arr3[] = {"bbaaabbba", "bbaaaaaab", "abaaababb", "baaaabbbb", "abbbababa",
 "baaaaaaaa", "aabbbbbab", "aaaaabbba", "baabababb", "babaaabab",
 "baaababaa", "bbbbaabba", "bbaabbabb", "bbaaababb", "abbabbaba",
 "aababaaab", "abbbbbbaa", "aabbaabaa", "bbbaabbba", "abbabbaba",
 "aaabbbaaa", "bbaabaaaa", "aabababbb", "abbbbabab", "baaabbbba",
 "bababbbba", "aababbaab", "bbaabbaab", "bbbaaabbb", "babbbbabb",
 "ababababb", "babaaabab", "bbaaaaaba", "aaaaabaaa", "abbaaabbb",
 "bbbbababb", "baabababb", "bbaabaaaa", "aaababbbb", "abbbbbbba",
 "bbaabbaaa"}; vector <string> Arg3(Arr3, Arr3 + (sizeof(Arr3) / sizeof(Arr3[0]))); string Arg4 = "o"; string Arg5 = "C"; verify_case(7, Arg5, makeMatch(Arg0, Arg1, Arg2, Arg3, Arg4)); }

// END CUT HERE
 
}; 

// BEGIN CUT HERE 
int main()
{
	MatchMaking ___test; 
	___test.run_test(-1); 
} 
// END CUT HERE 
