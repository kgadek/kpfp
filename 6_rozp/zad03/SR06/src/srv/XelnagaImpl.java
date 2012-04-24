package srv;

import java.util.ArrayList;
import java.util.List;

import org.omg.PortableServer.POA;
import org.omg.PortableServer.Servant;
import org.omg.PortableServer.POAPackage.ServantAlreadyActive;
import org.omg.PortableServer.POAPackage.ServantNotActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;

import lab.Drone;
import lab.DroneHelper;
import lab.Overlord;
import lab.XelnagaPOA;

public class XelnagaImpl extends XelnagaPOA {

	private int xid;
	private List<Overlord> overlords = new ArrayList<Overlord>();
	private List<Drone> drones = new ArrayList<Drone>();
	//private POA rootpoa;
	private static int currDroneId = 0;
	
	public XelnagaImpl(POA rootpoa) throws ServantAlreadyActive, WrongPolicy, ServantNotActive, InstantiationException, IllegalAccessException {
		//this.rootpoa = rootpoa;
		
		Servant drone01instance = new MutaliskImpl();
		rootpoa.activate_object(drone01instance);
		Drone drone01 = DroneHelper.narrow(rootpoa.servant_to_reference(drone01instance));
		drone01.id(currDroneId++);
		drone01.name("Ed");
		
		drones.add(drone01);
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
		String msg = "<drone in reaction to " + id + " registering new overlord (listener)> Blumbbbrrrr!";
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

}
