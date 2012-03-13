package org.gadek.agh;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class UDPClient implements Runnable {
	
	private static final int PORT = 12346;
	
	private DatagramSocket socket;
	private InetAddress address;
	private int buffSize;
	private int port;

	public UDPClient(InetAddress address, int port) {
		super();
		this.address = address;
		this.port = port;
		try {
			this.socket = new DatagramSocket();
			this.buffSize = socket.getSendBufferSize();
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream(buffSize);
			DataOutputStream dos = new DataOutputStream(baos);
			dos.writeUTF("Konrad ");
			dos.writeUTF("tu ");
			dos.writeUTF("by³: ");
			dos.writeInt(2012);
			byte[] data = baos.toByteArray();
			socket.send(new DatagramPacket(data, data.length, address, port));
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			socket.close();
		}
	}
	
	public static void main(String[] args) {
		try {
			(new UDPClient(InetAddress.getLocalHost(), PORT)).run();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
	}
}
