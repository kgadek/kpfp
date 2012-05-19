package srv;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import lab.MutaliskPOA;
import lab.Overlord;

public class MutaliskImpl extends MutaliskPOA {
	
	private List<Overlord> overlords = new ArrayList<Overlord>();
	private String name;
	private int id;
	private int ticket = (new Random()).nextInt(100);
	private boolean controlled = false;
	
	@Override
	public void disableWPA(int ticket) {
		command(ticket,"<mutalisk " + name + "> *skrzek* GRROOOOOOOOOORRRR [wylacza AGH-WPA]");
	}

	@Override
	public void justFly(int ticket) {
		command(ticket,"<mutalisk " + name + "> GRRAAARRRR [sobie lata]");
	}

	@Override
	public String name() {
		return name;
	}

	@Override
	public void name(String newName) {
		name = newName;
	}

	@Override
	public int id() {
		return id;
	}

	@Override
	public void id(int newId) {
		id = newId;
	}

	@Override
	public void add_overmind(Overlord lstnr) {
		overlords.add(lstnr);
	}

	@Override
	public void rem_overmind(Overlord lstnr) {
		overlords.remove(lstnr);
	}

	@Override
	public void command(int ticket, String cmd) {
		if(ticket != this.ticket)
			return;
		System.out.println("<mutalisk> received command: " + cmd);
		System.out.println("<mutalisk> responded: groaaaarrr!");
		for(Overlord i : overlords)
			i.detector(cmd);
	}

	@Override
	public int getTicket() {
		System.out.format("==LOG: getTicket\n");
		if(controlled)
			return -1;
		controlled = true;
		ticket = (new Random()).nextInt(100);
		return ticket;
	}

	@Override
	public void relTicket() {
		controlled = false;
	}

}
