package blackmesa.testlab;

public class TestLab {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Ice.Communicator ic = null;
        try {
            ic = Ice.Util.initialize(args);
            Ice.ObjectAdapter adapter = ic.createObjectAdapter("TestLabAdapter");
            
            Ice.Object blackMesaTestLab = new BlackMesaTestLabI();
            DevicesAvailable eviesFood = ((BlackMesaTestLab)blackMesaTestLab).list(null);
            Ice.ServantLocator evie = new DevicesEvictor(eviesFood); // my little evictor!
            
            adapter.add(blackMesaTestLab, ic.stringToIdentity("TestLab"));
            adapter.addServantLocator(evie, "");
            adapter.activate();
            
            System.out.format("System is up and running...");
            
            ic.waitForShutdown();
        } catch (Exception e) {
            System.err.println(e.getMessage());
        } finally {
	        if (ic != null) {
	            try {
	                ic.destroy();
	            } catch (Exception e) {
	                System.err.println(e.getMessage());
	            }
	        }
        }
	}

}
