package srv;

import lab.Xelnaga;
import lab.XelnagaHelper;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

public class XelnagaServer {
	public static void main(String[] args) {
		try {
			// things 'n stuff from oracle tutorial
			ORB orb = ORB.init(args, null);
			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			rootpoa.the_POAManager().activate();
			
			XelnagaImpl xelnagaImpl = new XelnagaImpl(rootpoa);
			org.omg.CORBA.Object ref = rootpoa.servant_to_reference(xelnagaImpl);
			Xelnaga href = XelnagaHelper.narrow(ref);
			
			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			
			NameComponent path[] = ncRef.to_name("Xelnaga");
			ncRef.rebind(path, href);
			
			System.out.println("CORBA initialized on Zerus, yay!");
			
			orb.run();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
