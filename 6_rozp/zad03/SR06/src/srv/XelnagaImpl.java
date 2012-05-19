package srv;

import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;

import lab.Drone;
import lab.Overlord;
import lab.XelnagaPOA;

import org.omg.PortableServer.POA;
import org.omg.PortableServer.Servant;
import org.omg.PortableServer.POAPackage.ServantAlreadyActive;
import org.omg.PortableServer.POAPackage.ServantNotActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XelnagaImpl extends XelnagaPOA {

	private int xid;
	private List<Overlord> overlords = new ArrayList<Overlord>();
	private List<Drone> drones = new ArrayList<Drone>();
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public XelnagaImpl(POA rootpoa) throws ServantAlreadyActive, WrongPolicy, ServantNotActive, InstantiationException, IllegalAccessException {
		try {
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(new File("../config.xml"));
			// a w Clojure to wyżej wygląda tak:
			// 		(. DocumentBuilderFactory NewInstance newDocumentBuilder (parse (File. "config.xml")))
			// Lisp bardziej przejrzysty niż Java? :D 
			doc.getDocumentElement().normalize();
			NodeList listOfDrones = doc.getElementsByTagName("drones");
			for(int idx = 0; idx < listOfDrones.getLength(); ++idx) { // nie ma iteratora? a to smut ;<
				Node droneNode = listOfDrones.item(idx);
				if(droneNode.getNodeType() != Node.ELEMENT_NODE)
					continue;
				
				String type;
				String name;
				String idS;
				int id;
				Element thisDroneOrAThing = null;
				try {
					thisDroneOrAThing = (Element)droneNode;

					type = ((Node)
								((Element) thisDroneOrAThing.getElementsByTagName("type").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
					name = ((Node)
								((Element) thisDroneOrAThing.getElementsByTagName("name").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
					idS = ((Node)
								((Element) thisDroneOrAThing.getElementsByTagName("id").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
					id = Integer.parseInt(idS);

					Servant currI = (Servant) Class.forName("srv."+type+"Impl").newInstance();
					rootpoa.activate_object(currI);
					Class c = Class.forName("lab."+type+"Helper");
					Method m = c.getDeclaredMethod("narrow", org.omg.CORBA.Object.class);
					Drone curr = (Drone) m.invoke(null, rootpoa.servant_to_reference(currI));
					
					curr.id(id);
					curr.name(name);
					System.out.format("==LOG: adding type=%s id=%d name=%s\n", type, id, name);
					drones.add(curr);
				} catch (Exception e) {
					System.out.format("==LOG: couldn't parse: %s\n", thisDroneOrAThing.getTextContent());  
					System.out.format("==LOG: stacktrace:\n");
					e.printStackTrace();
				}
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public int xid() {
		return xid;
	}

	@Override
	public void xid(int newXid) {
		xid = newXid;
	}

	@Override
	public Drone searchDroneByName(String name) {
		if(name != null)
			for(Drone i : drones)
				if(name.equals(i.name()))
					return i;
		return null;
	}

	@Override
	public Drone[] searchSpaceAndDestroyProtossRace() {
		// TODO: destroy every protoss
		Drone[] ret = new Drone[drones.size()];
		int idx = 0;
		for(Drone d : drones)
			ret[idx++] = d;
		return ret;
	}

	@Override
	public void observeDrone(int id, Overlord lstnr) {
		String msg = "<drone " + id + " in reaction to registering new overlord (listener)> Blumbbbrrrr!";
		System.out.println("LOG: " + msg);
		System.out.println("LOG: listener = " + lstnr);
		if(lstnr == null) {
			System.err.println("LOG: Oh wait... It's null ;<");
			return;
		}
			
		for(Drone i : drones)
			if(i.id() == id)
				i.add_overmind(lstnr);
		for(Overlord i : overlords)
			i.detector(msg);
	}

	@Override
	public void unobserveDrone(int id, Overlord lstnr) {
		String msg = "<drone in reaction to " + id + " deregistering overlord (listener)> Bbbbrrrraaaar!";
		for(Overlord i : overlords)
			i.detector(msg);
		overlords.remove(lstnr);
	}

}
