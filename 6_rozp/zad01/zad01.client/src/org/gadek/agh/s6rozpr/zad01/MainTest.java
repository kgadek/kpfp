package org.gadek.agh.s6rozpr.zad01;

import static org.junit.Assert.*;

import java.io.IOException;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class MainTest {
	
	private Client client = null;
	private static final String HOST = "localhost";
	private static final int PORT = 12346;

	@Before
	public void setUp() throws Exception {
		client = new Client(HOST, PORT);
	}

	@Test
	public void testMain() throws IOException {
		assertEquals(0xAB+3, client.sendByte((byte) 0xAB));
		assertEquals(0xAAAB+3, client.sendShort((short) 0xAAAB));
		assertEquals(0xAAAAAAAB+3, client.sendInt(0xAAAAAAAB));
		assertEquals(0xAAAAAAAAAAAAAAABL+3, client.sendLong(0xAAAAAAAAAAAAAAABL));
	}

}
