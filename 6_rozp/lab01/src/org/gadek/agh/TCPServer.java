package org.gadek.agh;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

	private static int port = 12346;

	public static void main(String[] args) {
		Socket socket = null;
		ServerSocket ssocket;
		
		while (true) {
			ssocket = null;
			try {
				ssocket = new ServerSocket(port);
				socket = ssocket.accept();
				DataInputStream is = new DataInputStream(socket.getInputStream());
				String txt = is.readUTF();
				DataOutputStream os = new DataOutputStream(socket.getOutputStream());
				os.writeUTF(txt.toUpperCase());
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

}
