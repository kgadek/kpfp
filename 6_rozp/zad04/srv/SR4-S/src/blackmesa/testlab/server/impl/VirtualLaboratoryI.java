package blackmesa.testlab.server.impl;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import Ice.Current;
import blackmesa.testlab.exception.ConfigurationParsingException;
import blackmesa.testlab.server._VirtualLaboratoryDisp;

public class VirtualLaboratoryI extends _VirtualLaboratoryDisp {

	private static final long serialVersionUID = -3297343832494155447L;

	private static final String Device_PACKAGE = "virtuallab.Devices.impl.";

	private Map<String, Class<?>> availableDevices = new HashMap<String, Class<?>>();
	
	private String[] DeviceList;

	public VirtualLaboratoryI(String conf) throws ConfigurationParsingException {
		File file = new File(conf);
		Scanner scanner = null;
		try {
			scanner = new Scanner(file);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		ArrayList<String> tempDeviceList = new ArrayList<String>();
		while (scanner.hasNext()) {
			StringBuilder DeviceData = new StringBuilder("");
			
			String DeviceClassName = scanner.next();
			Class<?> DeviceClass;
			try {
				DeviceClass = Class.forName(Device_PACKAGE + DeviceClassName + "I");
				
				DeviceData.append("[Class: ");
				DeviceData.append(DeviceClassName);
				DeviceData.append("]");
				
				int namesNumber = Integer.parseInt(scanner.next());
				for (int i = 0; i < namesNumber; i++) {
					String name = scanner.next();
					if (!availableDevices.containsKey(name)) {
						availableDevices.put(name, DeviceClass);
						
						DeviceData.append(" ");
						DeviceData.append(name);
					} else {
						throw new ConfigurationParsingException();
					}
				}
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
			tempDeviceList.add(DeviceData.toString());
			DeviceList = new String[tempDeviceList.size()];
			for(int i=0; i<tempDeviceList.size(); i++) {
				DeviceList[i] = tempDeviceList.get(i);
			}
		}
	}

	@Override
	public String[] getDeviceList(Current __current) {
		return DeviceList;
	}
	
	public Class<?> getDeviceClassForName(String name) {
		return availableDevices.get(name);
	}

}
