package org.gadek.agh.tw;

import java.util.Random;

public class RandomPrinter extends Thread {

	private static final int maxWaitTime = 5000;
	
	private String msg;
	private CountingSemaphore semaphore;
	
	public RandomPrinter(CountingSemaphore semaphore, String msg) {
		super();
		this.msg = msg;
		this.semaphore = semaphore;
	}

	@Override
	public void run() {
		semaphore.waitAndObtain();
		Random generator = new Random();
		try {
//			Thread.sleep(generator.nextInt(maxWaitTime));
			Thread.sleep(generator.nextInt(2000));
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(msg);
		semaphore.release();
	}

}
