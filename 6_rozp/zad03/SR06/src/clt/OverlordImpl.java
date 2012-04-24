package clt;

import lab.OverlordPOA;

public class OverlordImpl extends OverlordPOA {

	@Override
	public void detector(String event) {
		System.out.println(event);
	}

}
