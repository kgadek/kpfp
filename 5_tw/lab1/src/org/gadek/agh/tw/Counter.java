package org.gadek.agh.tw;

public class Counter {
	private int cnt;

	public int getCnt() {
		return cnt;
	}

	public void setCnt(int cnt) {
		this.cnt = cnt;
	}
	
	public int add(int direction) {
		cnt += direction;
		return cnt;
	}
}
