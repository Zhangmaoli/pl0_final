const N = 3;
var i,j;

// 
// print() Êä³ö»»ÐÐ·û 
begin

	for(j=0;j<10;j+=1)
		begin
			i = j ;
			switch (i)
  			begin
	  			case 0 :
	  			case (3 > 2)+2 : print(i);break; 
	  			case 1+N*2 : print(i);
	   	       			 break;
	  			default: switch (i)
	  							   begin
	  							     case 2: case 4:case 6 : case 8: print(-i);break;
	  							     default:print(i*3);
	  							   end
				end
		end
	print();
end.
    