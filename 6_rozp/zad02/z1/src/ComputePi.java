import java.math.BigDecimal;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;


public class ComputePi {

	public static void main(String[] args) {
		if(System.getSecurityManager() == null)
			System.setSecurityManager(new SecurityManager());
		
		try {
			String name = "Compute";
			Registry registry = LocateRegistry.getRegistry();
			Compute comp = (Compute)registry.lookup(name);
			Pi task = new Pi(5);
			BigDecimal pi = comp.executeTask(task);
			System.out.println("Pi =~ " + String.valueOf(pi));
			System.out.println(comp.jol());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
