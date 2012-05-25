package blackmesa.testlab.server.impl;

public class SimplePasswordChecker {
	private String password;

	public boolean check(String pass) {
		return password != null && password.equals(pass);
	}

	public boolean setPassword(String pass) {
		if(password != null)
			return false;
		password = pass;
		return true;
	}
	
	public boolean resetPassword(String pass) {
		if(check(pass)) {
			password = null;
			return true;
		} else
			return false;
	}
	
	public void resetPassword() {
		password = null;
	}
	
	public boolean isEmpty() {
		return password == null;
	}
}
