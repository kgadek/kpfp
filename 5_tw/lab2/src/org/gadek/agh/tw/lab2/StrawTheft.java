package org.gadek.agh.tw.lab2;

import java.util.concurrent.Semaphore;

public class StrawTheft {
	private final static double maxWaitTime = 5000.0;
	private static boolean[] slot;
	private static int M;
	private static Semaphore freeSlots;
	private static Semaphore straws = new Semaphore(0, true);
	private static Semaphore iWantToSay = new Semaphore(1,true);

	public StrawTheft(int M) {
		StrawTheft.M = M;
		slot = new boolean[M];
		freeSlots = new Semaphore(2*M);
	}
	
	static class McDonald extends Thread {
		private static int idx = 0;
		private static Semaphore thisCityIsTooSmallForTwoMcDs = new Semaphore(1, true);
		private int mcdId;
		
		public McDonald(int mcdId) {
			super();
			this.mcdId = mcdId;
		}
		
		@Override
		public void run() {
			int produceHowMany;
			while(true) {
				produceHowMany = (int)(Math.random()*(M+1));
				assert(produceHowMany <= M);
				try {
					Thread.sleep((int)(Math.random()*StrawTheft.maxWaitTime));
					freeSlots.acquire(produceHowMany);
					thisCityIsTooSmallForTwoMcDs.acquire();
					for(int i=0; i<produceHowMany; ++i)
						slot[idx = (idx+1)%slot.length] = true;
					iWantToSay.acquire();
					System.out.print("McD produced some straws, in a bucket I see:\n\t [");
					for(int i=0; i<slot.length; ++i)
						System.out.print(slot[i] ? "|" : "_");
					System.out.println(" ]");
					iWantToSay.release();
					thisCityIsTooSmallForTwoMcDs.release();
					straws.release(produceHowMany);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	static class Theft extends Thread {
		private static int idx = 0;
		private static Semaphore oneStudentOneCup = new Semaphore(1,true);
		private int thId;
		
		public Theft(int thId) {
			super();
			this.thId = thId;
		}

		@Override
		public void run() {
			int grabHowMany;
			while(true) {
				grabHowMany = (int)(Math.random()*(M+1));
				assert(grabHowMany <= M);
				try {
					Thread.sleep((int)(Math.random()*StrawTheft.maxWaitTime));
					straws.acquire(grabHowMany);
					oneStudentOneCup.acquire();
					for(int i=0; i<grabHowMany; ++i)
						slot[idx = (idx+1)%slot.length] = false;
					iWantToSay.acquire();
					System.out.print("Theft grabbed some straws, in a bucket I see:\n\t [");
					for(int i=0; i<slot.length; ++i)
						System.out.print(slot[i] ? "|" : "_");
					System.out.println(" ]");
					iWantToSay.release();
					oneStudentOneCup.release();
					freeSlots.release(grabHowMany);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	public void doTheftiery(int p, int c) {
		McDonald[] mcds = new McDonald[p];
		Theft[] students = new Theft[c];
		for(int i=0; i<p; ++i)
			mcds[i] = new McDonald(i);
		for(int i=0; i<c; ++i)
			students[i] = new Theft(i);
		for(int i=0; i<p; ++i)
			mcds[i].start();
		for(int i=0; i<c; ++i)
			students[i].start();
	}
	
	public static void main(String[] args) {
		StrawTheft warszawa = new StrawTheft(100);
		warszawa.doTheftiery(40, 300);
	}

}
