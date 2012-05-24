package blackmesa.testlab;

import java.util.ArrayList;
import java.util.List;

import Ice.Current;

public class BlackMesaTestLabI extends _BlackMesaTestLabDisp {
	
	private static final long serialVersionUID = -96075141793624732L;
	private List<DeviceAvailable> devs = new ArrayList<DeviceAvailable>();

	public BlackMesaTestLabI() {
		try {
			String spectometers[] = {"antimass_spectometer"};
			DeviceAvailable dev1 = new DeviceAvailable("analyzer", spectometers);
			devs.add(dev1);
			
			String probes[] = {"xengg_3883"};
			DeviceAvailable dev2 = new DeviceAvailable("probe", probes);
			devs.add(dev2);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public DeviceAvailable[] list(Current __current) {
		return devs.toArray(new DeviceAvailable[0]);
	}

}
