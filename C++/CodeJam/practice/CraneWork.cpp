
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

#define SZ(i) ((i).size())
#define FOR(i,a,b) for(int i=(a);i<(b);i++)
#define REP(i,n) FOR(i,0,n)
#define DB double
#define VI vector<int>
#define VS vector<string>

struct Crane{
	int v;
	int s;
	Crane(int _s,int _v){v=_v;s=_s;}
	bool operator < (const Crane& b) const{
		if(v==b.v) return s<b.s;
		return v < b.v;
	}
};

int T[21]={0};
vector<Crane> vc;

class CraneWork
{
public:
    int moves(vector <int> stack1, vector <int> stack2, vector <int> warehouse)
    {
		vc.clear();
		REP(i,SZ(stack1))vc.push_back(Crane(0,stack1[i]));
		REP(i,SZ(stack2))vc.push_back(Crane(1,stack2[i]));
		REP(i,SZ(warehouse))vc.push_back(Crane(2,warehouse[i]));
		sort(vc.begin(),vc.end());
		REP(i,SZ(vc)) if(i>0 && vc[i].v != vc[i-1].v) T[i+1] = T[i]*2+1; else T[i+1] = T[i]+1;
		return steps(SZ(vc)-1,2);
    }
	int steps(int i, int s)
	{
		if (i<0) return 0;
		int c[3] = {0};
		int k = i;
		while(k >=0 && vc[k].v == vc[i].v) c[vc[k--].s]++;
		if( c[(s+1)%3] && c[(s+2)%3] ) 
			return min(steps(k,s)+min(c[(s+2)%3],c[(s+1)%3]), min(steps(k,(s+2)%3),steps(k,(s+1)%3)))
					+T[k+1]*2+c[(s+2)%3]+c[(s+1)%3];
		else if( c[(s+1)%3] ){
			return steps(k,(s+2)%3)+c[(s+1)%3]+T[k+1];
		}else if( c[(s+2)%3] ){
			return steps(k,(s+1)%3)+c[(s+2)%3]+T[k+1];
		}else{
			return steps(k,s);
		}
	}
//CUT_BEGIN
public:
void run_test(int Case) {if ((Case == -1) || (Case == 0)) test_case_0();if ((Case == -1) || (Case == 1)) test_case_1();if ((Case == -1) || (Case == 2)) test_case_2();if ((Case == -1) || (Case == 3)) test_case_3();}
private:
    template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
    template <typename T> void verify_case(int Case, const T &Expected, const T &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
void test_case_0(){int Arr0[] = {3,50};vector <int> Arg0(Arr0, Arr0 + sizeof(Arr0)/sizeof(Arr0[0]));vector <int> Arg1;int Arr2[] = {1,2,50,50,50};vector <int> Arg2(Arr2, Arr2 + sizeof(Arr2)/sizeof(Arr2[0]));int Arg3 =  12;verify_case<int>(0, Arg3, moves(Arg0, Arg1, Arg2));}
void test_case_1(){int Arr0[] = {50};vector <int> Arg0(Arr0, Arr0 + sizeof(Arr0)/sizeof(Arr0[0]));int Arr1[] = {50};vector <int> Arg1(Arr1, Arr1 + sizeof(Arr1)/sizeof(Arr1[0]));int Arr2[] = {10,20,30};vector <int> Arg2(Arr2, Arr2 + sizeof(Arr2)/sizeof(Arr2[0]));int Arg3 =  17;verify_case<int>(1, Arg3, moves(Arg0, Arg1, Arg2));}
void test_case_2(){vector <int> Arg0;vector <int> Arg1;int Arr2[] = {2,5,6,7};vector <int> Arg2(Arr2, Arr2 + sizeof(Arr2)/sizeof(Arr2[0]));int Arg3 =  0;verify_case<int>(2, Arg3, moves(Arg0, Arg1, Arg2));}
void test_case_3(){int Arr0[] = {1,2,3};vector <int> Arg0(Arr0, Arr0 + sizeof(Arr0)/sizeof(Arr0[0]));vector <int> Arg1;vector <int> Arg2;int Arg3 =  7;verify_case<int>(3, Arg3, moves(Arg0, Arg1, Arg2));}


//CUT_END
};

//CUT_BEGIN
int main(){
    CraneWork __tester;
    __tester.run_test(-1);
    return 0;
}
//CUT_END
    