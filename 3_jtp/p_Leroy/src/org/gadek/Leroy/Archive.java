package org.gadek.Leroy;

import java.text.*;
import java.util.*;

class Msg {
	private String message;
	private String sender;
	
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	public String getSender() {
		return sender;
	}
	public void setSender(String sender) {
		this.sender = sender;
	}
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<message from=\"" + sender + "\">");
		buf.append(message);
		buf.append("</message>\n");
		return buf.toString();
	}
}

class Session {
	List<Msg> msgs = new ArrayList<Msg>();
	String date;
	
	public List<Msg> getMsgs() {
		return msgs;
	}

	public void setMsgs(List<Msg> msgs) {
		this.msgs = msgs;
	}

	public void addMsg(Msg m) {
		msgs.add(m);
	}

	public String getDate() {
		return date;
	}
	
	Session() {
		date = DateFormat.getDateInstance().format(new Date());
	}
	
	public void setDate(String date) throws ParseException {
		DateFormat df = DateFormat.getDateInstance();
		this.date = df.format(df.parse(date)); // konwersja w obie strony -- sprawdzenie, czy poprawny String
	}
	
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<session date=\"" + date + "\">\n");
		for(Msg m : msgs)
			buf.append(m.toXML());
		buf.append("</session>\n");
		return buf.toString();
	}
}

class Archive {
	List<Session> ses = new ArrayList<Session>();
	
	public void addSession(Session s) {
		ses.add(s);
	}
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<?xml version=\"1.0\"?>\n");
		buf.append("<archive>\n");
		for(Session s: ses)
			buf.append(s.toXML());
		buf.append("</archive>\n");
		return buf.toString();
	}
}
