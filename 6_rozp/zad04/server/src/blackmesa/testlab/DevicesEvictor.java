package blackmesa.testlab;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import Ice.Current;
import Ice.LocalObjectHolder;
import Ice.Object;
import Ice.ServantLocator;
import Ice.UserException;
import blackmesa.devices.Device;
import blackmesa.devices.DeviceInfo;

public class DevicesEvictor implements ServantLocator {

	private static final String DEVICE_PACKAGE = "blackmesa.devices";
	private static final int RELEASED_DEVICES_MAX_LEN = 2;
	
	private Map<String, Class<?>> devices = new HashMap<String, Class<?>>(); 
	private Map<String, Device> initializedDevices = new HashMap<String, Device>(); 
	private List<Device> releasedDevices = new ArrayList<Device>(RELEASED_DEVICES_MAX_LEN+1);
	
	@Override
	public synchronized Object locate(Current curr, LocalObjectHolder cookie) throws UserException {
		String currname = null;
		if(curr == null || curr.id==null || devices.containsKey(currname = curr.id.name))
			return null; // throw exception?
		if(initializedDevices.containsKey(currname))
			return initializedDevices.get(currname);
		Device newDev = null;
		try {
			newDev = (Device) devices.get(currname).newInstance();
			((DeviceInfo)newDev).setName(currname);
			initializedDevices.put(currname, newDev);
		} catch (InstantiationException | IllegalAccessException e) {
			e.printStackTrace();
		} 
		return newDev;
	}

	public DevicesEvictor(DeviceAvailable[] allDevs) {
		Class<?> device = null;
		for(DeviceAvailable i : allDevs)
			try {
				device = Class.forName(DEVICE_PACKAGE + "." + i.type + "I");
				for(String j : i.machines)
					devices.put(j, device);
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
	}

	@Override
	public synchronized void finished(Current curr, Object servant, java.lang.Object cookie) throws UserException {
		if(! curr.operation.equals("release") && ! curr.operation.equals("ignore"))
			releasedDevices.remove(initializedDevices.get(curr.id.name));
		else if(((DeviceInfo)servant).toBeReleased()) {
			releasedDevices.add((Device)servant);
			while(releasedDevices.size() > RELEASED_DEVICES_MAX_LEN) // in a case Java got silly again...
				initializedDevices.remove(((DeviceInfo)releasedDevices.remove(0)).name());
		}
	}

	@Override
	public synchronized void deactivate(String category) { }
}
