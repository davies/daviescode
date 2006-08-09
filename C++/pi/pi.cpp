  #include <stdlib.h>
#include <stdio.h>

long a=1000,b,c=100000,d,e,f[100001],g;

main()
{
//	double pi = 1.0;
//	double mul = 1.0;
//	for(int i=1;i<50;i++){
//		mul *= i;
//		mul /= (i*2+1);
//		pi += mul;
//		printf("%3d->%10.9f\n",i,pi*2);
//	}
	for(b=0;b<c;b++)
		f[b]=a/5;
	for(;;){
		d = 0;
		g = c*2;
		if ( !g) {
			break;
		}
		for(b=c;d+=f[b]*a,f[b]=d%--g,d/=g--,--b;){
			d*=b;
		}
		c-=10;
//		printf("%.3d",e+d/a);
		e=d%a;
	}
}

