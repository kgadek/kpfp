package blackmesa.testlab.server.impl;

import blackmesa.credentials.TestLabCertificateVerifier;
import blackmesa.testlab.exception.ConfigurationParsingException;

public class Server {
	public static void main(String args[]) {
		int status = 0;

		if (args.length != 2) {
			System.err.println("Server --Ice.Config=[server configuration] [laboratory configuration]");
			System.exit(1);
		}

		Ice.Communicator ic = null;
		ic = Ice.Util.initialize(args);
		
		Ice.PluginManager pluginMgr = ic.getPluginManager();
		Ice.Plugin plugin = pluginMgr.getPlugin("IceSSL");
		IceSSL.Plugin sslPlugin = (IceSSL.Plugin)plugin;
		sslPlugin.setCertificateVerifier(new TestLabCertificateVerifier());
		
		Ice.ObjectAdapter adapter = ic.createObjectAdapter("VirtualLabAdapter");

		try {
			Ice.Object vl = new VirtualLaboratoryI(args[1]);
			adapter.add(vl, ic.stringToIdentity("VirtualLab"));
			Ice.ServantLocator sl = new VirtualLabEvictor((VirtualLaboratoryI) vl, 2);
			adapter.activate();
			adapter.addServantLocator(sl, "");
			System.out.println("### Server ready!");
			ic.waitForShutdown();
		} catch (ConfigurationParsingException e) {
			e.printStackTrace();
			status = 1;
		}
		
		if (ic != null) {
			ic.destroy();
		}
		System.exit(status);
	}
}
