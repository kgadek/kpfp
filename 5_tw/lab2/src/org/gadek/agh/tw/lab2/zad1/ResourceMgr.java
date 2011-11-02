package org.gadek.agh.tw.lab2.zad1;

import java.util.concurrent.Semaphore;

public class ResourceMgr {
	private final static double maxWaitTime = 50.0;
	
	private static int[] res; 			/** resources */
	private static Semaphore free; 		/** counts free resources */
	private static Semaphore occupied; 	/** counts occupied resources */
	private static int resN; 			/** number of produced resources */
	private static int currRes; 		/** current resource pointer */
	
	public static void main(String[] args) {
		ResourceMgr magazyn = new ResourceMgr(1);
		System.out.println("" + args);
		if(args.length > 0 && args[0].equals("mon"))
			magazyn.monSim();
		else if(args.length > 0 && args[0].equals("sem"))
			magazyn.semSim();
	}

	public ResourceMgr(int resBuffer) {
		res = new int[resBuffer];
		resN = currRes = 0;
		free = new Semaphore(resBuffer);
		occupied = new Semaphore(0);
	}
	
	private void semSim() {
		Thread p = new ProducerSem(this);
		Thread c = new ConsumerSem(this);
		p.start();
		c.start();
	}

	private void monSim() {
		Thread p = new ProducerMon(this);
		Thread c = new ConsumerMon(this);
		p.start();
		c.start();
	}
	
	static class ProducerSem extends Thread {
		private int idx;
		public ProducerSem(ResourceMgr resourceManager) {
			idx = 0;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*ResourceMgr.maxWaitTime));
					free.acquire();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("ProducerSem made " + (res[idx = ((idx+1) % res.length)] = 7));
				occupied.release();
			}
		}
	}
	
	static class ConsumerSem extends Thread {
		private int idx;
		public ConsumerSem(ResourceMgr resourceManager) {
			idx = 0;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*ResourceMgr.maxWaitTime));
					occupied.acquire();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("ConsumerSem got " + res[idx = ((idx+1) % res.length)]);
				free.release();
			}
		}
	}
	
	static class ProducerMon extends Thread {
		private ResourceMgr resMgr = null;
		public ProducerMon(ResourceMgr resourceManager) {
			resMgr = resourceManager;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*ResourceMgr.maxWaitTime));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				resMgr.produce("ProducerMonitor", 7);
			}
		}
	}
	
	static class ConsumerMon extends Thread {
		private ResourceMgr resMgr = null;
		public ConsumerMon(ResourceMgr resourceManager) {
			resMgr = resourceManager;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*ResourceMgr.maxWaitTime));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				resMgr.consume("ConsumeMonitor");
			}
		}
	}

	public synchronized void consume(String consumer) {
		while(resN == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		int consumed = res[currRes];
		res[currRes = ((currRes+1) % res.length)] = 0;
		--resN;
		System.out.println("Consumer " + consumer + " gets " + consumed); // mo¿e siê pogryŸæ tylko z println z produce
		notify();
	}
	
	public synchronized void produce(String producer, int product) {
		while(resN == res.length) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		res[currRes = ((currRes+1) % res.length)] = product;
		++resN;
		System.out.println("Producer " + producer + " made " + product + " resN:" + resN + " res.length:" + res.length + " currRes:" + currRes);
		notify();
	}
	
}
