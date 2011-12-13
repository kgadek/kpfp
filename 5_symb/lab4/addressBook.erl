-module(addressBook).
-author("Konrad Gadek").
-compile(export_all).
%-export([]).

createAddressBook() ->
    [].

findByNameSurname([], _, _) ->
    false;
findByNameSurname([{Name, Surname, _Phones, _Emails}|_], Name, Surname) ->
    true;
findByNameSurname([_|T], Name, Surname) ->
    findByNameSurname(T,Name,Surname).

addContact(Book, Name, Surname) ->				     
    C = findByNameSurname(Book, Name, Surname),
    if C =:= false ->
	    [{Name, Surname, [], []}|Book];
       true ->
	    Book
    end.
