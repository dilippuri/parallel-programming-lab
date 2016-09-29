a=[2 -1 -2; -4 6 3; -4 -2 8];
[n,m]=size(a);

for k=1:n
	for j=k:n
		a(j,k)=a(j,k)/a(k,k);
	endfor
	for j=(k+1):n
		for i=(k+1):n
			a(i,j)=a(i,j)-a(i,k)*a(k,j);
		endfor
	endfor
endfor
a
