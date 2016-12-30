var i;
begin
	var j;
	for i:=1 to  3 do begin
	end;
	j:=2;
	writeln(j);
end.



const n=8;
var a[8][8],i,j,cnt;
procedure printarr;
var i,j;
begin
 cnt:=cnt+1;
 writeln(cnt);
 for i:=0 to n-1 do
  begin
   for j:=0 to n-2 do
    write(a[i][j]);
   writeln(a[i][n-1]);
  end;
end;
procedure search(i);
var j,flag,k,d;
begin
 if i=n then
  begin
   call printarr;
   exit;
  end;
 for j:=0 to n-1 do
  begin
   a[i][j]:=1; 
   flag:=1;
   for k:=0 to i-1 do
    if a[k][j]=1 then
     flag:=0;
   
   for k:=i-1 downto 0 do
    begin
     d:=i-k;
     if j-d>=0 then
      if a[i-d][j-d]=1 then
       flag:=0;
     if j+d<n then
      if a[i-d][j+d]=1 then
       flag:=0;
    end;
   

   if flag then
    begin
     call search(i+1);
    end;
   a[i][j]:=0;
  end;
end;
begin
 for i:=0 to n-1 do
  for j:=0 to n-1 do
   a[i][j]:=0; 
 cnt:=0;
 call search(0);
end. 




const n=8;
var a[20][20],i,j,cnt;
procedure printarr;
var i,j;
begin
 cnt:=cnt+1;
 if (cnt>1) then
  exit;
 writeln(cnt);
 for i:=0 to n-1 do
  begin
   for j:=0 to n-2 do
    write(a[i][j]);
   writeln(a[i][n-1]);
  end;
end;
procedure search(i);
var j,flag,k,d;
begin
 j:=2;
 k:=23;
 a[8][8]:=j*k;
 if i=n then
  begin
   call printarr;
   exit;
  end;
 for j:=0 to n-1 do
  begin
   a[i][j]:=1; 
   flag:=1;
   for k:=0 to i-1 do
    if a[k][j]=1 then
     flag:=0;
   
   for k:=i-1 downto 0 do
    begin
     d:=i-k;
     if j-d>=0 then
      if a[i-d][j-d]=1 then
       flag:=0;
     if j+d<n then
      if a[i-d][j+d]=1 then
       flag:=0;
    end;
   

   if flag then
    begin
     call search(i+1);
    end;
   a[i][j]:=0;
  end;
end;
begin
 for i:=0 to n-1 do
  for j:=0 to n-1 do
   a[i][j]:=0; 
 cnt:=0;
 call search(0);
end. 


onst n=8;
var a[8][8],i,j,cnt;
procedure printarr;
var i,j;
begin
 cnt:=cnt+1;
 writeln(cnt);
 for i:=0 to n-1 do
  begin
   for j:=0 to n-2 do
    write(a[i][j]);
   writeln(a[i][n-1]);
  end;
end;
procedure search(i);
var j,flag,k,d,t1,t2,t3;
begin
 if i=n then
  begin
   //call printarr;
   exit;
  end;
 for j:=0 to n-1 do
  begin
   flag:=1;
   for k:=0 to i-1 do
    if a[k][j]=1 then
     flag:=0;
   
   for k:=i-1 downto 0 do
    begin
     d:=i-k;
	 t1:=j-d;
	 t2:=j+d;
	 t3:=i-d;
     if j-d>=0 then
      if a[i-d][j-d]=1 then
       flag:=0;
     if j+d<n then
	  begin
      if a[i-d][j+d]=1 then begin
	   
       flag:=0;
	   end;
	  end;
	   
    end;
   

   if flag then
    begin
     call search(i+1);
    end;
   a[i][j]:=0;
  end;
end;
begin
 for i:=0 to n-1 do
  for j:=0 to n-1 do
   a[i][j]:=0; 
 cnt:=0;
 writeln(0);
 call search(0);
end. 


var 
	i,j,k;
begin
	if i-j and i+j then 
		k:=1;
	if i*j and i-j and i*j+3 then 
		k:=0;
		
end.

var
	a[10],i,j,k;
	
procedure p;
	var i;
	begin
		for i:=1 to 10 do
			begin
				if i<5 then
					write(a[i]);
			end;
	end;
	
begin
	i:=2;
	j:=2;
	k:=i*j;
	writeln(k);
	for j:=1 to 3 do
		a[0]:=1;
	for j:=1 to 3 do
		a[0]:=1;
	k:=i*j;
	writeln(k);
end.



const n=8;
var a[8][8],i,j,cnt;
procedure printarr;
var i,j;
begin
 cnt:=cnt+1;
 writeln(cnt);
 for i:=0 to n-1 do
  begin
   for j:=0 to n-2 do
    write(a[i][j]);
   writeln(a[i][n-1]);
  end;
end;
procedure search(i);
var j,flag,k,d,t1,t2,t3;
begin
 if i=n then
  begin
   //call printarr;
   exit;
  end;
 for j:=0 to n-1 do
  begin
   flag:=1;
   for k:=0 to i-1 do
    if a[k][j]=1 then
     flag:=0;
   
   for k:=i-1 downto 0 do
    begin
     d:=i-k;
	 t1:=j-d;
	 t2:=j+d;
	 t3:=i-d;
     if j-d>=0 then
      if a[i-d][j-d]=1 then
       flag:=0;
     if j+d<n then
	  begin
      if a[i-d][j+d]=1 then begin
	   
       flag:=0;
	   end;
	  end;
	   
    end;
   

   if flag then
    begin
     call search(i+1);
    end;
   a[i][j]:=0;
  end;
end;
begin
 for i:=0 to n-1 do
  for j:=0 to n-1 do
   a[i][j]:=0; 
 cnt:=0;
 writeln(0);
 call search(0);
end. 





var i,j,k,a,b,c;
begin
	j:=1;
	for i:=1 to 3 do
		begin
			for a:=1 to 2 do
				begin
					k:=i*j;
					writeln(i,j,k);
				end;
			for a:=1 to 2 do
				begin
					k:=i*j;
					writeln(i,j,k);
				end;
			end;

end.


var i,j,k;
begin
	i:=2; j:=3;
	k:=i*j;
	k:=i*j;
	writeln(i,j,k);
	for i:=1 to 3 do
		begin
			k:=i*j;
			k:=i*j;
			writeln(i,j,k);
		end;
end.


var i,j,k;
begin
	for i:=1 to 10 do
	 for i:=1 to 2 do
	  write(i);
end.
var i,j,k;
begin
	i:=2;j:=1;
	k:=i+j;
	for i:=1 to 3 do
		j:=1;
	k:=i+j;
	writeln(k);
end.


var
	i,j,k;
procedure p;
begin
	i:=2;
end;
begin
	i:=1;
	k:=i*i;
	writeln(k);
	k:=i*i;
	writeln(k);
	call p;
	k:=i*i;
	writeln(k);
end.


const n=7;
var a[20][20],i,j,cnt;
procedure printarr;
var i,j;
begin
 cnt:=cnt+1;
 if (cnt>1) then
  exit;
 writeln(cnt);
 for i:=0 to n-1 do
  begin
   for j:=0 to n-2 do
    write(a[i][j]);
   writeln(a[i][n-1]);
  end;
end;
procedure search(i);
var j,flag,k,d;
begin
 j:=2;
 k:=23;
 a[8][8]:=j*k;
 if i=n then
  begin
   call printarr;
   exit;
  end;
 for j:=0 to n-1 do
  begin
   a[i][j]:=1; 
   flag:=1;
   for k:=0 to i-1 do
    if a[k][j]=1 then
     flag:=0;
   
   for k:=i-1 downto 0 do
    begin
     d:=i-k;
     if j-d>=0 then
      if a[i-d][j-d]=1 then
       flag:=0;
     if j+d<n then
      if a[i-d][j+d]=1 then
       flag:=0;
    end;
   

   if flag then
    begin
     call search(i+1);
    end;
   a[i][j]:=0;
  end;
end;
begin
 for i:=0 to n-1 do
  for j:=0 to n-1 do
   a[i][j]:=0; 
 cnt:=0;
 call search(0);
end. 

var i,j,k,a,b,c;
begin
	i:=2;
	j:=3;
	a:=i*j;
	b:=i*j+1;
	writeln(a,b);
end.

    1 INT       0       9
    2 LIT       0       2
    3 STO       0       3
    4 LOD       0       3
    5 LIT       0       3
    6 OPR       0       4
    7 LOD       0       3
    8 LIT       0       3
    9 OPR       0       4
   10 OPR       0       2
   11 STO       0       6
   12 LOD       0       6
   13 OPR       0       14
   14 OPR       0       13
   15 OPR       0       0


begin
	i:=2;
	t:=i*3;
	a:=t*t;
end.

 
expression -> "i*3"
	saveCx=cx
	if  "i*3" does not exist in TABLE
		add "i*3" into TABLE as a variable
		TABLE[tx].varlist->i.cnt=TABLE[i].cnt
		TABLE[tx].cnt=0;
	else
		find i: TABLE[i].name="i*3"
		flag=1;
		if TABLE[i].varlist->var.cnt > TABLE[var.tx].cnt
			TABLE[i].varllist->var.cnt=TABLE[var.tx].cnt;
			flag=0;
		if flag==1
			//确定可以定义 ^_^
			cx=saveCx
			load TABLE[i]


			
		
	


var i,j;
 
begin
 
 i := 10;
 j := 20;
 if( i and j and i > j and not((j < 20) or i>10) ) then
  		writeln(100,j)
 else 
 		writeln(200,i);
 
end.

var i;
procedure move(a,b);
begin
  write(i,a);
  writeln(b);
  i := i + 1;
end;

procedure hanoi(n,a,b,c);
begin
	if (n=1) then
		call move(a,c)
	else 
		begin
			call hanoi(n-1,a,c,b);
			call move(a,c);
			call hanoi(n-1,b,a,c);
		end;
end;

begin
	i:=1;
	call hanoi(5,1,2,3);
end.
