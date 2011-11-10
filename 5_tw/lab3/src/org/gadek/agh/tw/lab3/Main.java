package org.gadek.agh.tw.lab3;

public class Main {

	static class DelayedAdder extends Thread {
		private MyList lst;
		private int sleep;
		private int times;
		private int[] toAdd;
		public DelayedAdder(MyList lst, int[] toAdd, int sleep, int times) {
			super();
			this.toAdd = toAdd;
			this.times = times;
			this.lst = lst;
			this.sleep = sleep;
		}
		@Override
		public void run() {
			while(times-- >= 0) {
				for(int i : toAdd) {
					assert(lst.add(i));
					try {
						Thread.sleep(sleep);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	static class DelayedReader extends Thread {
		private MyList lst;
		private int[] search;
		private int sleep;
		private int times;
		public DelayedReader(MyList lst, int[] search, int sleep, int times) {
			super();
			this.lst = lst;
			this.search = search;
			this.sleep = sleep;
			this.times = times;
		}
		@Override
		public void run() {
			while(times-- >= 0) {
				for(int i : search) {
					lst.contains(i);
					try {
						Thread.sleep(sleep);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	static class DelayedRemover extends Thread {
		private MyList lst;
		private int sleep;
		private int times;
		private int[] toRemove;
		public DelayedRemover(MyList lst, int sleep, int[] toAdd, int times) {
			super();
			this.toRemove = toAdd;
			this.times = times;
			this.lst = lst;
			this.sleep = sleep;
		}
		@Override
		public void run() {
			while(times-- >= 0) {
				for(int i : toRemove) {
					lst.remove(i);
					try {
						Thread.sleep(sleep);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	public static void main(String[] args) {
		Main.oneThreadedTestOne( new CoarseList() );
		Main.oneThreadedTestTwo( new CoarseList() );
		Main.oneThreadedTestThree( new CoarseList() );
		Main.oneThreadedTestOne( new FineList() );
		Main.oneThreadedTestTwo( new FineList() );
		Main.oneThreadedTestThree( new FineList() );
		long a, b, c;
		a = System.currentTimeMillis();
		Main.multiThreadedTestOne( new CoarseList(), 100);
		b = System.currentTimeMillis();
		Main.multiThreadedTestOne( new FineList(), 100);
		c = System.currentTimeMillis();
		System.out.println("OK: f" + (int)(b-a) + " : c" + (int)(c-b));
	}
	
	public static void multiThreadedTestOne(MyList lst, int thrdz) {
		int[] a = {1, 1, 2, 3, 5, 8, 13, 21};
		int[] b = {7, -1, 33};
		int[] c = {1, 2, 13, 8, 21};
		int[] d = {1, 3, 5};
		
		DelayedReader[] readers = new DelayedReader[thrdz];
		DelayedAdder[] adders = new DelayedAdder[thrdz];
		DelayedRemover[] removers = new DelayedRemover[thrdz];
		for(int i=0; i<thrdz; ++i) {
			readers[i] = 	new DelayedReader(lst, a, 0, 10);
			adders[i] = 	new DelayedAdder(lst, a, 0, 10);
			removers[i] = 	new DelayedRemover(lst, 0, a, 10);
		}
		for(int i=0; i<thrdz; ++i) {
			readers[i].start();
			adders[i].start();
			removers[i].start();
		}
		try {
			for(int i=0; i<thrdz; ++i) {
				readers[i].join();
				adders[i].join();
				removers[i].join();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public static void oneThreadedTestOne(MyList lst) {
		int[] a = {1, 1, 2, 3, 5, 8, 13, 21};
		int[] b = {7, -1, 33};
		
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : a) assert(lst.add(i));
		for(int i : a) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : b) assert(!lst.remove(i));
		for(int i : a) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i = a.length-1; i >= 0; --i) assert(lst.remove(a[i]));
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : b) assert(!lst.remove(i));
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
	}
	
	public static void oneThreadedTestThree(MyList lst) {
		int[] a = {1, 1, 2, 3, 5, 8, 13, 21};
		int[] b = {7, -1, 33};
		int[] c = {1, 2, 13, 8, 21};
		int[] d = {1, 3, 5};
		
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : a) assert(lst.add(i));
		for(int i : a) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : c) assert(lst.remove(i));
		for(int i : d) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : b) assert(!lst.remove(i));
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
	}
	
	public static void oneThreadedTestTwo(MyList lst) {
		int[] a = {1, 1, 2, 3, 5, 8, 13, 21};
		int[] b = {7, -1, 33};
		
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : a) assert(lst.add(i));
		for(int i : a) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : b) assert(!lst.remove(i));
		for(int i : a) assert(lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : a) assert(lst.remove(i));
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
		
		for(int i : b) assert(!lst.remove(i));
		for(int i : a) assert(!lst.contains(i));
		for(int i : b) assert(!lst.contains(i));
	}

}
