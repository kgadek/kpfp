import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;


public class ComputeEngine implements Compute {

	public ComputeEngine() {
		super();
	}
	
	@Override
	public <T> T executeTask(Task<T> t) throws RemoteException {
		return t.execute();
	}
	
	public static void main(String args[]) {
		if(System.getSecurityManager() == null)
			System.setSecurityManager(new SecurityManager());
		
		try {
			String name = "Compute";
			Compute engine = new ComputeEngine();
			Compute stub = (Compute) UnicastRemoteObject.exportObject(engine, 0);
			Registry registry = LocateRegistry.getRegistry();
			registry.rebind(name, stub);
			System.out.println("ComputeEngine bound");
		} catch (RemoteException e) {
			System.err.println("ComputeEngine failed:" + e.getMessage());
			e.printStackTrace();
		}
	}

}
