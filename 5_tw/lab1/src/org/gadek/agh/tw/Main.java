package org.gadek.agh.tw;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Counter counter1 = new Counter();
		SimpleCounter simpleInc = new SimpleCounter(counter1, 1);
		SimpleCounter simpleDec = new SimpleCounter(counter1, -1);
		simpleInc.start();
		simpleDec.start();
		try {
			simpleInc.join();
			simpleDec.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("SimpleCounter: " + counter1.getCnt());
		
		Counter counter2 = new Counter();
		SynchronizedCounter synchInc = new SynchronizedCounter(counter2, 1);
		SynchronizedCounter synchDec = new SynchronizedCounter(counter2, -1);
		synchInc.start();
		synchDec.start();
		try {
			synchInc.join();
			synchDec.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("SynchronizedCounter: " + counter2.getCnt());
		
		Counter counter3 = new Counter();
		LockingCounter lockInc = new LockingCounter(counter3, 1);
		LockingCounter lockDec = new LockingCounter(counter3, -1);
		lockInc.start();
		lockDec.start();
		try {
			lockInc.join();
			lockDec.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("LockingCounter: " + counter3.getCnt() + "\n\n");
		
		CountingSemaphore cntngSem = new CountingSemaphore(3);
		List<RandomPrinter> printersLst = new ArrayList<RandomPrinter>();
		for(int i = 0; i<5; i += 1) {
			printersLst.add(new RandomPrinter(cntngSem,"Bla "+i));
		}
		for(RandomPrinter i : printersLst) {
			System.out.print(".");
			i.start();
		}
		System.out.println();
		for(RandomPrinter i : printersLst) {
			try {
				i.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		System.out.println("DONE");
		
	}

}
