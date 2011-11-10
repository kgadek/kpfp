package org.gadek.agh.tw;

import java.util.concurrent.Semaphore;

public class Carrier {

	private int interceptorsK; /* liczba graniczna */
	private int interceptorsM; /* iloœæ samolotów */
	private int carrierCapacity; /* pojemnoœæ pok³adu */
	private int onBoard; /* na pok³adzie */
	
	private int wantDieForAiur; /* chce wystartowaæ */
	private int wantRechargeShields; /* chce l¹dowaæ */
	private boolean free; /* pas jest wolny */
	Semaphore carrierCommanderAccess;
	Semaphore takingOffTicket;
	Semaphore landingTicket;
	
	Interceptor[] interceptors;
	
	public static void main(String[] args) {
		Carrier protossCarrier = new Carrier(32, 8, 16, 8);
		protossCarrier.operate();
	}

	private void operate() {
		try {
			for(int i=0; i<interceptorsM; ++i) interceptors[i].start();
			for(int i=0; i<interceptorsM; ++i) interceptors[i].join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public Carrier(int interceptorsM, int carrierCapacity, int interceptorsK, int onBoard) {
		assert(onBoard <= carrierCapacity);
		this.interceptorsK = interceptorsK;
		this.interceptorsM = interceptorsM;
		this.carrierCapacity = carrierCapacity;
		
		this.onBoard = 0;
		this.wantDieForAiur = 0;
		this.wantRechargeShields = 0;
		this.free = true;
		this.carrierCommanderAccess = new Semaphore(1);
		try {
			this.takingOffTicket = new Semaphore(1);
			this.takingOffTicket.acquire();
			this.landingTicket = new Semaphore(1);
			this.landingTicket.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		this.interceptors = new Interceptor[interceptorsM];
		for(int i = 0; i < interceptorsM; ++i) this.interceptors[i] = new Interceptor(i, this);
	}

	private void zwolnijPas() {
		if(onBoard < interceptorsK) {
			if(wantRechargeShields > 0) {
				--wantRechargeShields;
				landingTicket.release();
			} else if(wantDieForAiur > 0) {
				--wantDieForAiur;
				takingOffTicket.release();
			} else
				free = true;
		} else {
			if(wantDieForAiur > 0) {
				--wantDieForAiur;
				takingOffTicket.release();
			} else if(wantRechargeShields > 0 && onBoard < carrierCapacity) {
				--wantRechargeShields;
				landingTicket.release();
			} else
				free = true;
		}
	}

	public void iHaveTakenOff() throws InterruptedException {
		carrierCommanderAccess.acquire();
		--onBoard;
		zwolnijPas();
		carrierCommanderAccess.release();
	}

	public void wantDieForAiur() throws InterruptedException {
		carrierCommanderAccess.acquire();
		if(free) {
			free = false;
			carrierCommanderAccess.release();
		} else {
			++wantDieForAiur;
			carrierCommanderAccess.release();
			takingOffTicket.acquire();
		}
	}

	public void iHaveReturned() throws InterruptedException {
		carrierCommanderAccess.acquire();
		++onBoard;
		zwolnijPas();
		carrierCommanderAccess.release();
	}

	public void wantRechargeShields() throws InterruptedException {
		carrierCommanderAccess.acquire();
		if(free && onBoard < carrierCapacity) {
			free = false;
			carrierCommanderAccess.release();
		} else {
			++wantRechargeShields;
			carrierCommanderAccess.release();
			landingTicket.acquire();
		}
	}

	public int getInterceptorsK() {
		return interceptorsK;
	}

	public int getInterceptorsM() {
		return interceptorsM;
	}

	public int getCarrierCapacity() {
		return carrierCapacity;
	}

	public int getOnBoard() {
		return onBoard;
	}

	public int getWantDieForAiur() {
		return wantDieForAiur;
	}

	public int getWantRechargeShields() {
		return wantRechargeShields;
	}

	public boolean isFree() {
		return free;
	}
}
