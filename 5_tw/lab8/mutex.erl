-module(mutex).
-author("Konrad Gadek").
-compile([export_all, debug_info]).

start() ->
    try
	exit(whereis(mutex), kill),
	unregister(mutex),
	renewed_mutex
    catch
	error:badarg -> fresh_n_clean
    after
	register(mutex, spawn(?MODULE, init, []))
    end.

stop() ->
    mutex ! stop.

wait() ->
    mutex ! {self(), wait},
    receive ok -> ok end.

signal() ->
    mutex ! {self(), signal},
    ok.

init() ->
    free().

free() ->
    receive
	{Pid, wait} ->
	    Pid ! ok,
	    busy(Pid);
	stop ->
	    terminate()
    end.

busy(Pid) ->
    receive
	{Pid, signal} ->
	    free
    end.

terminate() ->
    receive
	{Pid, wait} ->
	    exit(Pid, kill),
	    terminate()
    after
	0 -> ok
    end.
	    
