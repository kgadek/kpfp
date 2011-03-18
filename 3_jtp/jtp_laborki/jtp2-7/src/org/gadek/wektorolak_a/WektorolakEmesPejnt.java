package org.gadek.wektorolak_a;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Vector;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

/**
 * Uwaga - klasa nie jest odporna na ataki Cthulu.
 * @author Konrad
 */
public class WektorolakEmesPejnt extends Canvas {
	/**
	 * Konstruktor. W sumie nie robi nic istotnego oprócz tego, ¿e
	 * robi bardzo wa¿ne rzeczy ;) Pierwsze primo: inicjalizacja zmiennych.
	 * Drugie primo: dodajê obs³ugê zdarzeñ. I najwa¿niejsze - trzecie
	 * primo: zmienia kolor t³a na bia³y.
	 */
	public WektorolakEmesPejnt() {
		points = new Vector<Point2D>();
		addMouseListener(new WektorolakowyJerry());
		setBackground(Color.WHITE);
		locked = false;
		mask = null;
		
		akszynSejw = new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser czuz = new JFileChooser();
				czuz.setCurrentDirectory(new File("."));
				czuz.setSelectedFile(new File("pikczer.ser"));
				int res = czuz.showDialog(null,"Sejw");
				if(res == JFileChooser.APPROVE_OPTION)
					save(czuz.getSelectedFile().getPath());
			}
		};
		akszynLo³d = new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser czuz = new JFileChooser();
				czuz.setCurrentDirectory(new File("."));
				czuz.setSelectedFile(new File("pikczer.ser"));
				int res = czuz.showDialog(null,"O³pen");
				if(res == JFileChooser.APPROVE_OPTION)
					load(czuz.getSelectedFile().getPath());
			}
		};
		akszynKlin = new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				clear();
			}
		};
		akszynFlaszbak = new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				undo();
			}
		};
		akszynCthulu = new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				fightCthulhu();
			}
		};
	}
	
	/* (non-Javadoc)
	 * @see java.awt.Canvas#paint(java.awt.Graphics)
	 */
	public void paint(Graphics g) {
		Graphics2D g2 = (Graphics2D)g;
		
		Point2D prev = null;
		for(Point2D p : points) {
			if(prev != null)
				g2.draw(new Line2D.Double(prev, p));
			prev = p;
		}
		if(mask != null)
			g.drawImage(mask, 0,0, this);
	}
	
	/**
	 * Dodaje punkt do wektora. 
	 * @param p Dodawany punkt
	 */
	public void add(Point2D p) {
		if(!locked) {
			points.add(p);
			repaint();
		} else {
			Toolkit kit = Toolkit.getDefaultToolkit();
			kit.beep();
		}
	}
	
	/**
	 * Zapisywanie WEKTORO£AKA do pliku.
	 * @param filename Nazwa pliku.
	 */
	private void save(String filename) {
		locked = true;
		try {
			ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(filename));
			oos.writeObject(points);
			oos.close();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			locked = false;
		}
	}
	
	/**
	 * £adowanie WEKTORO£AKA z pliku.
	 * @param filename Nazwa pliku. Przyda³oby siê, by jednak istnia³ bo
	 * inaczej bêdzie Wielka BUBA™.
	 */
	private void load(String filename) {
		locked = true;
		try {
			ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filename));
			points = (Vector<Point2D>)ois.readObject();
			repaint();
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(this, "Bubaczek jest niezadowolony - plik " +
					filename + " nie istnieje!");
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			locked = false;
		}
	}
	
	/**
	 * Czyœci pole Canvas. 
	 */
	private void clear() {
		locked = true;
		points.clear();
		points.removeAllElements();
		mask = null;
		repaint();
		locked = false;
	}
	
	/**
	 * Cofaj, szwagier!
	 */
	private void undo() {
		if(!points.isEmpty())
			points.remove(points.size()-1);
		repaint();
	}
	
	/**
	 * Walka z Cthulhu? WTF?
	 */
	private void fightCthulhu() {
		if(mask != null) {
			JOptionPane.showMessageDialog(this, "Ej ej! Po *jednym* Cthulhu dla ka¿dego!");
			return;
		}
		MediaTracker media = new MediaTracker(this);
		//mask = Toolkit.getDefaultToolkit().getImage("maskotka.gif");
		mask = Toolkit.getDefaultToolkit().getImage(getClass().getResource("maskotka.gif"));
		media.addImage(mask, 0);
		try {
			media.waitForID(0);
		} catch( Exception e) {
			e.printStackTrace();
		}
		repaint();
		JOptionPane.showMessageDialog(this, "Nie musisz walczy z Cthulhu... Od dzisiaj Cthulhu dla ka¿dego!");
	}
	
	/**
	 * Obiekt zajmuj¹cy siê obs³ug¹ zdarzeñ od ró¿nego rodzaju myszy (Tom
	 * jest na L-4). 
	 */
	public class WektorolakowyJerry extends MouseAdapter {
		public void mousePressed(MouseEvent e) {
			if(e.isControlDown())
				save("sejw.ser"); 
			else if(e.isShiftDown())
				load("sejw.ser");	// to siê nazywa uproszczone Ctrl-Z
			else
				add(e.getPoint());
		}
	}
	
	/**
	 * Unikalny ID klasy. A przy okazji rozwiniêcie liczby PI.
	 */
	private static final long serialVersionUID = 31415926535L;
	
	/**
	 * WEKTORO£AK! Lista punktów wyznaczaj¹ca wektor wynikowy.
	 */
	private Vector<Point2D> points;
	
	/**
	 * Locked s³u¿y do blokowania mo¿liwoœci rysowania w trakcie
	 * operacji odczytu/zapisu.
	 */
	private boolean locked;
	
	/**
	 * Akszyn-Listener do zapisu plików.
	 */
	public ActionListener akszynSejw;

	/**
	 * Akszyn-Listener do odczytu plików.
	 */
	public ActionListener akszynLo³d;
	
	/**
	 * Akszyn-Listener do czyszczenia pola Canvas.
	 */
	public ActionListener akszynKlin;
	
	/**
	 * Akszyn-Listener do cofania.
	 */
	public ActionListener akszynFlaszbak;
	
	/**
	 * Akszyn-Listener do niczego, bo i tak nikt nie spodziewa siê
	 * hiszpañskiego Cthulhu!
	 */
	public ActionListener akszynCthulu;
	
	/**
	 * Zupe³nie zbêdny dodatek.
	 */
	Image mask;
}
