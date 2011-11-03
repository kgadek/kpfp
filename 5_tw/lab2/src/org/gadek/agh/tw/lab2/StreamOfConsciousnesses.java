package org.gadek.agh.tw.lab2;

import java.util.concurrent.Semaphore;

public class StreamOfConsciousnesses {
	private final static double maxWaitTime = 500.0;

	private static int[] thoughts;
	private static Semaphore[] conclusions;

	public static void main(String[] args) {
		StreamOfConsciousnesses disco = new StreamOfConsciousnesses(10, 4);
		disco.think();
	}
	
	public StreamOfConsciousnesses(int numberOfThoughts, int numberOfConclusions) {
		thoughts = new int[numberOfThoughts];
		conclusions = new Semaphore[numberOfConclusions];
		conclusions[0] = new Semaphore(numberOfThoughts);
		for(int i=1; i<numberOfConclusions; ++i)
			conclusions[i] = new Semaphore(0);
	}
	
	public void think() {
		Neuron[] neurons = new Neuron[conclusions.length];
		for(int i=0; i<conclusions.length; ++i)
			neurons[i] = new Neuron(i);
		for(int i=0; i<conclusions.length; ++i)
			neurons[i].start();
	}

	static class Neuron extends Thread {
		private int idx;
		private int neuronId;
		private int aTimeOfPassage;
		
		public Neuron(int hemiId) {
			super();
			this.neuronId = hemiId;
			this.idx = 0;
			this.aTimeOfPassage = (int)(Math.random()*StreamOfConsciousnesses.maxWaitTime);
		}
		
		public String nth(int nth) {
			if(nth == 0 || nth < -3 || nth > 3)
				return "th";
			switch(nth) {
				case 1:
				case -1:
					return "st";
				case 2:
				case -2:
					return "nd";
				default: // 3 / -3
					return "rd";
			}
		}

		@Override
		public void run() {
			while(true) {
				try {
					Thread.sleep(aTimeOfPassage);
					conclusions[neuronId].acquire();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				if(neuronId != 0) // parse…
					thoughts[idx] = (int)((long)thoughts[idx]*(long)neuronId*(long)idx % 2147483629);
				else // …or produce
					thoughts[idx] = 127-idx;
				System.out.println(""+neuronId+nth(neuronId) + " neuron has been passed by "+idx+nth(idx)+" thought and conclusion "+thoughts[idx]+" was born");
				idx = (idx+1) % thoughts.length;
				conclusions[(neuronId+1) % conclusions.length].release();
			}
		}
	}

}
