-module(fact).
-author("Konrad Gadek").
-compile([export_all]).

-export([wsp/1, sekw/1]).

sekw_fast(0) -> % wersja sekwencyjna szybka
    [1];
sekw_fast(N) ->
    [H|T] = sekw(N-1),
    [H*N|[H|T]].

sekw(0) -> % wersja sekwencyjna normalna
    [1];
sekw(N) ->
    [wsp_innerB(N)|sekw(N-1)].

wsp(N) -> % wersja współbieżna I
    wsp_spawn(N),
    wsp_rec(0,N,[]).

wsp_rec(A,A,State) ->
    receive
	{factWA, A, Result} ->
	    [Result|State]
    end;
wsp_rec(A,B,State) ->
    receive
	{factWA, A, Result} ->
	    wsp_rec(A+1, B, [Result|State])
    end.

wsp_spawn(-1) -> ok;
wsp_spawn(N) ->
    spawn(?MODULE, wsp_innerA, [N, self()]),
    wsp_spawn(N-1).

wsp_innerA(N, To) ->
    To ! {factWA, N, wsp_innerB(N)}.

wsp_innerB(0) ->
    1;
wsp_innerB(N) ->
    N*wsp_innerB(N-1).



wsp2(N, Core) -> % worker pool
    wsp2(0,N,Core),
    wsp2_rec(0,N,[]).

wsp2(N,N,_Core) ->
    ok;
wsp2(I,N,Core) ->
    spawn(?MODULE, wsp2_calc, [I,N,self()]),
    wsp2(I+1,N,Core).

wsp2_calc(X,N,_Root) when X>N ->
    ok;
wsp2_calc(X,N,Root) ->
    Root ! {factWB, X, wsp_innerB(X)},
    wsp2_calc(X+N,N,Root).

wsp2_rec(I,N,State) when I>N ->
    State;
wsp2_rec(I,N,State) ->
    receive
	{factWB, I, Y} ->
	    wsp2_rec(I+1,N,[Y|State])
    end.
