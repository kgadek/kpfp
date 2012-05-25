package blackmesa.testlab.server.impl;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

import Ice.Current;
import Ice.LocalObjectHolder;
import Ice.Object;
import Ice.ServantLocator;
import Ice.UserException;

public class VirtualLabEvictor implements ServantLocator {

	private VirtualLaboratoryI virtualLab;

	private int servantPopulationSize;

	private Map<String, Object> servantPool = new HashMap<String, Object>();

	private Queue<Object> readyForEviction = new LinkedList<Object>();

	public VirtualLabEvictor(VirtualLaboratoryI vl, int populationSize) {
		virtualLab = vl;
		servantPopulationSize = populationSize;
		System.out.println("### Evictor ready!");
	}

	@Override
	public synchronized Object locate(Current curr, LocalObjectHolder cookie) throws UserException {
		String DeviceName = curr.id.name;
		Object Device = servantPool.get(DeviceName);
		if (Device == null) {
			while (servantPool.size() >= servantPopulationSize && readyForEviction.size() > 0) {
				tryEviction();
			}
			try {
				Device = (Object) virtualLab.getDeviceClassForName(DeviceName).newInstance();
				((ITestLabServant) Device).setID(DeviceName);
				servantPool.put(DeviceName, Device);
				System.out.println("### Curent pool size: " + servantPool.size() + " !");
				if (!curr.operation.equals("takeAndUse") && !curr.operation.equals("startObserving")) {
					readyForEviction.add(Device);
				}
			} catch (InstantiationException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			}
		}
		return Device;
	}

	private void tryEviction() {
		Object evicted = readyForEviction.poll();
		if (evicted != null) {
			servantPool.remove(((ITestLabServant) evicted).getID());
			System.out.println("### Eviction successful!");
		}
	}

	@Override
	synchronized public void finished(Current curr, Object servant, java.lang.Object cookie) throws UserException {
		if (curr.operation.equals("takeAndUse") || curr.operation.equals("startObserving")) {
			if (!((ITestLabServant) servant).isBeingTracked()) {
				readyForEviction.add(servant);
			} else if (readyForEviction.contains(servant)) {
				readyForEviction.remove(servant);
			}
		} else if (curr.operation.equals("leaveAndFree") || curr.operation.equals("stopObserving")) {
			if (!((ITestLabServant) servant).isBeingTracked()) {
				readyForEviction.add(servant);
			}
		}
	}

	@Override
	synchronized public void deactivate(String category) {
		System.out.println("### Evictor deactivating!");
	}

}
