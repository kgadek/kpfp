package blackmesa.testlab.server.impl;

import java.util.HashMap;
import java.util.Map;

import Ice.Current;
import blackmesa.testlab.scientist.PushFetcherPrx;
import blackmesa.testlab.scientist.PushFetcherPrxHelper;

public class OrdinarySimpleNotificationTool {
	
	// ordinarily, this is also a note ;P
	private Map<String, PushFetcherPrx> listeners = new HashMap<String, PushFetcherPrx>();

	public void processEvent(String string) {
		System.out.println(string);
		for(PushFetcherPrx nl : listeners.values())
			nl.begin_notifyMe(string);
	}

	public void startObserving(String password, PushFetcherPrx listener, Current __current) {
		Ice.ObjectPrx obj = __current.con.createProxy(listener.ice_getIdentity());
		PushFetcherPrx listenerPrx = PushFetcherPrxHelper.uncheckedCast(obj);
		listeners.put(password, listenerPrx);
	}
	
	public void stopObserving(String password) {
		listeners.remove(password);
	}
	
	public boolean isObserved() {
		return !listeners.isEmpty();
	}

}
