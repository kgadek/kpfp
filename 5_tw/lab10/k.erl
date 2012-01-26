-module(k).
-author("Konrad Gadek").
-compile(export_all).

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

fA([],_, _) ->
    ok;
fA([A1|A2],B, Func) ->
    fB(A1, B, Func),
    fA(A2, B, Func).

fB(_, [], _) ->
    ok;
fB(A, [B1|B2], Func) ->
    {T1, _} = timer:tc(Func, [A, B1]),
    {T2, _} = timer:tc(Func, [A, B1]),
    {T3, _} = timer:tc(Func, [A, B1]),
    io:format("(~p,~p,~p) = ~.3f ~n", [A, B1, Func, (T1 + T2 + T3)/3]),
    fB(A,B2, Func).
