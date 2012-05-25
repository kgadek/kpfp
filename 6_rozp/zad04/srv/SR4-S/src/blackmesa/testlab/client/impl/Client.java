package blackmesa.testlab.client.impl;

import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

import blackmesa.credentials.TestLabCertificateVerifier;
import blackmesa.testlab.devices.AlreadyUsedException;
import blackmesa.testlab.devices.DevicePrx;
import blackmesa.testlab.devices.DevicePrxHelper;
import blackmesa.testlab.devices.ElementType;
import blackmesa.testlab.devices.Lambda;
import blackmesa.testlab.devices.WrongPasswordException;
import blackmesa.testlab.scientist.PushFetcherPrx;
import blackmesa.testlab.scientist.PushFetcherPrxHelper;
import blackmesa.testlab.server.VirtualLaboratoryPrx;
import blackmesa.testlab.server.VirtualLaboratoryPrxHelper;

public class Client {

	private static Ice.Communicator ic;

	private static VirtualLaboratoryPrx vl;

	private static Scanner sc = new Scanner(System.in);

	private static DevicePrx Device;

	private static String ADDRESS;

	private static String password;

	private static PushFetcherPrx listener;

	private static Set<String> observed = new HashSet<String>();

	private static Set<String> used = new HashSet<String>();

	public static void main(String[] args) throws WrongPasswordException {
		int status = 0;

		if (args.length != 3) {
			System.err.println("Server --Ice.Config=[server configuration] [IP] [port]");
			System.exit(1);
		}

		ADDRESS = ":ssl -h " + args[1] + " -p " + args[2];

		ic = null;
		
		ic = Ice.Util.initialize(args);

		Ice.PluginManager pluginMgr = ic.getPluginManager();
		Ice.Plugin plugin = pluginMgr.getPlugin("IceSSL");
		IceSSL.Plugin sslPlugin = (IceSSL.Plugin) plugin;
		sslPlugin.setCertificateVerifier(new TestLabCertificateVerifier());

		Ice.ObjectPrx base = ic.propertyToProxy("TestLab.Proxy");

		vl = VirtualLaboratoryPrxHelper.checkedCast(base);
		if (vl == null)
			throw new Error("Invalid proxy");

		passwordUI();

		if (ic != null)
			try {
				ic.destroy();
			} catch (Exception e) {
				e.printStackTrace();
			}
		System.exit(status);
	}

	private static void passwordUI() throws WrongPasswordException {
		System.out.println(">Enter password:");
		password = sc.next();
		startUI();
	}

	private static void startUI() throws WrongPasswordException {
		System.out.println(">0 - end");
		System.out.println(">1 - print Device list");
		System.out.println(">2 - get Device");
		System.out.println(">What do you want to do?");
		int d = sc.nextInt();
		if (d == 0) {
			clean();
			System.exit(0);
		} else if (d == 1) {
			printDeviceList();
			startUI();
		} else if (d == 2) {
			getDevice();
			startUI();
		}
	}

	private static void getDevice() throws WrongPasswordException {
		System.out.println(">Insert Device name:");
		String name = sc.next();
		Ice.ObjectPrx base = ic.stringToProxy(name + ADDRESS);
		Device = DevicePrxHelper.checkedCast(base);
		DeviceUse();
	}

	private static void clean() throws WrongPasswordException {
		for (String name : used) {
			Ice.ObjectPrx base = ic.stringToProxy(name + ADDRESS);
			Device = DevicePrxHelper.checkedCast(base);
			try {
				Device.leaveAndFree(password);
			} catch (WrongPasswordException e) {
				e.printStackTrace();
			}
		}
		for (String name : observed) {
			Ice.ObjectPrx base = ic.stringToProxy(name + ADDRESS);
			Device = DevicePrxHelper.checkedCast(base);
			Device.stopObserving(password);

		}
	}

	private static void DeviceUse() throws WrongPasswordException {
		System.out.println(">0 - back");
		System.out.println(">1 - start using");
		System.out.println(">2 - stop using");
		System.out.println(">3 - start observing");
		System.out.println(">4 - stop observing");
		System.out.println(">5 - invoke method");
		System.out.println(">What do you want to do?");
		int d = sc.nextInt();
		if (d == 0)
			startUI();
		else if (d == 1) {
			try {
				Device.takeAndUse(password);
			} catch (AlreadyUsedException e) {
				e.printStackTrace();
			}
			used.add(Device.getID());
			DeviceUse();
		} else if (d == 2) {
			try {
				Device.leaveAndFree(password);
			} catch (WrongPasswordException e) {
				e.printStackTrace();
			}
			used.remove(Device.getID());
			DeviceUse();
		} else if (d == 3) {
			Device.startObserving(password, getListener());
			observed.add(Device.getID());
			DeviceUse();
		} else if (d == 4) {
			Device.stopObserving(password);
			observed.remove(Device.getID());
			DeviceUse();
		} else if (d == 5)
			invokeMethod();
	}

	private static void invokeMethod() throws WrongPasswordException {
		System.out.println(">0 - back");
		Lambda[] mdList = Device.getMethods();
		for (int i = 0; i < mdList.length; i++) {
			System.out.print(">" + (i + 1) + " - " + mdList[i].name);
			int j = 1;
			System.out.print("( ");
			for (ElementType et : mdList[i].parameters) {
				System.out.print(et);
				if (j < mdList[i].parameters.length)
					System.out.print(", ");
				j++;
			}
			System.out.println(" )");
		}
		System.out.println(">What do you want to do?");
		int d = sc.nextInt();
		if (d == 0)
			DeviceUse();
		else if (d > 0 && d <= mdList.length)
			methodUtil(mdList[d - 1]);
	}

	private static void methodUtil(Lambda md) throws WrongPasswordException {
		Ice.OutputStream out = Ice.Util.createOutputStream(ic);
		out.writeString(password);
		for (ElementType et : md.parameters) {
			if (et == ElementType.IntegerType) {
				System.out.println(">Insert integer:");
				out.writeInt(sc.nextInt());
			} else if (et == ElementType.FloatType) {
				System.out.println(">Insert float:");
				out.writeFloat(sc.nextFloat());
			} else if (et == ElementType.BooleanType) {
				System.out.println(">Insert boolean:");
				out.writeBool(sc.nextBoolean());
			} else if (et == ElementType.StringType) {
				System.out.println(">Insert string:");
				out.writeString(sc.next());
			}
		}
		byte[] inParams = out.finished();
		Ice.ByteSeqHolder outParams = new Ice.ByteSeqHolder();
		if (Device.ice_invoke(md.name, Ice.OperationMode.Normal, inParams, outParams)) {
			Ice.InputStream in = Ice.Util.createInputStream(ic, outParams.value);
			ElementType et = md._return;
			if (et == ElementType.IntegerType) {
				System.out.println(">Returned integer: " + in.readInt());
			} else if (et == ElementType.FloatType) {
				System.out.println(">Returned float: " + in.readFloat());
			} else if (et == ElementType.BooleanType) {
				System.out.println(">Returned boolean: " + in.readBool());
			} else if (et == ElementType.StringType) {
				System.out.println(">Returned string: " + in.readString());
			}
		} else {
			Ice.InputStream in = Ice.Util.createInputStream(ic, outParams.value);
			try {
				in.throwException();
			} catch (Ice.UserException ex) {
				ex.printStackTrace();
			}
		}
		invokeMethod();
	}

	public static void printDeviceList() {
		for (String s : vl.getDeviceList()) {
			System.out.println(s);
		}
	}

	private static PushFetcherPrx getListener() {
		if (listener == null) {

			Ice.ObjectAdapter adapter = ic.createObjectAdapter("PushFetcher");
			adapter.add(new PushFetcherI(), ic.stringToIdentity("PushFetcher"));
			adapter.activate();

			Ice.Connection connection = vl.ice_getConnection();
			connection.setAdapter(adapter);

			listener = PushFetcherPrxHelper.checkedCast(adapter.createProxy(ic.stringToIdentity("PushFetcher")));

		}
		return listener;

	}
}
