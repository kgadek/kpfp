package org.gadek.agh;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPMultiServer implements Runnable {

	private static final int PORT = 12346;
	private static final int THREADS_COUNT = 10;
	private static final int DEFAULT_DELAY = 1000;
	
	private static Thread[] ThreadPool;
	private static ServerSocket ssocket;
	
	private int delay;
	
	public TCPMultiServer(int delay, int threadNumber) {
		super();
		this.delay = delay;
	}
	
	@Override
	public void run() {
		Socket socket = null;
		
		while(true) {
			try {
				socket = ssocket.accept();
				DataInputStream is = new DataInputStream(socket.getInputStream());
				String txt = is.readUTF();
				Thread.sleep(delay);
				DataOutputStream os = new DataOutputStream(socket.getOutputStream());
				os.writeUTF(txt.toUpperCase());
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if(socket != null) {
					try {
						socket.close();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	public static void main(String[] args) {
		ThreadPool = new Thread[THREADS_COUNT];
		
		try {
			TCPMultiServer.ssocket = new ServerSocket(PORT);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		for(Thread i : ThreadPool) {
			i = new Thread(new TCPMultiServer(DEFAULT_DELAY, THREADS_COUNT));
			i.start();
		}
		
	}

}
