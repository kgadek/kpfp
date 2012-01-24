-module(addressBook).
-author("Konrad Gadek").
-compile[export_all].


start() ->
    catch unregister(addressbook),
    register(addressbook, spawn(?MODULE, loop, [])).

stop() ->
    addressbook ! {self(), stop}.

loop() ->
    loop([]).

loop(State) ->
    receive
	{_From, save, Filename} ->
	    file:write_file(Filename, term_to_binary(State)),
	    loop(State);
	{_From, load, Filename} ->
	    loop(binary_to_term(file:read_file(Filename)));
	{From, getall} ->
	    From ! State,
	    loop(State);
	{_From, stop} ->
	    unregister(self());
	{_From, add_phone,Name,Surname,Phone} ->
	    loop(addPhone(State,Name,Surname,Phone));
	{_From, add_email,Name,Surname,Mail} ->
	    loop(addEmail(State,Name,Surname,Mail));
	{_From, add_contact,Name,Surname} ->
	    loop(addContact(State,Name,Surname));
	{_From, rem_phone,Name,Surname,Phone} ->
	    loop(removePhone(State,Name,Surname,Phone));
	{_From, rem_email,Name,Surname} ->
	    loop(removeEmail(State,Name,Surname));
	{_From, rem_contact,Name,Surname} ->
	    loop(removeContact(State,Name,Surname));
	{From, get_mails,Name,Surname} ->
	    From ! getEmails(State, Name, Surname),
	    loop(State);
	{From, get_phones,Name,Surname} ->
	    From ! getPhones(State,Name,Surname),
	    loop(State);
	{From, find_by_name,Name,Surname} ->
	    From ! findByName(State,Name,Surname),
	    loop(State);
	{From, find_by_mail, Mail} ->
	    From ! findByEmail(State,Mail),
	    loop(State);
        {From, find_by_phone,Phone} ->
	    From ! findByPhone(State,Phone),
 	    loop(State);
	{From, find_relatives, Name, Surname} ->
	    From ! findRelatives(State, Name, Surname),
	    loop(State);
	{From, get_relatives, Name, Surname} ->
	    From ! getRelatives(State, Name, Surname),
	    loop(State);
 	{_From, add_relative,Name,Surname,Relation,
	 RName, RSurname} ->
 	    loop(addRelative(State,Name,Surname,Relation,
 			     RName, RSurname));
 	{_From, rem_relative_rel, Name, Surname, Relation} ->
 	    loop(removeRelativeByRelation(State,
 					  Name,Surname,Relation));
 	{_From, rem_relative_name, Name, Surname,
	 RName, RSurname} ->
 	    loop(removeRelativeByName(State, Name, Surname,
 				      RName, RSurname))
    end.

createAddressBook() ->
    [].

makeEntry(Name, Surname, PhoneList, MailList, RelList) ->
    {{Name, Surname}, PhoneList, MailList, RelList}.


%% add contact
addContact(Name, Surname) ->
    addressbook ! {self(), add_contact, Name, Surname}.

addContact(L, Name, Surname) ->
    Contains = lists:keymember({Name, Surname}, 1, L),
    if
 	Contains ->
 	    L;
 	true ->
 	    E = makeEntry(Name, Surname, [], [], []),
 	    lists:keystore({Name, Surname}, 1, L, E)
    end.

%% add email
addEmail(Name, Surname, Mail) ->
    addressbook ! {self(), add_email, Name, Surname, Mail}.

addEmail(L, Name, Surname, Mail) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    ML = lists:keystore(Mail, 1, MailList, {Mail}),
    E = makeEntry(Name, Surname, PhoneList, ML, RelList),
    lists:keystore({Name, Surname}, 1, L1, E).

%% add phone
addPhone(Name, Surname, Phone) ->
    addressbook ! {self(), add_phone, Name, Surname, Phone}.

addPhone(L, Name, Surname, Phone) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    PL = lists:keystore(Phone, 1, PhoneList, {Phone}),
    E = makeEntry(Name, Surname, PL, MailList, RelList),
    lists:keystore({Name, Surname}, 1, L1, E).


%% add relative
addRelative(Name, Surname, Relation, RName, RSurname) ->
    addressbook ! {self(), add_relative,
 		   Name, Surname,
 		   Relation,
 		   RName, RSurname}.

addRelative(L, Name, Surname, Relation, RName, RSurname) ->
    L1 = addContact(L, Name, Surname),
    {_, PhoneList, MailList, RelList} = lists:keyfind({Name, Surname}, 1, L),
    RL = lists:keystore({RName, RSurname}, 1, RelList, {{RName, RSurname}, Relation}),
    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
    lists:keystore({Name, Surname}, 1, L1, E).


%% remove contact
removeContact(Name,Surname) ->
    addressbook ! {self(), rem_contact, Name, Surname}.

removeContact(L,Name,Surname) ->
    lists:keydelete({Name, Surname}, 1, L).


%% remove mail
removeEmail(Name, Surname, Mail) ->
    addressbook ! {self(), rem_email, Name, Surname, Mail}.

removeEmail(L, Name, Surname, Mail) ->
    case lists:keymember({Name, Surname}, 1, L) of
 	false ->
 	    L;
 	{_, PhoneList, MailList, RelList} ->
 	    ML = lists:keydelete(Mail, 1, MailList),
 	    E = makeEntry(Name, Surname, PhoneList, ML, RelList),
 	    lists:keystore({Name, Surname}, 1, L, E)
    end.


%% remove phone
removePhone(Name, Surname, Phone) ->
    addressbook ! {self(), rem_phone, Name, Surname, Phone}.

removePhone(L, Name, Surname, Phone) ->
    case lists:keymember({Name, Surname}, 1, L) of
 	false ->
 	    L;
 	{_, PhoneList, MailList, RelList} ->
 	    PL = lists:keydelete(Phone, 1, PhoneList),
 	    E = makeEntry(Name, Surname, PL, MailList, RelList),
 	    lists:keystore({Name, Surname}, 1, L, E)
    end.


%% remove relative
removeRelativeByRelation(Name, Surname, Relation) ->
    addressbook ! {self(), rem_relative_rel,Name,Surname,Relation}.

removeRelativeByRelation(L, Name, Surname, Relation) ->
    case lists:keymember({Name, Surname}, 1, L) of
 	false ->
 	    L;
 	{_, PhoneList, MailList, RelList} ->
 	    RL = lists:keydelete(Relation, 2, RelList),
 	    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
 	    lists:keystore({Name, Surname}, 1, L, E)
    end.


%% remove relative
removeRelativeByName(Name, Surname, RName, RSurname) ->
    addressbook ! {self(), rem_relative_name, Name, Surname,
 		   RName, RSurname}.

removeRelativeByName(L, Name, Surname, RName, RSurname) ->
    case lists:keymember({Name, Surname}, 1, L) of
 	false ->
 	    L;
 	{_, PhoneList, MailList, RelList} ->
 	    RL = lists:keydelete({RName, RSurname}, 1, RelList),
 	    E = makeEntry(Name, Surname, PhoneList, MailList, RL),
 	    lists:keystore({Name, Surname}, 1, L, E)
    end.


%% get mails
getEmails(Name, Surname) ->
    receive _ -> ok after 0 -> fail end,
    addressbook ! {self(), get_mails, Name, Surname},
    receive
	Result -> Result
    after 20000 -> failed
    end.

getEmails(L, Name, Surname) ->
    case lists:keyfind({Name,Surname}, 1, L) of
 	false ->
 	    [];
	{_, _PhoneList, MailList, _RelList} ->
	    MailList
    end.


%% get phones
getPhones(Name, Surname) ->
    receive _ -> ok after 0 -> fail end,
    addressbook ! {self(), get_phones, Name, Surname},
    receive
	Result ->
	    Result
    after 20000 -> failed
    end.

getPhones(L, Name, Surname) ->
    case lists:keyfind({Name,Surname}, 1, L) of
	false ->
	    [];
	{_, PhoneList, _MailList, _RelList} ->
	    PhoneList
    end.


%% get relatives
getRelatives(Name, Surname) ->
    receive _ -> ok after 0 -> ok end,
    addressbook ! {self(), get_relatives, Name, Surname},
    receive
	Result ->
	    Result
    after 20000 -> failed
    end.

getRelatives(L, Name, Surname) ->
    case lists:keyfind({Name, Surname}, 1, L) of
	false ->
	    [];
	{_, _PhoneList, _MailList, RelList} ->
	    RelList
    end.

%% find by name
findByName(Name, Surname) ->
    receive _ -> ok after 0 -> failed end,
    addressbook ! {self(), find_by_name, Name, Surname},
    receive Result ->
	    Result
    after 20000 -> failed end.

findByName(L, Name, Surname) ->
    receive _ -> ok after 0 -> ok end,
    case lists:keyfind({Name, Surname}, 1, L) of
	false ->
	    [];
	Res ->
	    Res
    end.


%% find by email
findByEmail(Mail) ->
    receive _ -> ok after 0 -> ok end,
    addressbook ! {self(), find_by_mail, Mail},
    receive Result ->
	    Result
    after 20000 -> failed end.

findByEmail(L, Mail) ->
    lists:filter(fun({_, _PhoneList, MailList, _RelList}) ->
			 lists:keymember(Mail, 1, MailList)
		 end,
		 L).

%% find by phone
findByPhone(Phone) ->
    receive _ -> ok after 0 -> ok end,
    addressbook ! {self(), find_by_phone, Phone},
    receive Result ->
	    Result
    after 20000 -> failed end.

findByPhone(L, Phone) ->
    lists:filter(fun({_, PhoneList, _MailList, _RelList}) ->
			 lists:keymember(Phone, 1, PhoneList)
		 end,
		 L).


%% find relatives
findRelatives(Name, Surname) ->
    receive _ -> ok after 0 -> ok end,
    addressbook ! {self(), find_relatives, Name, Surname},
    receive Result ->
	    Result
    after 20000 -> failed end.

findRelatives(L, Name, Surname) ->
    lists:filter(fun({_, _PhoneList, _MailList, RelList}) ->
			 lists:keymember({Name, Surname}, 1, RelList)
		 end,
		 L).
