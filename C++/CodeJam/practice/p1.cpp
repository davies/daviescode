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

#define FOR(i,a,b) for(int i=(a);i<int(b);++i)
#define REP(i,n) FOR(i,0,n)
template<class U,class T> T cast(U x){T y; ostringstream a; a<<x; istringstream b(a.str());b>>y;return y;}
#define ALL(v) (v).begin(),(v).end()
#define SZ(v) ((int)(v).size())
#define FOUND(v,p) (find(ALL(v),p)!=v.end())
#define SUM(v) accumulate(ALL(v),0)

class SecretSum{
	vector<list<char> > stand;
	map<char,list<int> > mp;
	int solve(string a,string b,string s, int p){
		int res = 0;
		REP(i,10) if( stand[i].empty()&&mp[a[p]].empty() || !stand[i].empty()&&stand[i].back()==a[p]) {
			if( i==0 && p== 0 ) continue;
			stand[i].push_back(a[p]);mp[a[p]].push_back(i);
			a[p] = '0'+i;
			REP(j,10)if( stand[j].empty()&&mp[b[p]].empty() || !stand[j].empty()&&stand[j].back()==b[p]) {
				if( j==0 && p== 0 ) continue;
				stand[j].push_back(b[p]);mp[b[p]].push_back(j);
				b[p] = '0'+j;
				REP(k,10) if( stand[k].empty()&&mp[s[p]].empty() || !stand[k].empty()&&stand[k].back()==s[p]){
					if( j==0 && p== 0 ) continue;
					stand[k].push_back(s[p]);mp[s[p]].push_back(k);
					s[p] = '0'+k;
					int ab = cast<string,int>(a)+cast<string,int>(b);
					int ss = cast<string,int>(s);
					if( p == 2 ){
						res += (ab == ss );
					}else{
						if( ab == ss || ab == ss-1 ){
							res += solve(a,b,s,p+1);
						}
					}
					s[p]=stand[k].back();
					stand[k].pop_back();mp[s[p]].pop_back();
				}
				b[p]=stand[j].back();
				stand[j].pop_back();mp[b[p]].pop_back();
			}
			a[p]=stand[i].back();
			stand[i].pop_back();mp[a[p]].pop_back();
		}
		return res;
	}
public:
	int countPossible(string a, string b,string s)
	{
		stand = vector<list<char> >(10);
		mp.clear();
		return solve(a,b,s,0);
	}
};

int main()
{
	SecretSum ___test; 
	___test.run_test(-1); 
} 
