import java.rmi.Remote;
import java.rmi.RemoteException;

class Ble {
	private String jojo;

	public Ble(String jojo) {
		super();
		this.jojo = jojo;
	}

	public String getJojo() {
		return jojo;
	} 
}

public interface Compute extends Remote {
	<T> T executeTask(Task<T> t) throws RemoteException;
	Ble jol();

}
