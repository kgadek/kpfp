package srv;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;

import lab.Drone;
import lab.Mutalisk;
import lab.MutaliskHelper;
import lab.Overlord;
import lab.XelnagaPOA;

import org.omg.CosNaming.NamingContextExt;
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
	//private POA rootpoa;
	private static int currDroneId = 0;
	
	public XelnagaImpl(POA rootpoa) throws ServantAlreadyActive, WrongPolicy, ServantNotActive, InstantiationException, IllegalAccessException {
		try {
//			DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
//			DocumentBuilder docBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(new File("../config.xml"));
			// a w Clojure to wyżej wygląda tak:
			// 		(. DocumentBuilderFactory NewInstance newDocumentBuilder (parse (File. "config.xml")))
			doc.getDocumentElement().normalize();
			NodeList listOfDrones = doc.getElementsByTagName("drones");
			for(int idx = 0; idx < listOfDrones.getLength(); ++idx) { // nie ma iteratora? a to smut ;<
				Node droneNode = listOfDrones.item(idx);
				if(droneNode.getNodeType() != Node.ELEMENT_NODE)
					continue;
				
				Element firstDroneE = (Element)droneNode;
				
				String type =
						((Node)
								((Element) firstDroneE.getElementsByTagName("type").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
				
				String name =
						((Node)
								((Element) firstDroneE.getElementsByTagName("name").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
				
				String idS =
						((Node)
								((Element) firstDroneE.getElementsByTagName("id").item(0))
							.getChildNodes().item(0)).getNodeValue().trim();
				int id = Integer.parseInt(idS);
			
				Drone curr = null;
				try {
					if(type.equals("Mutalisk")) { // ohhh, dirty and dirty but works & is quicker to write
						Servant currI = new MutaliskImpl();
						rootpoa.activate_object(currI);
						curr = MutaliskHelper.narrow(rootpoa.servant_to_reference(currI));
					} else continue;
					
					curr.id(id);
					curr.name(name);
					System.out.format("==LOG: adding type=%s id=%d name=%s\n", type, id, name);
					drones.add(curr);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		
		/*Servant drone01instance = new MutaliskImpl();
		rootpoa.activate_object(drone01instance);
		Mutalisk drone01 = MutaliskHelper.narrow(rootpoa.servant_to_reference(drone01instance));
		drone01.id(currDroneId++);
		drone01.name("Ed");
		drones.add(drone01);*/
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
	public void controlDrone(int id, Overlord lstnr) {
		String msg = "<drone " + id + " in reaction to registering new overlord (listener)> Blumbbbrrrr!";
		System.out.println("LOG: " + msg);
		System.out.println("LOG: listener = " + lstnr);
		if(lstnr == null) {
			System.err.println("LOG: Oh wait... It's null ;<");
			return;
		}
			
		for(Overlord i : overlords)
			i.detector(msg);
		for(Drone i : drones)
			if(i.id() == id)
				i.add_overmind(lstnr);
	}

	@Override
	public void abandonDrone(int id, Overlord lstnr) {
		String msg = "<drone in reaction to " + id + " deregistering overlord (listener)> Bbbbrrrraaaar!";
		for(Overlord i : overlords)
			i.detector(msg);
		overlords.remove(lstnr);
	}

	public void setNcRef(NamingContextExt ncRef) {
	}

}
