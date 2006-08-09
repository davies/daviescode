   #include <STDIO.H>
#include <STRING.H>
#include <STDLIB.H>
#include "math.h"

const int N = 47;
typedef  unsigned __int64 nType;
/*
 *求W(n,m)
 *从n个样本中取出m个(可以重复取)的组合数
 *W(n,m) = (n+m-1)! / (n-1)! / m! = C(n+m-1,m)
 */
nType factor(int n,int m)
{
	nType result = 1;
	for(int i=0;i<m;i++){
		result *= (n+i);
		result /= i+1;
	}
	return result;
}

/*
 *由于递归过程中会反复用到前面的结果，将它们保存起来可以显著的提高计算速度
 *计算复杂度会降低到多少呢？ O(N*N) ?
 */
nType TreeCount[N][N][N];

/*
 *求解nodes个节点组成roots棵树时所有可能情况的数目
 *其中每棵树的节点数不得少于minnodes
 *每个节点都是等价的，树的分支没有顺序，拓扑相同的树视为等价
 */
nType treesCount(const int nodes,const int roots,const int minnodes)
{
	if ( nodes <= 0 || roots <= 0 || nodes < roots*minnodes) {
		return 0;
	}
	if ( TreeCount[nodes-1][roots-1][minnodes-1] > 0 ) { //利用缓存
		return TreeCount[nodes-1][roots-1][minnodes-1];
	}

	nType sumTrees = 0;
	if ( roots == 1 ) {
		for(int i=1;i<nodes;i++){
			sumTrees += treesCount(nodes-1,i,1);
		}
	}else{
		for( int i=minnodes;i<=nodes/roots;i++){
			nType tmp = treesCount(i,1,1);
			int min = roots*(i+1)-nodes;
			min = min < 1 ? 1 : min;
			for(int j=min;j<=roots;j++){
				if (roots-j) {
					sumTrees += factor(tmp,j)*treesCount(nodes-i*j,roots-j,i+1);
				}else if (nodes == i*j){
					sumTrees += factor(tmp,j);
				}
			}
		}
	}

	TreeCount[nodes-1][roots-1][minnodes-1] = sumTrees; //保存结果到缓存中
	return sumTrees ;
}

void main(int argc , char** argv)
{
	int i;
	memset(TreeCount,0,sizeof(TreeCount));
	TreeCount[0][0][0] = 1;

	treesCount(N,1,1);

	char buffer[100] = "";
	for( i=0;i<N;i++){
		_ui64toa(TreeCount[i][0][0],buffer,10);
		printf("%s \n",buffer);
	}
//	for(i=1;i<10;i++){
//		for(int j=1;j<10;j++)
//			printf("%6d",factor(i,j));
//		printf("\n");
//	}
}