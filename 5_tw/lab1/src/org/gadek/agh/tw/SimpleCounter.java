package org.gadek.agh.tw;

public class SimpleCounter extends Thread {

	private Counter toOperate;
	private int direction;	
	
	public SimpleCounter(Counter simpleCounter, int direction) {
		super();
		this.toOperate = simpleCounter;
		this.direction = direction;
	}

	@Override
	public void run() {
		for(int i = 0; i<100000000; i = i+1)
			toOperate.add(direction);
	}

}
