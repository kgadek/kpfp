package blackmesa.scientist;

import java.util.Scanner;

import Ice.ByteSeqHolder;
import IceSSL.CertificateVerifier;
import IceSSL.NativeConnectionInfo;
import blackmesa.devices.DevicePrx;
import blackmesa.devices.DevicePrxHelper;
import blackmesa.devices.Lambda;
import blackmesa.devices.Type;
import blackmesa.testlab.BlackMesaTestLabPrx;
import blackmesa.testlab.BlackMesaTestLabPrxHelper;
import blackmesa.testlab.DeviceAvailable;

public class Scientist {
	
	static final String address = ":ssl -h 127.0.0.1 -p 123456";
	private Ice.Communicator ice;
	private Scanner sc = new Scanner(System.in);
	private SpectatorPrx spectator;
	
	public BlackMesaTestLabPrx proxy;

	public static void main(String[] args) {
		Ice.Communicator ic = null;
		Scientist c = new Scientist();
		try {
			ic = Ice.Util.initialize(args);
			BlackMesaTestLabPrx proxy = BlackMesaTestLabPrxHelper.checkedCast(ic.propertyToProxy("TestLab.Proxy"));
			
			c.ice = ic;
			c.proxy = proxy;
			c.chooseClass(args[0]);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if(ic != null)
			try {
				ic.destroy();
			} catch (Exception e) {
				e.printStackTrace();
			}
	}

	private void chooseClass(String login) {
		while(true) {
			for(DeviceAvailable i : proxy.list()) {
				System.out.println("" + i.type + ":");
				for(String j : i.machines)
					System.out.println("\t"+ j);
				System.out.println("----------");
			}
			System.out.println("Wybierz jedną:");
			secondaryLoop(login, sc.next());
		}
	}

	private void secondaryLoop(String login, String name) {
		Ice.ObjectPrx proxy = ice.stringToProxy(name+address);
		DevicePrx device = DevicePrxHelper.checkedCast(proxy);
		System.out.println("Do wyboru:");
		int i = 0;
		for(Lambda l : device.functions()) {
			System.out.format("%2d:\t\t%s/%d -> %s.\n", i++, l.name, l.params.length, l._return);
			for(Type t : l.params)
				System.out.format("  :\t\t\t %s\n", typeToName(t));
		}
		System.out.format("%2d:\t\twyjście\n",i);
		int ch = i+1;
		while(ch > i || ch < 0)
			ch = sc.nextInt();
		if(ch == i)
			return;
		Lambda fun = device.functions()[ch];
		if(fun.name.equals("takeover") || fun.name.equals("release") ||
				fun.name.equals("observe") || fun.name.equals("ignore"))
			try {
				switch(ch) {
					case 0: device.takeover(login);
						break;
					case 1: device.release(login);
						return;
					case 2: device.observe(login, getObserver());
						break;
					case 3: device.ignore(login, getObserver());
						break;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		else
			try {
				Ice.OutputStream iout = Ice.Util.createOutputStream(ice);
				iout.writeString(login);
				for(Type t : fun.params) {
					System.out.format("Podaj argument (%s): ", typeToName(t));
					switch(t) {
						case TFloat: iout.writeFloat(sc.nextFloat());
							break;
						case TInteger: iout.writeInt(sc.nextInt());
							break;
						case TNone:
						default:
							break;
					}
				}
				byte[] params = iout.finished();
				Ice.ByteSeqHolder outParams = new ByteSeqHolder();
				if(device.ice_invoke(fun.name, Ice.OperationMode.Normal, params, outParams)) {
					Ice.InputStream iin = Ice.Util.createInputStream(ice, outParams.value);
					switch(fun._return) {
						case TFloat:
							System.out.format("returned: %.3f\n", iin.readFloat());
							break;
						case TInteger:
							System.out.format("returned: %d\n", iin.readInt());
							break;
						case TNone:
						default:
							System.out.format("returned: ---\n");
							break;
					}
				} else
					try {
						Ice.InputStream iin = Ice.Util.createInputStream(ice, outParams.value);
						iin.throwException();
					} catch (Exception e) {
						e.printStackTrace();
					}
			} catch (Exception e) {
				e.printStackTrace();
			}
	}

	private SpectatorPrx getObserver() {
		if(spectator == null) {
			Ice.ObjectAdapter adapter = ice.createObjectAdapter("ObserverAdapter");
			adapter.add(new SpectatorI(), ice.stringToIdentity("spect"));
			adapter.activate();
			
			Ice.Connection connection = proxy.ice_getConnection();
			connection.setAdapter(adapter);
			
			spectator = SpectatorPrxHelper.checkedCast(adapter.createProxy(ice.stringToIdentity("spect")));
		}
		return spectator;
	}

	private String typeToName(Type t) {
		switch(t) {
			case TFloat: return "float";
			case TInteger: return "int";
			case TNone: return "void?";
			default: return "?";
		}
	}
}
