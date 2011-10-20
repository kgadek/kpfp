package org.gadek.agh.tw;

public interface ISemaphore {

	public abstract void waitAndObtain();

	public abstract void release();

	public abstract boolean isLocked();

}