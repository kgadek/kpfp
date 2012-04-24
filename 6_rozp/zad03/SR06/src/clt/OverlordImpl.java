package clt;

import lab.OverlordPOA;

public class OverlordImpl extends OverlordPOA {
	private final String descr;
	
	public OverlordImpl(String name) {
		super();
		this.descr = name;
	}

	@Override
	public void detector(String event) {
		System.out.format("==EVENT (%s): %s\n", descr, event);
	}

}
