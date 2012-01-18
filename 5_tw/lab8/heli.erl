-module(heli).
-author("Konrad Gadek").
-compile([export_all, debug_info]).
%-export([start/0, ctrl/1]).

start() ->
    try
	exit(whereis(heli), kill),
	unregister(heli),
	respawned_heli
    catch
	error:badarg -> fresh_n_clean
    after
	register(heli, spawn(?MODULE, on_ground_off, [0,0]))
    end.

ctrl(Msg) ->
    % spin_rotors, stop_rotors, take_off, land,
    % fly_front, fly_back, fly_up, fly_down,
    % fly_left, fly_right
    whereis(heli) ! {self(), Msg}.

on_ground_off(X, Y) ->
    receive
	{_Pid, spin_rotors} ->
	    spin_rotors(X,Y),
	    on_ground_on(X,Y);
	{_Pid, quit} ->
	    io:format("Bye bye~n"),
	    state(0,X,Y)
    end.

on_ground_on(X, Y) ->
    receive
	{_Pid, stop_rotors} ->
	    stop_rotors(X,Y),
	    on_ground_off(X,Y);
	{_Pid, take_off} ->
	    take_off(X,Y),
	    in_the_air(0,X,Y)
    end.

in_the_air(H, X, Y) ->
    receive
	{_Pid, land} ->
	    if H =:= 0 ->
		    land(X,Y),
		    on_ground_on(X,Y);
	       true ->
		    in_the_air(H,X,Y)
	    end;
	{_Pid, fly_up} ->
	    fly_up(H+1,X,Y),
	    in_the_air(H+1,X,Y);
	{_Pid, fly_down} ->
	    if H > 0 ->
		    fly_down(H-1,X,Y),
		    in_the_air(H-1,X,Y);
	       true ->
		    in_the_air(H,X,Y)
	    end;
	{_Pid, fly_front} ->
	    if H > 0 ->
		    fly_front(H,X+1,Y),
		    in_the_air(H,X+1,Y);
	       true ->
		    in_the_air(H,X,Y)
	    end;
	{_Pid, fly_back} ->
	    if H > 0 ->
		    fly_back(H,X-1,Y),
		    in_the_air(H,X-1,Y);
	       true ->
		    in_the_air(H,X,Y)
	    end;
	 {_Pid, fly_left} ->
	    if H > 0 ->
		    fly_left(H,X,Y-1),
		    in_the_air(H,X,Y-1);
	       true ->
		    in_the_air(H,X,Y)
	    end;
	{_Pid, fly_right} ->
	    if H > 0 ->
		    fly_right(H,X,Y+1),
		    in_the_air(H,X,Y+1);
	       true ->
		    in_the_air(H,X,Y)
	    end
    end.
		    

spin_rotors(X,Y) ->
    io:format("Uruchamiam silnik...~n"),
    state(0,X,Y).
stop_rotors(X,Y) ->
    io:format("Zatrzymuje silnik...~n"),
    state(0,X,Y).
take_off(X,Y) ->
    io:format("Startuje...~n"),
    state(0,X,Y).
land(X,Y) ->
    io:format("Laduje...~n"),
    state(0,X,Y).
fly_up(H,X,Y) ->
    io:format("Zwiększa pulap...~n"),
    state(H,X,Y).
fly_down(H,X,Y) ->
    io:format("Obniza pulap...~n"),
    state(H,X,Y).
fly_front(H,X,Y) ->
    io:format("Lece do przodu...~n"),
    state(H,X,Y).
fly_back(H,X,Y) ->
    io:format("Lece do tylu...~n"),
    state(H,X,Y).
fly_left(H,X,Y) ->
    io:format("Lece w lewo...~n"),
    state(H,X,Y).
fly_right(H,X,Y) ->
    io:format("Lece w prawo...~n"),
    state(H,X,Y).
state(H,X,Y) ->
    io:format("    Jestem na wysokosci ~B i na pozycji (~B,~B)~n", [H, X, Y]).
