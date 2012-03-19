package org.gadek.agh.s6rozpr.zad01;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

	private Socket socket = null;
	private DataOutputStream dos = null;
	private DataInputStream dis = null;
	
	public Client(String host, int port) throws UnknownHostException, IOException {
		super();
		this.socket = new Socket(host, port);
		this.dos = new DataOutputStream(this.socket.getOutputStream());
		this.dis = new DataInputStream(this.socket.getInputStream());
	}
	
	public byte sendByte(byte v) throws IOException {
		dos.writeByte(v);
		return dis.readByte();
	}
	
	public short sendShort(short v) throws IOException {
		dos.writeShort(v);
		return dis.readShort();
	}
	
	public int sendInt(int v) throws IOException {
		dos.writeInt(v);
		return dis.readInt();
	}
	
	public long sendLong(long v) throws IOException {
		dos.writeLong(v);
		return dis.readLong();
	}

	public static void main(String[] args) {
		MainTest test = new MainTest();
		try {
			test.setUp();
			test.testMain();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
