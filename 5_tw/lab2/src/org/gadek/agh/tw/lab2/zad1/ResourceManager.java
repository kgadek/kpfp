package org.gadek.agh.tw.lab2.zad1;

import java.util.concurrent.Semaphore;

public class ResourceManager {
	private static int[] res; /** resources */
	private static Semaphore free; /** counts free resources */
	private static Semaphore occupied; /** */
	private int resN; /** number of produced resources */
	private int currRes; /** current resource pointer */
	
	public ResourceManager(int resBuffer) {
		res = new int[resBuffer];
		resN = currRes = 0;
		free = new Semaphore(resBuffer);
		occupied = new Semaphore(0);
	}

	public static void main(String[] args) {
		ResourceManager magazyn = new ResourceManager(10);
		magazyn.monSim();
		magazyn.semSim();
	}

	private void semSim() {
		Thread p = new ProducerSem(this);
		Thread c = new ConsumerSem(this);
		p.start();
		c.start();
	}

	private void monSim() {
		Thread p = new ProducerMon(this);
		Thread c = new ProducerMon(this);
		p.start();
		c.start();
	}
	
	static class ProducerSem extends Thread {
		private int idx;
		public ProducerSem(ResourceManager resourceManager) {
			idx = 0;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*5000.0));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				try {
					free.acquire();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("ProducerSem made " + (res[idx++ % res.length] = 7));
			}
		}
	}
	
	static class ConsumerSem extends Thread {
		public ConsumerSem(ResourceManager resourceManager) {
		}
		@Override
		public void run() {
		}
	}
	
	static class ProducerMon extends Thread {
		private ResourceManager resMgr = null;
		public ProducerMon(ResourceManager resourceManager) {
			resMgr = resourceManager;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*5000.0));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				resMgr.produce("ProducerMonitor", 7);
			}
		}
	}
	
	static class ConsumerMonitor extends Thread {
		private ResourceManager resMgr = null;
		public ConsumerMonitor(ResourceManager resourceManager) {
			resMgr = resourceManager;
		}
		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep((int)(Math.random()*5000.0));
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
		int consumed = res[currRes % res.length];
		res[currRes++ % res.length] = 0;
		--resN;
		notify();
		System.out.println("Consumer " + consumer + " gets " + consumed);
	}
	
	public synchronized void produce(String producer, int product) {
		while(resN == res.length) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		res[currRes++ % res.length] = product;
		notify();
		System.out.println("Producer " + producer + " made " + product);
	}
	
}
