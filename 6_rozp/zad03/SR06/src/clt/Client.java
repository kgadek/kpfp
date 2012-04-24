package clt;

import lab.Drone;
import lab.Overlord;
import lab.OverlordHelper;
import lab.Xelnaga;
import lab.XelnagaHelper;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

public class Client {
	private static Xelnaga someRandomHiveOnSomePlanet;

	public static void main(String[] args) {
		try {
			ORB orb = ORB.init(args, null);
			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			
			someRandomHiveOnSomePlanet = XelnagaHelper.narrow(ncRef.resolve_str("Xelnaga"));
			
			System.out.println("Drones:");
			for(Drone i : someRandomHiveOnSomePlanet.searchSpaceAndDestroyProtossRace())
				System.out.println("\tAvailable drone: #" + i.id() + " name:" + i.name());
			
			Drone ed = someRandomHiveOnSomePlanet.searchDroneByName("Ed");
			System.out.println("Ed?\n >>> " + ed);
			
			
			ed.command("Lool");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
