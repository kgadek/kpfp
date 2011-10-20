package org.gadek.agh.tw;

public class SynchronizedCounter extends Thread {

	private Counter toOperate;
	private int direction;
	
	public SynchronizedCounter(Counter toOperate, int direction) {
		super();
		this.toOperate = toOperate;
		this.direction = direction;
	}

	@Override
	public void run() {
		for(int i = 0; i<10000000; i = i+1)
			synchronized(toOperate) {
				toOperate.add(direction);
			}
	}
	
}
