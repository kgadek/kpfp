-module(ss).
-author("Konrad Gadek").
-export([start/0, stop/0]).
-export([distWork/1, moarWork/1, statusWork/0, addWork/2]).
-export([radek/0]).

start() ->
    catch unregister(radek),
    register(radek, spawn(ss, radek,[])),
    ok.

radek() ->
    JobList = [{konrad, [preparser, integracja, generator]}, {idzik, [guiJung, gui]}, {gwidon, [guiSuwaczki]}, {niemczur, [dokumentacja]}, {dominika, [generator]}, {anka, [instrukcja]}],
    loop(JobList).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% funkcje użytkownika
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

stop() ->
    asyncCall(stop).
addWork(Worker, Task) ->
    syncCall({addWork, Worker, Task}).
distWork(Worker) ->
    syncCall({distWork, Worker}).
moarWork(Worker) ->
    syncCall({moarWork, Worker}).
statusWork() ->
    syncCall(statusWork).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% helpery
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

syncCall(Msg) ->
    whereis(radek) ! {req, self(), Msg},
    receive
	{{ack, Msg}, Reply} ->
	    Reply
    after 3000 ->
	    fail	      
    end.

asyncCall(Msg) ->
    whereis(radek) ! {req, Msg},
    ok.

reply(Pid, Req, Msg) ->
    Pid ! {{ack, Req}, Msg}.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% main loop
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

loop(Jobs) ->
    receive
	{req, Pid, {addWork, Worker, Task}} ->
	    {NewJobs, Rep} = addWork(Jobs, Worker, Task),
	    reply(Pid, {addWork, Worker, Task}, Rep),
	    loop(NewJobs);
	{req, Pid, {distWork, Person}} ->
	    {NewJobs, Rep} = distWork(Jobs, Person),
	    reply(Pid, {distWork, Person}, Rep),
	    loop(NewJobs);
	{req, Pid, {moarWork, Person}} ->
	    Rep = moarWork(Jobs, Person),
	    reply(Pid, {moarWork, Person}, Rep),
	    loop(Jobs);
	{req, Pid, statusWork} ->
	    Rep = statusWork(Jobs),
	    reply(Pid, statusWork, Rep),
	    loop(Jobs);
	{req, stop} ->
	    io:format("No to papatki~n")
	end.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% funkcje
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

distWork(Jobs, Person) ->
    case lists:keyfind(Person, 1, Jobs) of
	false ->
	    {Jobs, nothing};
	{Person, [Work]} ->
	    NewJobs = lists:keydelete(Person, 1, Jobs),
	    {NewJobs, Work};
	{Person, [Work|WorkR]} ->
	    NewJobs = lists:keyreplace(Person, 1, Jobs, {Person, WorkR}),
	    {NewJobs, Work}
    end.

addWork(Jobs, Person, Task) ->
    case lists:keyfind(Person, 1, Jobs) of
	false ->
	    NewJobs = lists:keystore(Person, 1, Jobs, {Person, [Task]}),
	    {NewJobs, ok};
	{Person, Work} ->
	    NewJobs = lists:keystore(Person, 1, Jobs, {Person, [Task|Work]}),
	    {NewJobs, ok}
    end.

moarWork(Jobs, Person) ->
    case lists:keyfind(Person, 1, Jobs) of
	false ->
	    [];
	{Person, Work} ->
	    Work
    end.

statusWork(Jobs) ->
    Jobs.
