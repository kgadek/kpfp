-module(ex1).
-export([procesA/1, procesB/0]).
-author("Konrad Gadek").

procesA(GreetWho) ->
	GreetWho ! {self(), hello},
	ok.

procesB() ->
	receive
		{From, hello} ->
			io:format("Hello, ~w~n!",[From]),
			procesB();
		{_, goodbye} ->
			io:format("Goodbye, cruel world...~n")
	end.

