package blackmesa.scientist;

import Ice.Current;

public class SpectatorI extends _SpectatorDisp {

	private static final long serialVersionUID = 95753010121396213L;

	@Override
	public void acknowledge(Report rprt, Current __current) {
		System.out.format("[ %2d ]: %s\n", rprt.importance, rprt.msg);

	}

}
