package org.gadek.agh.sr;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.Inet4Address;
import java.net.UnknownHostException;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import org.jgroups.Message;

import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatAction;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatAction.ActionType;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatMessage;


public class AintNoGui extends JPanel implements ActionListener {
	
	private static final long serialVersionUID = 1L;
	protected JTextField textField;
    protected JTextArea textArea;
	private SpamBot spamBot = null;
	private String currCh = SpamBot.DEFAULT_CHANNEL;
	
	public static void main(String[] args) {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				JFrame frame = new JFrame("Messages");
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.add(new AintNoGui());
				frame.pack();
				frame.setVisible(true);
			}
		});
	}
	
	public AintNoGui() {
		super(new GridBagLayout());
		
		textField = new JTextField(20);
		textField.addActionListener(this);
		 
        textArea = new JTextArea(5, 20);
        textArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(textArea);
		
        GridBagConstraints c = new GridBagConstraints();
        c.gridwidth = GridBagConstraints.REMAINDER;
 
        c.fill = GridBagConstraints.HORIZONTAL;
        add(textField, c);
 
        c.fill = GridBagConstraints.BOTH;
        c.weightx = 1.0;
        c.weighty = 1.0;
        add(scrollPane, c);
        
        textArea.append("==> Podaj swój nick by zacząć\n");
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		String cmd = textField.getText();
		
		if(spamBot == null)
			(spamBot = new SpamBot(cmd, this)).start();
		else {
		
			if(cmd.startsWith("/join ")) {
				try {
					if(!Inet4Address.getByName(cmd.substring("/join ".length())).isMulticastAddress())
						throw new UnknownHostException();
				} catch (UnknownHostException e1) {
					textArea.insert("==> #" + cmd.substring("/join ".length()) + " is not a valid channel\n", 0);
				}
					
				textArea.insert("==> joining #"+cmd.substring("/join ".length())+"\n", 0);
				spamBot.join(cmd.substring("/join ".length()));
				
				spamBot.spamuj(new Message(null,
						ChatAction.newBuilder()
						.setAction(ActionType.LEAVE)
						.setChannel(currCh)
						.setNickname(spamBot.getNickname())
						.build()));
				spamBot.spamuj(new Message(null,
						ChatAction.newBuilder()
						.setAction(ActionType.JOIN)
						.setChannel(cmd.substring("/join ".length()))
						.setNickname(spamBot.getNickname())
						.build()));
			} else if(cmd.startsWith("/quit ")) {
				
				spamBot.spamuj(new Message(null,
						ChatAction.newBuilder()
						.setAction(ActionType.LEAVE)
						.setChannel(currCh)
						.setNickname(spamBot.getNickname())
						.build()));
				spamBot.stop();
			} else if(cmd.equals("/users")) {
				textArea.insert("==> users on #" + currCh + ":" + spamBot.getUsersForCurrentCh() + "\n", 0);
			} else if(cmd.equals("/channels")) {
				textArea.insert("==> users on #" + currCh + ":" + spamBot.getChannels() + "\n", 0);
			} else {
				String[] msg = cmd.split(" ",2);
				if(msg.length < 2)
					return;
				
				spamBot.spamuj(msg[0], new Message(null,ChatMessage.newBuilder().setMessage(msg[1]).build()));
			}
        
		}
        textField.selectAll();
        textArea.setCaretPosition(0);
	}

	public String getCurrCh() {
		return currCh;
	}
}
