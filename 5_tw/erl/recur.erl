-module(recur).
-compile([export_all, debug_info]).

reverse(L) -> reverse(L,[]).
reverse([],Acc) -> Acc;
reverse([H|T],Acc) -> reverse(T,[H|Acc]).

zip([],[]) -> [];
zip([X|Xs], [Y|Ys]) -> [{X,Y}|zip(Xs,Ys)].

zip_rec(Xs,Ys) -> reverse(zip_rec(Xs,Ys,[])).
zip_rec([],[],L) -> L;
zip_rec([X|Xs],[Y|Ys],L) -> zip_rec(Xs,Ys,[{X,Y}|L]).

%% quicksort
qs([]) -> [];
qs([Pivot|T]) ->
    qs([X || X <- T, X =< Pivot ]) ++ [Pivot] ++ qs([X || X <- T, X > Pivot]).
