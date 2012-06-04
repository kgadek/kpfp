package org.gadek.agh.sr;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashSet;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.jgroups.Address;
import org.jgroups.JChannel;
import org.jgroups.Message;
import org.jgroups.Receiver;
import org.jgroups.View;
import org.jgroups.protocols.BARRIER;
import org.jgroups.protocols.FD_ALL;
import org.jgroups.protocols.FD_SOCK;
import org.jgroups.protocols.FRAG2;
import org.jgroups.protocols.MERGE2;
import org.jgroups.protocols.MFC;
import org.jgroups.protocols.PING;
import org.jgroups.protocols.UDP;
import org.jgroups.protocols.UFC;
import org.jgroups.protocols.UNICAST2;
import org.jgroups.protocols.VERIFY_SUSPECT;
import org.jgroups.protocols.pbcast.FLUSH;
import org.jgroups.protocols.pbcast.GMS;
import org.jgroups.protocols.pbcast.NAKACK;
import org.jgroups.protocols.pbcast.STABLE;
import org.jgroups.protocols.pbcast.STATE_TRANSFER;
import org.jgroups.stack.ProtocolStack;
import org.jgroups.util.Util;

import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatAction;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatMessage;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatAction.ActionType;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatState;
import pl.edu.agh.dsrg.sr.chat.protos.ChatOperationProtos.ChatState.Builder;

public class SpamBot implements Receiver {
	public static final String DEFAULT_CHANNEL = "ChatManagement768264";

	private String nickname;
	private Map<String, HashSet<String>> addressBook = new ConcurrentHashMap<String, HashSet<String>>();
	JChannel channel = null;
	ProtocolStack stack = null;
	private AintNoGui gui = null;
	
	public static void main(String[] args) {
		(new SpamBot("kgadek", null)).start();
	}

	public SpamBot(String nickname, AintNoGui gui) {
		super();
		this.nickname = nickname;
		this.gui = gui;
	}

	public String getNickname() {
		return nickname;
	}

	public void setNickname(String nickname) {
		this.nickname = nickname;
	}

	public SpamBot start() {
		try {
			channel = new JChannel(false);
			
			stack = new ProtocolStack();
			channel.setProtocolStack(stack);
			stack.addProtocol(new UDP())
			.addProtocol(new PING())
			.addProtocol(new MERGE2())
			.addProtocol(new FD_SOCK())
			.addProtocol(new FD_ALL().setValue("timeout", 12000).setValue("interval", 3000))
			.addProtocol(new VERIFY_SUSPECT())
			.addProtocol(new BARRIER())
			.addProtocol(new NAKACK())
			.addProtocol(new UNICAST2())
			.addProtocol(new STABLE())
			.addProtocol(new GMS())
			.addProtocol(new UFC())
			.addProtocol(new MFC())
			.addProtocol(new FRAG2())
			.addProtocol(new STATE_TRANSFER())
			.addProtocol(new FLUSH());
			stack.init();
			
			channel.setReceiver(this);
			channel.setName(nickname);
			channel.connect(DEFAULT_CHANNEL);
			System.out.println("CONNECT");
			channel.getState(null, 15000); // 15 sec is long enough... is it?
		} catch (Exception e) {
			e.printStackTrace();
			if(channel != null)
				channel.close();
		}
		return this;
	}

	public SpamBot stop() {
		if(channel != null)
			channel.close();
		return this;
	}
	
	@Override
	public void getState(OutputStream out) throws Exception {
		Builder builder = ChatState.newBuilder();
		for(Entry<String, HashSet<String>> i : addressBook.entrySet())
			for(String p : i.getValue())
				builder = builder.addState(ChatAction.newBuilder()
											.setAction(ActionType.JOIN)
											.setChannel(i.getKey())
											.setNickname(p)
											.build());
		Util.objectToStream(builder.build(), new DataOutputStream(out));
	}
	@Override
	public void receive(Message msg) {
		if (msg.getObject() instanceof ChatAction) {
			ChatAction actn = (ChatAction) msg.getObject();
			String channelName = null, newcomer = null;
			Set<String> currChnnl = null;
			switch(actn.getAction()) {
			case JOIN:
				channelName = actn.getChannel();
				if(!addressBook.containsKey(channelName))
					addressBook.put(channelName, new HashSet<String>());
				currChnnl = addressBook.get(channelName);
				newcomer = actn.getNickname();
				if(!currChnnl.contains(newcomer)) {
					System.out.format("\n\n==> %s joined #%s\n\n", newcomer, channelName);
					
					gui.textArea.insert("==> "+newcomer+" joined #"+channelName+"\n", 0);
					gui.textField.selectAll();
					gui.textArea.setCaretPosition(0);
					currChnnl.add(newcomer);
				}
				break;
			case LEAVE:
				channelName = actn.getChannel();
				if(!addressBook.containsKey(channelName))
					addressBook.put(channelName, new HashSet<String>());
				currChnnl = addressBook.get(channelName);
				newcomer = actn.getNickname();
				if(currChnnl.contains(newcomer)) {
					System.out.format("\n\n==> %s left #%s\n\n", newcomer, channelName);
					gui.textArea.insert("==> "+newcomer+" left #"+channelName+"\n", 0);
					gui.textField.selectAll();
					gui.textArea.setCaretPosition(0);
					currChnnl.remove(newcomer);
				}
			}
		} else if(msg.getObject() instanceof ChatMessage) {
			ChatMessage chtMsg = (ChatMessage) msg.getObject();
			gui.textArea.insert(chtMsg.getMessage()+"\n", 0);
			gui.textField.selectAll();
			gui.textArea.setCaretPosition(0);
		}
		
	}
	
	@Override
	public void setState(InputStream in) throws Exception {
		ChatState chat = (ChatState) Util.objectFromStream(new DataInputStream(in));
		addressBook.clear();
		for(ChatAction action : chat.getStateList()) {
			String channelName = action.getChannel();
			String userNick = action.getNickname();
			if(addressBook.containsKey(channelName))
				addressBook.put(nickname, new HashSet<String>());
			addressBook.get(userNick).add(userNick);
		}
	}
	@Override
	public void block() {
	}
	@Override
	public void suspect(Address arg0) {
	}
	@Override
	public void unblock() {
	}
	@Override
	public void viewAccepted(View arg0) {
	}
	
	public void spamuj(Message msg) {
		try {
			channel.send(msg);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public String getUsersForCurrentCh() {
		String currCh = gui.getCurrCh();
		if(!addressBook.containsKey(currCh))
			return "< no such channel exist? >";
		StringBuilder sb = new StringBuilder();
		for(String user : addressBook.get(currCh))
			sb.append("\n  > ").append(user);
		return sb.toString();
	}
	
	public String getChannels() {
		StringBuilder sb = new StringBuilder();
		for(String i : addressBook.keySet())
			sb.append("\n  > ").append(i);
		return sb.toString();
	}
	
	public boolean isClosed() {
		return channel == null || !channel.isOpen();
	}
}
