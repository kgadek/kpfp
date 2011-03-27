package org.gadek.Leroy;

import org.xml.sax.*;
import org.xml.sax.helpers.*;

class ArchiveHandler extends DefaultHandler {
	private Archive a = new Archive();
	private Session s = null;
	private Msg m = null;
	
	public Archive getArchive() {
		return a;
	}
	public void startDocument() throws SAXException { }
	public void endDocument() throws SAXException { }
	public void startElement(String namespaceURI, String localName, String qname, Attributes attr) throws SAXException {
		if(localName.equalsIgnoreCase("message")) {
			m = new Msg();
			m.setSender(attr.getValue("from"));
		} else if(localName.equalsIgnoreCase("session")) {
			s = new Session();
			m = null;
		}
	}
	public void endElement(String namespaceURI, String localName, String qname) throws SAXException {
		if(localName.equalsIgnoreCase("session")) {
			a.addSession(s);
			s = null;
		} else if(localName.equalsIgnoreCase("message")) {
			s.addMsg(m);
			m = null;
		}
	}
	public void characters(char[] ch, int start, int length) throws SAXException {
		String tmp = new String(ch,start,length).trim();
		if(m!=null)
			m.setMessage(tmp);
	}
}