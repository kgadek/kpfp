package blackmesa.testlab;

import IceSSL.NativeConnectionInfo;

public class TestLabCredentialsVerifier implements IceSSL.CertificateVerifier {

	@Override
	public boolean verify(NativeConnectionInfo info) {
		/*X509Certificate cert0 = (X509Certificate)info.nativeCerts[0];
		if(info.nativeCerts[0] == null)
			return true;
		System.out.println("**** verify: " + cert0.getSubjectDN());
		X500Principal p = cert0.getIssuerX500Principal();
		if(p.getName().indexOf("CN=CA dla Systemów Rozproszonych 2012") != -1)
			return true;
		return false;*/
		return true;
	}

}
