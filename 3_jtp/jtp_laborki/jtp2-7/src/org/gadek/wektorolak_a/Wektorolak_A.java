package org.gadek.wektorolak_a;

import java.awt.EventQueue;
import javax.swing.*;

/**
 * 
 * @author Konrad
 * @version 001
 */
public class Wektorolak_A {

	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				WektorolakFrame frame = new WektorolakFrame();
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			}
		});
	}
}
