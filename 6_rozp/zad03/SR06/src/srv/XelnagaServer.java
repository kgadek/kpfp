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
			
//			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			org.omg.CORBA.Object objRef = orb.string_to_object("IOR:010000002b00000049444c3a6f6d672e6f72672f436f734e616d696e672f4e616d696e67436f6e746578744578743a312e300000010000000000000074000000010102000f0000003137322e31372e3132382e3232340000c05a00000b0000004e616d6553657276696365000300000000000000080000000100000000545441010000001c000000010000000100010001000000010001050901010001000000090101000354544108000000e02a8c4f010004e4");
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
