-module(kpfp).
-export([fold/3, map/2, filter/2]).
-author("Konrad Gadek").

%% From "Learn you some Erlang for great good"
fold(_, Acc, []) -> Acc;
fold(F, Acc, [H|T]) -> fold(F, F(H,Acc), T).

%% From "Learn you some Erlang for great good"
%reverse(L) ->
%    fold(fun(X,Acc) -> [X|Acc] end, [], L).

%% From "Learn you some Erlang for great good"
map(F,L) ->
    lists:reverse(fold(fun(X,Acc) -> [F(X)|Acc] end, [], L)).

%% From "Learn you some Erlang for great good"
filter(Pred, L) ->
    F = fun(X,Acc) ->
            case Pred(X) of
                true -> [X|Acc];
                false -> Acc
            end
    end,
    lists:reverse(fold(F, [], L)).

