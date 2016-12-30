var queen[8];

procedure output();
var i;
begin
	i = 0;
	do
		begin
  		print(queen[i]);
  		i=i+1;
		end
	while  (i<7);
	print();
end;
 
procedure place(n);
var i,ok,result;
begin
	queen[n] = 0;
	while (queen[n]<8)
		begin
  		ok = 1;
  		i  = 0;
  		while ((i<n) && ok) 
  			begin
  				
  				result = queen[n] - queen[i];
  				if (result < 0 ) result = - result;
     			if (result == 0 || result == (n-i) ) ok = 0;
     			i = i + 1;
  			end
  		if (ok)  
    		if (n<7) place(n+1);
    		else output();
  		queen[n]++;
	end
end;

begin

	place(0);

end.
