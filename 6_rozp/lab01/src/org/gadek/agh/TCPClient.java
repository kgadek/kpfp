package org.gadek.agh;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class TCPClient {

	static int port = 12346;
	static String host = "localhost";
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Socket socket = null;
		try {
			socket = new Socket(host,port);
			DataOutputStream os = new DataOutputStream(socket.getOutputStream());
			os.writeUTF("asdf");
			DataInputStream is = new DataInputStream(socket.getInputStream());
			String txt = is.readUTF();
			System.out.format("response = %s", txt);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if(socket != null) {
				try {
					socket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

}
