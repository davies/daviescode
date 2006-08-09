   #include <STDIO.H>
#include <STRING.H>
#include <STDLIB.H>
#include "math.h"

const int N = 47;
typedef  unsigned __int64 nType;
/*
 *��W(n,m)
 *��n��������ȡ��m��(�����ظ�ȡ)�������
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
 *���ڵݹ�����лᷴ���õ�ǰ��Ľ���������Ǳ�������������������߼����ٶ�
 *���㸴�ӶȻή�͵������أ� O(N*N) ?
 */
nType TreeCount[N][N][N];

/*
 *���nodes���ڵ����roots����ʱ���п����������Ŀ
 *����ÿ�����Ľڵ�����������minnodes
 *ÿ���ڵ㶼�ǵȼ۵ģ����ķ�֧û��˳��������ͬ������Ϊ�ȼ�
 */
nType treesCount(const int nodes,const int roots,const int minnodes)
{
	if ( nodes <= 0 || roots <= 0 || nodes < roots*minnodes) {
		return 0;
	}
	if ( TreeCount[nodes-1][roots-1][minnodes-1] > 0 ) { //���û���
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

	TreeCount[nodes-1][roots-1][minnodes-1] = sumTrees; //��������������
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