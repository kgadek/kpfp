package org.gadek.agh;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPMultiServer implements Runnable {

	static final int PORT = 12346;
	static final int THREADS_COUNT = 10;
	static final int DEFAULT_DELAY = 1000;
	
	static Thread[] ThreadPool;
	static ServerSocket ssocket;
	
	int delay;
	int threadNumber;
	
	public TCPMultiServer(int delay, int threadNumber) {
		super();
		this.delay = delay;
		this.threadNumber = threadNumber;
	}
	
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

	/**
	 * @param args
	 */
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
