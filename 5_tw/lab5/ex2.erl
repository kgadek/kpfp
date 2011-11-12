-module(ex2).
-export([procesA/1, procesB/0]).
-author("Konrad Gadek").

procesA(GreetWho) ->
	GreetWho ! {self(), hello},
	ok.

procesB() ->
	receive
		{_, goodbye} ->
			io:format("Goodbye, cruel world...~n")
	after 5000 ->
			io:format("Ej, nudno tutaj. Przyslij cos, co zrozumiem!~n",[]),
			procesB()
	end.

