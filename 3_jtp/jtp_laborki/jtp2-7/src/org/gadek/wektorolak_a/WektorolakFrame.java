package org.gadek.wektorolak_a;

import java.awt.Color;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

public class WektorolakFrame extends JFrame {
	public WektorolakFrame() {
		Toolkit kit = Toolkit.getDefaultToolkit();
		
		//Image img = kit.getImage("icon.gif");
		Image img = kit.getImage(getClass().getResource("icon.gif"));
		setIconImage(img);
		
		setSize(DEFAULT_W, DEFAULT_H);
		setTitle("Wektoro³ak A");
		setResizable(false);
		
		WektorolakEmesPejnt comp = new WektorolakEmesPejnt();
		comp.setBackground(Color.WHITE);
		getContentPane().add(comp);
		
		JMenuBar meni = new JMenuBar();
		setJMenuBar(meni);
		
		JMenu fajlMeni = new JMenu("Plik");
		fajlMeni.setMnemonic('P');
		meni.add(fajlMeni);
		JMenuItem klin = fajlMeni.add("Nowy");
		klin.addActionListener(comp.akszynKlin);
		klin.setMnemonic('N');
		klin.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_DOWN_MASK));
		JMenuItem o³pen = fajlMeni.add("Otwórz");
		o³pen.addActionListener(comp.akszynLo³d);
		o³pen.setMnemonic('O');
		o³pen.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
		JMenuItem sejw = fajlMeni.add("Zapisz");
		sejw.addActionListener(comp.akszynSejw);
		sejw.setMnemonic('S');
		sejw.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
		JMenuItem cthulu = fajlMeni.add("Odeprzyj ataki Cthulhu");
		cthulu.addActionListener(comp.akszynCthulu);
		cthulu.setMnemonic('H');
		cthulu.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, InputEvent.CTRL_DOWN_MASK));
		
		JMenu editMeni = new JMenu("Edycja");
		editMeni.setMnemonic('E');
		meni.add(editMeni);
		JMenuItem back = editMeni.add("Cofej¿e");
		back.addActionListener(comp.akszynFlaszbak);
		back.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z, InputEvent.CTRL_DOWN_MASK));
		
		setVisible(true);
		comp.createBufferStrategy(2);
	}
	
	/**
	 * Unikalny ID klasy, a przy okazji sta³a Eulera.
	 */
	private static final long serialVersionUID = 27182818284L;
	
	/**
	 * Domyœlna szerokoœæ. 
	 */
	public static final int DEFAULT_W = 600;
	
	/**
	 * Domyœlna wysokoœæ. 
	 */
	public static final int DEFAULT_H = 600;
}
