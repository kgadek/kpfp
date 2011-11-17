-module(ex3).
-export([procesUniw/2, procesC1/0, procesC2/0, procesC2B/0, procesC3/0, procesUniwIt/4]).
-author("Konrad Gadek").

%% Uniwersalny proces. Wysy³a co 1s do Pid wiadomoœæ Msg.
procesUniw(Pid,Msg) ->
	procesUniw(Pid,Msg,1).

procesUniw(Pid,Msg,Iter) ->
	timer:sleep(1000),
	Pid ! {self(), Msg, Iter},
	procesUniw(Pid,Msg,Iter+1).

procesUniwIt(Pid,Msg,Iter,MaxIter) ->
	Iter < MaxIter andalso
		Pid ! {self(), Msg, Iter},
		procesUniwIt(Pid, Msg, Iter+1, MaxIter).

%% Proces C1: odbiera tylko aaa oraz bbb (na przemian).
procesC1() ->
	procesC1(aaa,bbb).
procesC1(ExpMsg,NextMsg) ->
	receive
		{From, ExpMsg, _} ->
			io:format("Dostalem ~s od ~w~n",[ExpMsg, From]),
			procesC1(NextMsg, ExpMsg)
	end.

%% Proces C2: odbiera wiadomoœci wg kolejnoœci ich przychodzenia. Akceptuje
%% tylko dwa typy wiadomoœci
procesC2() ->
	receive
		{From, aaa, _} ->
			io:format("Dostalem aaa od ~w~n",[From]);
		{From, bbb, _} ->
			io:format("Dostalem bbb od ~w~n",[From])
	end,
	procesC2().

%% Proces C2B: odbiera wiadomoœci wg kolejnoœci ich przychodzenia. Akceptuje
%% tylko dwa typy wiadomoœci. Dodatkowo wyœwietla nr. iteracji, w której
%% wys³ana zosta³a wiadomoœæ.
procesC2B() ->
	receive
		{From, aaa, Iter} ->
			io:format("Dostalem aaa od ~w z ~w iteracji~n",[From,Iter]);
		{From, bbb, Iter} ->
			io:format("Dostalem bbb od ~w z ~w iteracji~n",[From,Iter])
	end,
	procesC2B().

%% Proces C3: odbieranie dowolnych wiadomoœci
procesC3() ->
	receive
		{From, Msg, Iter} ->
			io:format("Dostalem ~w od ~w z ~w iteracji~n",[Msg,From,Iter]);
		{From, Msg} ->
			io:format("Dostalem ~w od ~w~n",[Msg,From]);
		Msg ->
			io:format("Dostalem ~w~n",[Msg])
	end,
	procesC3().

