import java.io.Serializable;
import java.math.BigDecimal;


public class Pi implements Task<BigDecimal>, Serializable {

	private static final long serialVersionUID = 1L;

	Pi(int lol) {
		super();
	}
	
	@Override
	public BigDecimal execute() {
		return new BigDecimal(3.1415926535);
	}

}
