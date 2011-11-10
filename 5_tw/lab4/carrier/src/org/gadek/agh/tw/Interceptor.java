package org.gadek.agh.tw;

import java.util.Random;

public class Interceptor extends Thread {
	
	private final static int interceptionTime = 80;
	private final static int landingTime = 30;
	private final static int startingTime = 10;
	private final static int regeneratingTime = 50;
	
	private int id;
	private Carrier beacon; 
	private static final Random random = new Random();

	@Override
	public void run() {
		try {
			while(true) {
				intercept();
				beacon.wantRechargeShields();
				land();
				beacon.iHaveReturned();
				regeneratingShields();
				beacon.wantDieForAiur();
				takeOff();
				beacon.iHaveTakenOff();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void takeOff() throws InterruptedException {
		System.out.println("" + id + ": I have taken off. " + stats()); 
		Thread.sleep(random.nextInt(Interceptor.startingTime));
	}

	private void regeneratingShields() throws InterruptedException {
		System.out.println("" + id + ": My shields are regenerating. " + stats());
		Thread.sleep(random.nextInt(Interceptor.regeneratingTime));
	}

	private void land() throws InterruptedException {
		System.out.println("" + id + ": I have returned. " + stats());
		Thread.sleep(random.nextInt(Interceptor.landingTime));
	}
	
	private void intercept() throws InterruptedException {
		System.out.println("" + id + ": My life for Aiur. " + stats());
		Thread.sleep(random.nextInt(Interceptor.interceptionTime));
	}
	
	private String stats() {
		return "" + beacon.getWantDieForAiur() + " of " + beacon.getOnBoard() + " want to start, "
				+ beacon.getWantRechargeShields() + " of " + (beacon.getInterceptorsM() - beacon.getOnBoard()) + " want to return, " +
				"runway is " + (beacon.isFree() ? "free":"occupied") + ".";
	}

	public Interceptor(int id, Carrier beacon) {
		super();
		this.id = id;
		this.beacon = beacon;
	}
}
