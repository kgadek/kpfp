-module(klist).
-author("Konrad Gadek").

-export([make_toy/5,duplicateElements/1,contains/2,removeInts/1]).

make_toy(Type, Name, Price, Weight, Colour) ->
     {Type, Name, Price, Weight, Colour}.

contains([], _) ->
    false;
contains([V|_], V) ->
    true;
contains([_|T], V) ->
    contains(T,V).

duplicateElements([]) ->
    [];
duplicateElements([H|T]) ->			 
    [H|[H|duplicateElements(T)]].

removeInts([]) ->
    [];
removeInts([H|T]) when is_integer(H) ->
    removeInts(T);
removeInts([H|T]) ->
    [H|removeInts(T)].
