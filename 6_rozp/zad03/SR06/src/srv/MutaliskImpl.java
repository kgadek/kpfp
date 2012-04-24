package srv;

import java.util.ArrayList;
import java.util.List;

import lab.MutaliskPOA;
import lab.Overlord;

public class MutaliskImpl extends MutaliskPOA {
	
	private List<Overlord> overlords = new ArrayList<Overlord>();
	private String name;
	private int id;

	@Override
	public void disableWPA() {
		command("<mutalisk " + name + "> *skrzek* GRROOOOOOOOOORRRR [wylacza AGH-WPA]");
	}

	@Override
	public void justFly() {
		command("<mutalisk " + name + "> GRRAAARRRR [sobie lata]");
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
	public void command(String cmd) {
		System.out.println("<mutalisk> received command: " + cmd);
		System.out.println("<mutalisk>                   groar?");
		for(Overlord i : overlords)
			i.detector(cmd);
	}

	@Override
	public void add_overmind(Overlord lstnr) {
		overlords.add(lstnr);
	}

	@Override
	public void rem_overmind(Overlord lstnr) {
		overlords.remove(lstnr);
	}

}
