-module(addressBook).
-author("Konrad Gadek").

-export([createAddressBook/0]).
-export([addContact/3, addEmail/4, addPhone/4, addRelative/6]).
-export([removeContact/3, removeEmail/4, removePhone/4]).
-export([removeRelativeByRelation/4, removeRelativeByName/5]).
-export([getEmails/3, getPhones/3, getRelatives/3	]).
-export([findByEmail/2, findByPhone/2, findRelatives/3]).

createAddressBook() ->
    [].

makeEntry(Name, Surname, PhoneList, MailList, RelList) ->
    {{Name, Surname}, PhoneList, MailList, RelList}.

addContact(L, Name, Surname) ->
    Contains = lists:keymember({Name, Surname}, 1, L),
    if
	Contains ->
	    L;
	true ->
	    E = makeEntry(Name, Surname, [], [], []),
	    lists:keystore({Name, Surname}, 1, L, E)
    end.


addEmail(L, Name, Surname, Mail) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    ML = lists:keystore(Mail, 1, MailList, {Mail}),
    E = makeEntry(Name, Surname, PhoneList, ML, RelList),
    lists:keystore({Name, Surname}, 1, L1, E).

addPhone(L, Name, Surname, Phone) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    PL = lists:keystore(Phone, 1, PhoneList, {Phone}),
    E = makeEntry(Name, Surname, PL, MailList, RelList),
    lists:keystore({Name, Surname}, 1, L1, E).

addRelative(L, Name, Surname, Relation, RName, RSurname) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    RL = lists:keystore({RName, RSurname}, 1, RelList, {{RName, RSurname}, Relation}),
    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
    lists:keystore({Name, Surname}, 1, L1, E).

removeContact(L, Name, Surname) ->
    lists:keydelete({Name, Surname}, 1, L).

removeEmail(L, Name, Surname, Mail) ->
    case lists:keymember({Name, Surname}, 1, L) of
	false ->
	    L;
	{_, PhoneList, MailList, RelList} ->
	    ML = lists:keydelete(Mail, 1, MailList),
	    E = makeEntry(Name, Surname, PhoneList, ML, RelList),
	    lists:keystore({Name, Surname}, 1, L, E)
    end.

removePhone(L, Name, Surname, Phone) ->
    case lists:keymember({Name, Surname}, 1, L) of
	false ->
	    L;
	{_, PhoneList, MailList, RelList} ->
	    PL = lists:keydelete(Phone, 1, PhoneList),
	    E = makeEntry(Name, Surname, PL, MailList, RelList),
	    lists:keystore({Name, Surname}, 1, L, E)
    end.

removeRelativeByRelation(L, Name, Surname, Relation) ->
    case lists:keymember({Name, Surname}, 1, L) of
	false ->
	    L;
	{_, PhoneList, MailList, RelList} ->
	    RL = lists:keydelete(Relation, 2, RelList),
	    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
	    lists:keystore({Name, Surname}, 1, L, E)
    end.

removeRelativeByName(L, Name, Surname, RName, RSurname) ->
    case lists:keymember({Name, Surname}, 1, L) of
	false ->
	    L;
	{_, PhoneList, MailList, RelList} ->
	    RL = lists:keydelete({RName, RSurname}, 1, RelList),
	    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
	    lists:keystore({Name, Surname}, 1, L, E)
    end.

getEmails(L, Name, Surname) ->
    case lists:keyfind({Name,Surname}, 1, L) of
	false ->
	    [];
	{_, _PhoneList, MailList, _RelList} ->
	    MailList
    end.

getPhones(L, Name, Surname) ->
    case lists:keyfind({Name,Surname}, 1, L) of
	false ->
	    [];
	{_, PhoneList, _MailList, _RelList} ->
	    PhoneList
    end.

getRelatives(L, Name, Surname) ->
    case lists:keyfind({Name, Surname}, 1, L) of
	false ->
	    [];
	{_, _PhoneList, _MailList, RelList} ->
	    RelList
    end.

findByEmail(L, Mail) ->
    lists:filter(fun({_, _PhoneList, MailList, _RelList}) ->
			 lists:keymember(Mail, 1, MailList)
		 end,
		 L).

findByPhone(L, Phone) ->
    lists:filter(fun({_, PhoneList, _MailList, _RelList}) ->
			 lists:keymember(Phone, 1, PhoneList)
		 end,
		 L).

findRelatives(L, Name, Surname) ->
    lists:filter(fun({_, _PhoneList, _MailList, RelList}) ->
			 lists:keymember({Name, Surname}, 1, RelList)
		 end,
		 L).
