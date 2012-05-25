package blackmesa.testlab.Devices.impl;

import java.util.ArrayList;
import java.util.List;

import Ice.Current;
import blackmesa.testlab.devices.AlreadyUsedException;
import blackmesa.testlab.devices.ElementType;
import blackmesa.testlab.devices.Lambda;
import blackmesa.testlab.devices.WrongPasswordException;
import blackmesa.testlab.devices._SpiderDisp;
import blackmesa.testlab.scientist.PushFetcherPrx;
import blackmesa.testlab.server.impl.ITestLabServant;
import blackmesa.testlab.server.impl.OrdinarySimpleNotificationTool;
import blackmesa.testlab.server.impl.SimplePasswordChecker;

public class Spider extends _SpiderDisp implements ITestLabServant {

	private static final long serialVersionUID = 369208575889653232L;
	private SimplePasswordChecker passChk = new SimplePasswordChecker();
	private OrdinarySimpleNotificationTool note = new OrdinarySimpleNotificationTool();
	private String id;
	
	@Override
	public void move(String password, float direction, float speed,
			int movementType, Current __current) throws WrongPasswordException {
		if(!passChk.check(password)) throw new WrongPasswordException();
		System.out.format("Spider:move(%.3f, %.3f, %d)\n", direction, speed, movementType);
		note.processEvent("Spider:move");
	}

	@Override
	public void growl(String password, int type, Current __current)
			throws WrongPasswordException {
		if(!passChk.check(password)) throw new WrongPasswordException();
		System.out.format("Spider:growl(%d)\n", type);
		note.processEvent("Spider:growl");
	}

	@Override
	public void takeAndUse(String password, Current __current)
			throws AlreadyUsedException, WrongPasswordException {
		if(!passChk.setPassword(password))
			throw new AlreadyUsedException();
	}

	@Override
	public void leaveAndFree(String password, Current __current)
			throws WrongPasswordException {
		if(!passChk.resetPassword(password))
			throw new WrongPasswordException();
	}

	@Override
	public void startObserving(String password, PushFetcherPrx listener,
			Current __current) throws WrongPasswordException {
		note.startObserving(password, listener, __current);

	}

	@Override
	public void stopObserving(String password, Current __current)
			throws WrongPasswordException {
		note.stopObserving(password);
	}

	@Override
	public Lambda[] getMethods(Current __current) {
		List<Lambda> lambdas = new ArrayList<Lambda>();
		List<ElementType> types = new ArrayList<ElementType>();
		types.clear();
		types.add(ElementType.FloatType);
		types.add(ElementType.FloatType);
		types.add(ElementType.IntegerType);
		lambdas.add(new Lambda("move", ElementType.VoidType, (ElementType[]) types.toArray()));
		return (Lambda[]) lambdas.toArray();
	}

	@Override
	public String getID(Current __current) {
		return id;
	}

	@Override
	public void setID(String id) {
		this.id = id;
	}

	@Override
	public synchronized boolean isBeingTracked() {
		return !passChk.isEmpty() || note.isObserved();
	}

}
