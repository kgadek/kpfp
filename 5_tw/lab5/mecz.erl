-module(mecz).
-export([start/2, kopnij/1, stop/1, startMeczu/3]).
-author("Konrad Gadek").


%% Proces dru¿yny
start(KrajNazwa, PartnerNick) ->
	random:seed(erlang:now()),
	start(KrajNazwa, PartnerNick, 0,0).
start(KrajNazwa, PartnerNick, JaGoli, OnGoli) ->
	timer:sleep(1000),
	receive 
		stop ->
			io:format("~w: koniec meczu z wynikiem ~B:~B~n", [KrajNazwa, JaGoli,OnGoli]),
			ok;
		pass ->
			io:format("~w: dostalem pilke... wiec strzelam!~n",[KrajNazwa]),
			whereis(PartnerNick) ! kopnij,
			start(KrajNazwa, PartnerNick, JaGoli, OnGoli);
		bramka ->
			io:format("~w: strzelilem gola! jest wynik ~B:~B~n",[KrajNazwa,JaGoli+1,OnGoli]),
			whereis(PartnerNick) ! pass,
			start(KrajNazwa, PartnerNick, JaGoli+1, OnGoli);
		obroniona ->
			io:format("~w: nie udalo sie strzelic mi gola...~n",[KrajNazwa]),
			whereis(PartnerNick) ! pass,
			start(KrajNazwa, PartnerNick, JaGoli, OnGoli);
		kopnij ->
			Res = random:uniform() < 0.2,
			if
				Res ->
					io:format("~w: ... przeciwnik strzelil gola, jest ~B:~B~n",[KrajNazwa, JaGoli, OnGoli+1]),
					whereis(PartnerNick) ! bramka,
					start(KrajNazwa, PartnerNick, JaGoli, OnGoli+1);
				true ->
					io:format("~w: przeciwnik nie strzelil gola, uf.~n",[KrajNazwa]),
					whereis(PartnerNick) ! obroniona,
					start(KrajNazwa, PartnerNick, JaGoli, OnGoli)
			end
	end.


%% Inicjuje wymianê
kopnij(KrajNick) -> whereis(KrajNick) ! kopnij.


%% Koñczy mecz
stop(KrajNick) -> whereis(KrajNick) ! stop.


%% Przeprowadza rozgrywkê
startMeczu(KrajA, KrajB, Czas) ->
	catch unregister(aaa),
	catch unregister(bbb),
	PidA = spawn_link(?MODULE, start, [KrajA, bbb]),
	timer:sleep(500),
	PidB = spawn_link(?MODULE, start, [KrajB, aaa]),
	register(aaa, PidA),
	register(bbb, PidB),
	kopnij(aaa),
	timer:sleep(Czas*1000),
	stop(aaa),
	stop(bbb),
	startMeczuDone.

