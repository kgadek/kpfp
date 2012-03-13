package org.gadek.agh;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;

public class UDPServer implements Runnable {

	private static final int PORT = 12346;
	
	DatagramSocket socket;
	int buffSize;
	
	public UDPServer(InetAddress address, int port) {
		super();
		try {
			if(address.isMulticastAddress()) {
				socket = new MulticastSocket(port);
				((MulticastSocket)socket).joinGroup(address);
			} else
				socket = new DatagramSocket(port, address);
			buffSize = socket.getReceiveBufferSize();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		while (true) {
			try {
				byte data[] = new byte[buffSize];
				DatagramPacket packet = new DatagramPacket(data, buffSize);
				socket.receive(packet);
				data = packet.getData();
				DataInputStream dis = new DataInputStream(
						new ByteArrayInputStream(data));
				System.out.format("%s%s%s%d", dis.readUTF(), dis.readUTF(),
						dis.readUTF(), dis.readInt());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {
		try {
			(new UDPServer(InetAddress.getLocalHost(), PORT)).run();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
	}

}
