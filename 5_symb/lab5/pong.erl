-module(pong).
-author("Konrad Gadek").
-export([start/0, stop/0, play/1, proc/0]).

start() ->
    catch unregister(pingP),
    catch unregister(pongP),
    register(pingP, spawn(?MODULE, proc, [])),
    register(pongP, spawn(?MODULE, proc, [])).

stop() ->
    catch unregister(pingP),
    catch unregister(pongP).

play(N) ->
    pingP ! {pongP, pingP, N}.

proc() ->
    receive
	{To, From, N} ->
	    timer:sleep(1000),
	    io:format("Dostalem ~p, odsylam dalej do ~p~n", [N, To]),
	    if N > 0 ->
		    To ! {From, To, N-1};
	       true -> ok
	    end,
	    proc()
    after 20000 ->
	      ok
    end.
