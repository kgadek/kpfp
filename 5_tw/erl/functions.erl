-module(functions).
-compile([export_all, debug_info]).
-author("Konrad Gadek").

head([H|_]) -> H.
second([_, X|_]) -> X.

same(X,X) -> true;
same(_,_) -> false.

valid_time({{Y,M,D}, {H,Min,_}}) ->
    io:format("Today is ~p:~p:~p, ~p:~p~n", [Y,M,D,H,Min]);
valid_time(_) ->
    io:format("Ble!~n").
