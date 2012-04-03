package client;

import java.io.Serializable;
import java.math.BigDecimal;

import compute.Task;

public class Pi implements Task<BigDecimal>, Serializable {

	private static final long serialVersionUID = 1245398358878185583L;

	@Override
	public BigDecimal execute() {
		return new BigDecimal(22.0/7.0);
	}

}
