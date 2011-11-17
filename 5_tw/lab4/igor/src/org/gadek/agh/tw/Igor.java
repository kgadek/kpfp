package org.gadek.agh.tw;

import java.util.Random;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Igor {

	static class Client extends Thread {
		static private final int timeToBeingHungry = 2000;
		static private final int timeToEatWell = 4000;
		static private final String prefixA = "Gentleman";
		static private final String prefixB = "Madamme";
		private int clientId;
		private String title;
		private Igor myServant;

		public Client(int clientId, String title, Igor myServant) {
			assert(title != null && myServant != null);
			this.clientId = clientId;
			this.title = title;
			this.myServant = myServant;
		}

		@Override
		public void run() {
			Random rnd = new Random();
			while(true) {
				try {
					System.out.println(title + " o numerze #" + clientId + " siedzi i DDoSuje 4chana");
					Thread.sleep(rnd.nextInt(Client.timeToBeingHungry));
					System.out.println(title + " o numerze #" + clientId + " zrobi³(a) siê g³odny(a)");
					myServant.wantTable(clientId);
					System.out.println(title + " o numerze #" + clientId + " dosta³(a) stolik");
					Thread.sleep(rnd.nextInt(Client.timeToEatWell));
					myServant.freeTable();
					System.out.println(title + " o numerze #" + clientId + " wsta³(a) od stolika");
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
		
	}
	
	private ReentrantLock iMustStayFocused = new ReentrantLock();
	private Condition waitingForPair[];
	private Condition waitingForTable;
	private int clientsByTheTable;
	
	public Igor(int n) {
		waitingForPair = new Condition[n];
		for(int i=0; i<n; ++i)
			waitingForPair[i] = iMustStayFocused.newCondition();
		waitingForTable = iMustStayFocused.newCondition();
		clientsByTheTable = 0;
	}
	
	public void freeTable() {
		iMustStayFocused.lock();
		if(--clientsByTheTable == 0)
			waitingForTable.signal();
		iMustStayFocused.unlock();
	}

	public void wantTable(int myId) throws InterruptedException {
		iMustStayFocused.lock();
		if( !iMustStayFocused.hasWaiters(waitingForPair[myId]) )
			waitingForPair[myId].await();
		else {
			if(clientsByTheTable > 0)
				waitingForTable.await();
			clientsByTheTable = 2;
			waitingForPair[myId].signal();
		}
		iMustStayFocused.unlock();
	}

	public static void main(String[] args) {
		try {
			int n = 10;
			Igor waiter = new Igor(n);
			Client clientTable[] = new Client[2*n];
			for(int i=0;i<n;++i) {
				clientTable[i] = new Client(i, Client.prefixA, waiter);
				clientTable[n+i] = new Client(i, Client.prefixB, waiter);
			}
			for(int i=0;i<n;++i) {
				clientTable[i].start();
				clientTable[i+n].start();
			}
			for(int i=0; i<n; ++i) {
				clientTable[i].join();
				clientTable[i+n].join();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
}
