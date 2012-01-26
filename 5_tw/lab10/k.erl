-module(k).
-author("Konrad Gadek").
-export([testm/2, testpm/2]).

fib(0) -> 0;
fib(1) -> 1;
fib(N) when N > 0 ->
	       fib(N-1) + fib(N-2).

testm(N,M) ->
    lists:map(
      fun(X) -> fib(X) end,
      lists:map(fun(X) -> X rem M end,
		lists:seq(0,N))).

testpm(N,M) ->
    pm:pmap(
      fun(X) -> fib(X) end,
      lists:map(fun(X) -> X rem M end,
		lists:seq(0,N))).
			   
