package org.gadek.agh.tw;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class LockingCounter extends Thread {

	private Counter toOperate;
	private int direction;
	private static Lock loki = new ReentrantLock();
	
	public LockingCounter(Counter toOperate, int direction) {
		super();
		this.toOperate = toOperate;
		this.direction = direction;
	}

	@Override
	public void run() {
		for(int i = 0; i<1000000; i = i+1) {
			loki.lock();
			toOperate.add(direction);
			loki.unlock();
		}
	}

}
