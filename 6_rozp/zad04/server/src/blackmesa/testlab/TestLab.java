package blackmesa.testlab;

import IceSSL.Plugin;

public class TestLab {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Ice.Communicator ic = null;
        try {
            ic = Ice.Util.initialize(args);
            
            Ice.PluginManager pluginMgr = ic.getPluginManager();
            IceSSL.Plugin pluginSSL = (Plugin) pluginMgr.getPlugin("IceSSL");
            pluginSSL.setCertificateVerifier(new TestLabCredentialsVerifier()); // as simple as that ;P
            
            Ice.ObjectAdapter adapter = ic.createObjectAdapter("TestLabAdapter");
            
            Ice.Object blackMesaTestLab = new BlackMesaTestLabI();
            DeviceAvailable[] eviesFood = ((BlackMesaTestLab)blackMesaTestLab).list(null);
            Ice.ServantLocator evie = new DevicesEvictor(eviesFood); // my little Evictor!
            
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
