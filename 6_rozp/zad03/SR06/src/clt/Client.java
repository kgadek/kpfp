package clt;

import java.text.DateFormat;
import java.util.Date;
import java.util.Random;

import lab.Drone;
import lab.Mutalisk;
import lab.MutaliskHelper;
import lab.Overlord;
import lab.OverlordHelper;
import lab.Xelnaga;
import lab.XelnagaHelper;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

public class Client {
	private Xelnaga someRandomHiveOnSomePlanet;
	private String myNameIs = "Client"+(new Random()).nextInt();
	private Drone controlled = null;
	private int ticket = -1;
	
	private void run(String[] args) {
		try {
			// things 'n stuff from oracle tutorial
			ORB orb = ORB.init(args, null);
			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			rootpoa.the_POAManager().activate();
			
			OverlordImpl overlordImpl = new OverlordImpl("for:"+myNameIs);
			org.omg.CORBA.Object ref = rootpoa.servant_to_reference(overlordImpl);
			Overlord href = OverlordHelper.narrow(ref);
			
//			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			org.omg.CORBA.Object objRef = orb.string_to_object("IOR:010000002b00000049444c3a6f6d672e6f72672f436f734e616d696e672f4e616d696e67436f6e746578744578743a312e300000010000000000000074000000010102000f0000003137322e31372e3132382e3232340000c05a00000b0000004e616d6553657276696365000300000000000000080000000100000000545441010000001c000000010000000100010001000000010001050901010001000000090101000354544108000000e02a8c4f010004e4");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			
			NameComponent path[] = ncRef.to_name(myNameIs);
			ncRef.rebind(path, href);
			
			someRandomHiveOnSomePlanet = XelnagaHelper.narrow(ncRef.resolve_str("Xelnaga"));
			
			System.out.println("Drones:");
			for(Drone pi : someRandomHiveOnSomePlanet.searchSpaceAndDestroyProtossRace()) {
				Mutalisk i = MutaliskHelper.narrow(pi);
				if(controlled == null)
					if((ticket = i.getTicket()) > 0) {
						controlled=i;
						System.out.format("==LOG: Observing drone below:_____________\n");
					}
				System.out.println("\tAvailable drone: #" + i.id() + " name:" + i.name() + " ticket: " + ticket);
				someRandomHiveOnSomePlanet.observeDrone(i.id(), OverlordHelper.narrow(ncRef.resolve_str(myNameIs)));
			}
			
			Drone ed = someRandomHiveOnSomePlanet.searchDroneByName("Ed");
			System.out.println("Ed is a mutalisk? " + ed._is_a("Mutalisk"));
			Mutalisk m_ed = MutaliskHelper.narrow(ed);
			System.out.println("Ed?\n >>> " + m_ed);
			
			//someRandomHiveOnSomePlanet.observeDrone(ed.id(), OverlordHelper.narrow(ncRef.resolve_str(myNameIs)));
			
			DateFormat jo = DateFormat.getTimeInstance(DateFormat.FULL);
			while(true) {
				System.out.println("Currently is " + jo.format(new Date()));
				((Mutalisk)MutaliskHelper.narrow(controlled)).disableWPA(ticket);
				((Mutalisk)MutaliskHelper.narrow(controlled)).command(ticket, "Controlled command");
				Thread.sleep(5000);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		Client a = new Client();
		a.run(args);
	}
}
