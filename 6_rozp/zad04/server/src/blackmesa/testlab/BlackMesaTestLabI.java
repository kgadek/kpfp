package blackmesa.testlab;

import java.util.ArrayList;
import java.util.List;

import Ice.Current;

public class BlackMesaTestLabI extends _BlackMesaTestLabDisp {
	
	private static final long serialVersionUID = -96075141793624732L;
	private List<DevicesAvailable> devs = new ArrayList<DevicesAvailable>();

	public BlackMesaTestLabI() {
		super();
		
		try {
			String spectometers[] = {"antimass_spectometer"};
			DevicesAvailable dev1 = new DevicesAvailable("analyzer", spectometers);
			devs.add(dev1);
			
			String probes[] = {"xengg_3883"};
			DevicesAvailable dev2 = new DevicesAvailable("probe", probes);
			devs.add(dev2);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public DevicesAvailable list(Current __current) {
		return null;
	}

}
