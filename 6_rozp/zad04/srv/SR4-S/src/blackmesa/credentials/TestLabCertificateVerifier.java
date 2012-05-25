package blackmesa.credentials;

import IceSSL.CertificateVerifier;
import IceSSL.NativeConnectionInfo;

public class TestLabCertificateVerifier implements CertificateVerifier {
	@Override
	public boolean verify(NativeConnectionInfo info) {
		System.out.println("==Verifier working!====="); // HOW SIMPLE IS THAT!
		return true;
	}
}
