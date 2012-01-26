-module(pm).
-author("Konrad Gadek").
-export([pmap/2]).

gather([H|T]) ->
    receive
	{H, Ret} -> [Ret|gather(T)]
    end;
gather([]) ->
    [].

do_f(Parent, F, I) ->
    Parent ! {self(), (catch F(I))}.

pmap(F, L) ->
    S = self(),
    Nod = [node() | nodes()],
    {Pids, _} = lists:mapfoldl(
		  fun(I, {N1, N2}) ->
			  case N1 == [] of
			      true -> N = N2;
			      false -> N = N1
			  end,
			  [H|T] = N,
			  {spawn(H, fun() -> do_f(S, F, I) end), {T, N2}}
		  end, {Nod, Nod}, L),

    gather(Pids).
					    
