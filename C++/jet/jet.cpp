#include "stdafx.h"
#include "jet.h"

#include <math.h>
/*
function J = jet(m)
%JET    Variant of HSV.
%   JET(M), a variant of HSV(M), is an M-by-3 matrix containing
%   the default colormap used by CONTOUR, SURF and PCOLOR.
%   The colors begin with dark blue, range through shades of
%   blue, cyan, green, yellow and red, and end with dark red.
%   JET, with no arguments, is the same length as the current colormap.
%   Use COLORMAP(JET).
%
%   See also HSV, HOT, PINK, FLAG, COLORMAP, RGBPLOT.

%   Copyright 1984-2002 The MathWorks, Inc. 
%   $Revision: 5.7 $  $Date: 2002/04/01 21:01:50 $

if nargin < 1
   m = size(get(gcf,'colormap'),1);
end
n = ceil(m/4);
u = [(1:1:n)/n ones(1,n-1) (n:-1:1)/n]';
g = ceil(n/2) - (mod(m,4)==1) + (1:length(u))';
r = g + n;
b = g - n;
g(g>m) = [];
r(r>m) = [];
b(b<1) = [];
J = zeros(m,3);
J(r,1) = u(1:length(r));
J(g,2) = u(1:length(g));
J(b,3) = u(end-length(b)+1:end);
*/
double** jet(int m)
{
	int i=0;
	//n = ceil(m/4);
	int n = ceil(m/4.0);
	//u = [(1:1:n)/n ones(1,n-1) (n:-1:1)/n]';
	int u_len = n*3-1;
	double* u = new double[u_len];
	for(i=0;i<n;i++){
		u[u_len-i-1] =  u[i] = double(i+1)/n;
		u[n+i] = 1;
	}
	//J = zeros(m,3);
	double** J = new double*[m];
	for(i=0;i<m;i++){
		J[i] = new double[3];
		for(int j=0;j<3;j++){
			J[i][j] = 0;
		}
	}
	//g = ceil(n/2) - (mod(m,4)==1) + (1:length(u))';
	//g(g>m) = [];
	//J(r,1) = u(1:length(r));
	int t = ceil(n/2.0) - (m % 4 == 1);
	int g_len = m-t;
	for(i=0;i<g_len;i++){
		J[t+i][1] = u[i];
	}
	//r = g + n;
	//r(r>m) = [];
	//J(g,2) = u(1:length(g));
	int r_len = m-t-n;
	for(i=0;i<r_len;i++){
		J[t+i+n][0] = u[i];
	}
	//b = g - n;
	//b(b<1) = [];	
	//J(b,3) = u(end-length(b)+1:end);	
	int b_start = n-t;
	for(i=b_start;i<u_len;i++){
		J[t+i-n][2] = u[i];
	}
	delete[] u;
	return J;
}