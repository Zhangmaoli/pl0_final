const N = 10;

var i,j,k,a[N][N],b[N][N],c[N][N];
 

procedure initMatrix(p[N][N],q[N][N],r[N][N]);
begin
   for(i=0;i<N;i+=1)
   		for(j=0;j<N;j+=1)
   		  begin
   		     p[i][j] = 1; 
   		     q[i][j] = 1;   
   		     r[i][j] = 0;
   		  end
   
end;

procedure printMatrix(p[N][N]);
var m,n;

begin
	for(m=0;m<N;m=m+1)
	  begin
  		for(n=0;n<N;n=n+1) print(p[m][n]);
  		print();
  	end
  print();
end;  

procedure matrixMul(p[N][N],q[N][N],r[N][N]);
var i,j,k;
begin
	
  for(i=0;i<N;i++)
   	for(j=0;j<N;++j)
   	    for(k=0;k<N;k+=1)
   			   r[i][j] += p[i][k] * q[k][j];
end;

begin

	initMatrix(a,b,c);
  
  printMatrix(a);
  printMatrix(b);
  printMatrix(c);
  
  matrixMul(a,b,c);	 
  
  printMatrix(c);
   
end.
