package org.gadek.agh.tw;

public class CountingSemaphore implements ISemaphore {

	private int resources;
	private BinarySemaphore theX;
	private BinarySemaphore theY;
	private BinarySemaphore theZ;
	
	public CountingSemaphore(int resources) {
		super();
		assert(resources > 0);
		this.resources = resources;
		theX = new BinarySemaphore();
		theY = new BinarySemaphore();
		theZ = new BinarySemaphore();
		theY.waitAndObtain();
	}

	@Override
	public void waitAndObtain() {
		theX.waitAndObtain();
		resources--;
		if(resources < 0) {
			theX.release();
			theY.waitAndObtain();
		}
		theX.release();
	}

	@Override
	public void release() {
		theX.waitAndObtain();
		resources++;
		if(resources <= 0) theY.release();
		theX.release();
	}

	@Override
	public boolean isLocked() {
		return resources == 0;
	}
	
	public int getCnt() {
		return resources;
	}

}
