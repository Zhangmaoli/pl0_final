procedure fib(n);
begin
	if n <= 1 then
            return n;
	else return fib(n-1) + fib(n-2);
end;

begin
	call writeln(fib(21));
	call writeln(fib(8));
	call writeln(fib(fib(fib(5))));
	//print();
end.