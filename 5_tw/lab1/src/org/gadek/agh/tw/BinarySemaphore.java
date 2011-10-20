package org.gadek.agh.tw;

public class BinarySemaphore implements ISemaphore {
	private boolean locked;

	public BinarySemaphore() {
		locked = false;
	}

	@Override
	synchronized public void waitAndObtain() {
		while(locked) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		locked = true;
	}

	@Override
	synchronized public void release() {
		locked = false;
		notify();
	}

	@Override
	public boolean isLocked() {
		return locked;
	}
	
}
