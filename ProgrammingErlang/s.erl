-module(s).
-author("Konrad Gadek").
-compile([export_all,debug_info]).
-export([start/2]).

%% @spec start(atom(), function()) -> boolean()
start(Atom, Fun) ->
    Pid = spawn(?MODULE, loop, [sets:new()]),
    Me = self(),
    try register(pid, Pid)
    catch _:_X -> Pid ! {Me, quit} end,
    pid ! {Me, Atom, Fun},
    receive
	{Me, Atom, Fun, State} -> State
    end.

loop(L) ->
    receive
	{_Pid, quit} ->
	    void;
	{Pid, Atom, Fun} ->
	    case sets:is_element(Atom, L) of
	       true -> Pid ! {Pid, Atom, Fun, false},
		       loop(L);
	       false -> Stat = register(Atom, spawn(Fun)),			
			Pid ! {Pid, Atom, Fun, Stat},
			case Stat of
			    true -> loop(sets:add_element(Atom, L));
			    false -> loop(L)
			end
	    end
    end.

bla() ->
    bla(make_ref()).
bla(Msg) ->
    receive
	quit -> void
    after 1000 ->
	    io:format("~p", [Msg]),
	    bla(Msg)
    end.
