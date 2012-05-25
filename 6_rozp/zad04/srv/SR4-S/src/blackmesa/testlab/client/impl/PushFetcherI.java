package blackmesa.testlab.client.impl;

import Ice.Current;
import blackmesa.testlab.scientist._PushFetcherDisp;

public class PushFetcherI extends _PushFetcherDisp {
	private static final long serialVersionUID = -9183959888196350232L;

	@Override
	public void notifyMe(String info, Current __current) {
		System.out.println(info);
	}
}
