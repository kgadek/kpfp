package blackmesa.credentials;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import SR.CertSignerPrx;
import SR.CertSignerPrxHelper;

public class CertificateSignClient {
	public static void main(String args[]) {
		int exitVal = 0;

		if (args.length != 2) {
			System.err.println("CertificateSignClient --Ice.Config=[cert configuration] [csr file path]");
			System.exit(1);
		}

		Ice.Communicator ic = null;
		FileOutputStream fout = null;
		try {
			ic = Ice.Util.initialize(args);
	
			Ice.ObjectPrx base = ic.propertyToProxy("SR.Proxy");
			CertSignerPrx cs = CertSignerPrxHelper.checkedCast(base);
			System.out.println("==Proxy Created!=====");
	
			byte[] bytes = extractBytesArrayFromPath(args[1]);
		
			byte[] effect = cs.signCSR("Konrad", "Gadek", bytes);
			fout = new FileOutputStream(new File(args[1].substring(0, args[1].lastIndexOf(".")) + ".crt"));
			fout.write(effect);
		} catch (Exception e) {
			System.err.println(e);
			e.printStackTrace();
		}
		if(ic != null)
			try {
				ic.destroy();
			} catch (Exception e) {
				e.printStackTrace();
			}
		
		System.out.println("==Signing done!=====");
		System.exit(exitVal);
	}

	public static byte[] extractBytesArrayFromPath(String pth) {
		ByteArrayOutputStream boutStrm = null;
		File fileName = new File(pth);
		InputStream inpStrm = null;
		try {
			byte[] buff = new byte[4096];
			boutStrm = new ByteArrayOutputStream();
			try {
				inpStrm = new FileInputStream(fileName);
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
			int read = 0;
			try {
				while ((read = inpStrm.read(buff)) != -1)
					boutStrm.write(buff, 0, read);
			} catch (IOException e) {
				e.printStackTrace();
			}
		} finally {
			if (inpStrm != null)
				try {
					inpStrm.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}
		return boutStrm.toByteArray();
	}

}
