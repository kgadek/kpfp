/*
 *      FTPCommander.java
 *      
 *      Copyright 2011 Michał Nowak <minowak@minowak-K51AE>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

// AWT
import java.awt.*;
import java.awt.event.*;
// SWING
import javax.swing.*;
import javax.swing.table.*;
// reszta
import java.io.*;
import java.net.*;

public class FTPCommander extends JFrame implements ActionListener {
	
	class SimpleFTPClient {

		private URLConnection m_client;
		private String host;
		private String user;
		private String password;
		private String remoteFile;
		private String erMesg;
		private String succMesg;

		public SimpleFTPClient(){}
		public void setHost (String host) {
			this.host = host;
		}
  
		public void setUser (String user) {
			this.user = user;
		}

		public void setPassword (String p) {
			this.password = p;
		}

		public void setRemoteFile (String d) {
			this.remoteFile = d;
		}

		public synchronized String getLastSuccessMessage() {
			if (succMesg==null ) return ""; return succMesg;
		}

		public synchronized String getLastErrorMessage() {
			if (erMesg==null ) return ""; return erMesg;
		}

		public synchronized boolean uploadFile (String localfilename, int length) {
			new ProgressBar(FTPClient, localfilename,
						length).start();
			return true;
		}

  public synchronized boolean downloadFile (String localfilename) {
    try{
      InputStream is = m_client.getInputStream();
      BufferedInputStream bis = new BufferedInputStream(is);

      OutputStream os = new FileOutputStream(localfilename);
      BufferedOutputStream bos = new BufferedOutputStream(os);

      byte[] buffer = new byte[1024];
      int readCount;

      while( (readCount = bis.read(buffer)) > 0)
      {
        bos.write(buffer, 0, readCount);
      }
      bos.close();
      is.close (); // close the FTP inputstream
      this.succMesg = "Downloaded!";

      return true;
    }catch(Exception ex)
    {
      StringWriter sw0= new StringWriter ();
      PrintWriter p0= new PrintWriter ( sw0, true );
      ex.printStackTrace ( p0 );
      erMesg = sw0.getBuffer().toString ();

      return false;
    }
  }

  public synchronized boolean connect() {
    try{

    URL url = new URL("ftp://"+user+":"+password+"@"+host+"/"+remoteFile+";type=i");
    m_client = url.openConnection();

    return true;

    }
    catch(Exception ex)
    {
      StringWriter sw0= new StringWriter ();
      PrintWriter p0= new PrintWriter ( sw0, true );
      ex.printStackTrace ( p0 );
      erMesg = sw0.getBuffer().toString ();
      return false;
    }
  }
}
	
	class ProgressBar extends Thread {
		private JProgressBar pb;
		private JFrame frame;
		private JPanel panel1;
		private JPanel panel;
		private int progress = 0;
		private String file1, file2;
		private long cnt = 0;
		private static final int WIDTH  = 80;
		private static final int HEIGHT = 30;
		private SimpleFTPClient parent;
	
		public void run() {
			frame.setVisible(true);
			pb.setMaximum((int)cnt);

			try{
				InputStream in = new FileInputStream(file1);
				OutputStream out = new FileOutputStream(file2);
			
				byte[] buff = new byte[1024] ;
				int len;
			
				while((len = in.read(buff)) > 0) {
					out.write(buff, 0, len);
					progress += len;	
					pb.setValue(progress);		// dodac mozliwosc kilku naraz ??
				}								
				in.close();
				out.close();
				Thread.currentThread().sleep(500); // zeby zobaczyc progres
			} catch(Exception e) {
				e.printStackTrace();
			}
			frame.setVisible(false);
		}
	
		public ProgressBar(SimpleFTPClient pr, String f1, long count) { // wysyla f1 na serwer
		    parent = pr;
			
			file1 = new String(f1);
			cnt = count;
			frame = new JFrame("Uploading . . .");
			pb = new JProgressBar(0, 100);
			pb.setValue(0);
			pb.setStringPainted(true);
			pb.setMaximum((int)count);
			// tu upload
			
			try{

				InputStream is = new FileInputStream(file1);
				BufferedInputStream bis = new BufferedInputStream(is);
				OutputStream os =pr.m_client.getOutputStream();
				BufferedOutputStream bos = new BufferedOutputStream(os);
				byte[] buffer = new byte[1024];
				int readCount;

				while( (readCount = bis.read(buffer)) > 0)
				{
					bos.write(buffer, 0, readCount);
					progress += readCount;
					pb.setValue(progress);
				}
				bos.close();

				progress = 100;
			}
			catch(Exception ex)
			{	
				ex.printStackTrace();
			}
			
			Toolkit tk = Toolkit.getDefaultToolkit();
			Dimension screenSize = tk.getScreenSize();
			
			frame.setBounds((getPositionX() + width / 2) - WIDTH, 
							(getPositionY() + height / 2) - HEIGHT, 
							WIDTH, HEIGHT);
    
			JPanel panel = new JPanel();
			panel.add(pb);

			JPanel panel1 = new JPanel();
			panel1.setLayout(new BorderLayout());
			panel1.add(panel, BorderLayout.CENTER);
			panel1.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));
			frame.setContentPane(panel1);
			frame.pack();
			frame.setVisible(false);
		}
	}
	
	private class MyMouseListener implements MouseListener {
		private int number;
		public MyMouseListener(int nr) { number = nr; }
			public void mouseClicked(MouseEvent e) {
			int j = fileSizes[number].rowAtPoint(e.getPoint());
			j--;
			if(j == -1) {
				if(here[number].getParent() != null) {
					pwd[number] = here[number].getParent();
					update();
					model[number].setDataVector(lista[number], columnNames);
					model[number].fireTableDataChanged();
				}
			} else
			if( j >= 0 && j <= files[number].length) {
				if(files[number][j].isDirectory()) {
					pwd[number] = files[number][j].getAbsolutePath();
					update();
					model[number].setDataVector(lista[number], columnNames);
					model[number].fireTableDataChanged();
				} else
				if(files[number][j].isFile()) {
					// kliknbal w plik <-----------------------------------------
					FTPClient.setRemoteFile("/");
					FTPClient.uploadFile(pwd[number] + "/" + files[number][j].getName(), (int)files[number][j].length());
				}
			}
		}
		
		public void mousePressed(MouseEvent e) {}
		public void mouseReleased(MouseEvent e) {}
		public void mouseEntered(MouseEvent e) {}
		public void mouseExited(MouseEvent e) {}
	}
	
	private static final int width = 900;
	private static final int height = 400;
	private static final String version = "1.00a";
	
	private JTable[] fileSizes;				// tabelki z plikami
	private String[][][] lista;				// nazwa pliku + opis 			
	private File[][] files;					// pliki 						
	private File[] here;						// gdzie jestem 			
	private String[] pwd;						// sciezka gdzie sie znajduje		
	private String[] columnNames = {"File", "Description"};
	
	private String title = "News's FTPCommander v." + version;	// tytul
	
	private Container[] inside;				// kontener na elementy JTable
	private DefaultTableModel[] model;		// model wygladu
	private JTableHeader[] header;			// naglowek
	private JScrollPane[] scrollPane;		// scroll'e
	
	private JButton connect = new JButton("Connect");
	private JTextField userTextField = new JTextField(15);
	private JTextField passwordTextField = new JTextField(15);
	private JTextField addressTextField = new JTextField(15);
	
	private JLabel userLabel = new JLabel("User name: ");
	private JLabel passwordLabel = new JLabel("Password: ");
	private JLabel addressLabel = new JLabel("Server: ");
	
	public String username;
	public String password;
	public String address;
	
	private SimpleFTPClient FTPClient = new SimpleFTPClient();
	
	public FTPCommander() throws Exception {
		setTitle(title);
		setSize(width, height);
		
		connect.addActionListener(this);
		
		Toolkit tk = Toolkit.getDefaultToolkit();
		Dimension screenSize = tk.getScreenSize();
		// ustawiam na srodku
		setBounds(screenSize.width / 2 - width / 2, screenSize.height / 2 - height / 2, width, height);
		
		pwd 	= new String[2];
		here 	= new File[2];
		files 	= new File[2][];
		lista 	= new String[2][][];
		
		//pwd[1] = //adres ftp
		
		inside = new Container[2];
		for(int i = 0 ; i < 2 ; i++) {
			inside[i] = getContentPane();
			pwd[i] = "/";
		}
		update();
		
		fileSizes = new JTable[2];
		fileSizes[0] = new JTable();
		fileSizes[1] = new JTable();
		model = new DefaultTableModel[2];
		
		for(int i = 0 ; i < 2 ; i++) {
			model[i] = (DefaultTableModel) fileSizes[i].getModel();
			model[i].setDataVector(lista[i], columnNames);
			model[i].fireTableDataChanged();
			fileSizes[i].setEnabled(true); // change
			fileSizes[i].addMouseListener(new MyMouseListener(i));
		}
		
		
		header = new JTableHeader[2];
		scrollPane = new JScrollPane[2];
		
		for(int i = 0 ; i < 2 ; i++) {
			header[i] = fileSizes[i].getTableHeader();
			header[i].setBackground(Color.GRAY);
			scrollPane[i] = new JScrollPane(fileSizes[i]);
		}
		
		setUndecorated(true);
		getRootPane().setWindowDecorationStyle(JRootPane.PLAIN_DIALOG);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		Panel ftpconnect = new Panel();
		ftpconnect.setLayout(new FlowLayout());
		ftpconnect.add(userLabel);
		ftpconnect.add(userTextField);
		ftpconnect.add(passwordLabel);
		ftpconnect.add(passwordTextField);
		ftpconnect.add(addressLabel);
		ftpconnect.add(addressTextField);
		ftpconnect.add(connect);
		
		inside[0].add(scrollPane[0], BorderLayout.CENTER);
		inside[1].add(scrollPane[1], BorderLayout.EAST);
		inside[0].add(ftpconnect, BorderLayout.SOUTH);

	}
	
	void update() {
		setTitle(title + "   " + pwd[0] + "  |  " + pwd[1]);

		for(int k = 0 ; k < 2 ; k++) {
			here[k] = new File(pwd[k]);
			try {
				files[k] = here[k].listFiles();
				lista[k] = new String[files[k].length + 1][2];
			} catch(Exception e) {
				e.printStackTrace();
			}
			lista[k][0][0] = "..";
			int j = 0;
			int i = 0;
			File[] tmpfiles = new File[files[k].length];
			while(i < files[k].length) {
				String tmp = files[k][i].getName();
				if(!files[k][i].isFile()) {
					lista[k][j + 1][0] = files[k][i].getName();
					lista[k][j + 1][1] = "<DIR>";
					tmpfiles[j] = files[k][i];
					j++;
				}
				i++;
			}
			i = 0;
			while(i < files[k].length) {
				String tmp = files[k][i].getName();
				if(files[k][i].isFile()) {
					lista[k][j + 1][0] = files[k][i].getName();
					lista[k][j + 1][1] = new Long(files[k][i].length()).toString() + "B";
					tmpfiles[j] = files[k][i];
					j++;
				}
				i++;
			}
			files[k] = tmpfiles;
		}
	}
	
	public int getPositionX() {
		Point point = getLocationOnScreen();
		return point.x;
	}
	
	public int getPositionY() {
		Point point = getLocationOnScreen();
		return point.y;
	}
	
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() == connect) {
			address = addressTextField.getText();
			username = userTextField.getText();
			password = passwordTextField.getText();
			System.out.println("Laczenie z " + address);
			// laczenie z ftp i podpisywanie pwd[1]
			FTPClient.setHost(address);
			FTPClient.setUser(username);
			FTPClient.setPassword(password);
			if(FTPClient.connect() == true) {
				System.out.println("Poloczono!");
				connect.setEnabled(false);
			}
		}
	}
	
	public static void main (String args[]) {
		EventQueue.invokeLater(new Runnable() {
								public void run() {
									try {
										FTPCommander myFileExplorer = new FTPCommander();
										myFileExplorer.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
										myFileExplorer.setVisible(true);
										
									} catch(Exception e) {
										e.printStackTrace();
									}
								}
							} );
	}
}
