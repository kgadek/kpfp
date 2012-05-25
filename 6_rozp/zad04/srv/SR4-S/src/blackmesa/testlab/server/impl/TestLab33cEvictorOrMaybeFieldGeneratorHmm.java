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

public class TestLab33cEvictorOrMaybeFieldGeneratorHmm implements ServantLocator {

	private TestLabC33a testLab;
	private int servPop;
	private Map<String, Object> servPool = new HashMap<String, Object>();
	private Queue<Object> readyForEviction = new LinkedList<Object>();

	public TestLab33cEvictorOrMaybeFieldGeneratorHmm(TestLabC33a vl, int populationSize) {
		testLab = vl;
		servPop = populationSize;
		System.out.println("### Evictor ready!");
	}

	@Override
	public synchronized Object locate(Current curr, LocalObjectHolder cookie) throws UserException {
		String DeviceName = curr.id.name;
		Object Device = servPool.get(DeviceName);
		if (Device == null) {
			while (servPool.size() >= servPop && readyForEviction.size() > 0) {
				tryEviction();
			}
			try {
				Device = (Object) testLab.getDeviceClassForName(DeviceName).newInstance();
				((ITestLabServant) Device).setID(DeviceName);
				servPool.put(DeviceName, Device);
				System.out.println("### Curent pool size: " + servPool.size() + " !");
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
			servPool.remove(((ITestLabServant) evicted).getID());
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
