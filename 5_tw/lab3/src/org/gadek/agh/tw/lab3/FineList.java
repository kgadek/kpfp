package org.gadek.agh.tw.lab3;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class FineList implements MyList {
	
	static class Node {
		private Object x;
		private Node n;
		private Lock l;

		public Node getN() {
			return n;
		}

		public void setN(Node n) {
			this.n = n;
		}

		public Node(Object x) {
			assert(x != null);
			this.x = x;
			this.l = new ReentrantLock();
			this.n = null;
		}

		public Object getX() {
			return x;
		}
		
		public void lock() {
			this.l.lock();
		}
		
		public void unlock() {
			this.l.unlock();
		}
		
	}
	
	private Node head;
	private Lock headLock;

	public FineList() {
		head = null;
		headLock = new ReentrantLock();
	}

	@Override
	public boolean contains(Object o) {
		assert(o != null);
		headLock.lock();
		Node curr = head;
		while(curr != null) {
			if(o.equals(curr.getX())) {
				return true;
			}
		}
		return false;
	}

	@Override
	public boolean remove(Object o) {
		assert(o != null);
		headLock.lock();
		Node curr = head;
		curr.lock();
		if(o.equals(curr.getX())) {
			head = curr.getN();
			headLock.unlock();
			curr.unlock();
			return true;
		}
		headLock.unlock();
		Node next = null;
		while(true) {
			next = curr.getN();
			if(next == null) {
				curr.unlock();
				return false;
			}
			next.lock();
			if(o.equals(next.getX())) {
				curr.setN(next.getN());
				next.setN(null);
				curr.unlock();
				next.unlock();
				return true;
			}
			curr.unlock();
			curr = next;
		}
	}

	@Override
	public boolean add(Object o) {
		Node nnode = new Node(o);
		headLock.lock();
		if(head == null) {
			head = nnode;
			headLock.unlock();
			return true;
		}
		Node curr = head;
		Node next = null;
		curr.lock();
		headLock.unlock();
		while(true) {
			if(curr.getN() == null) {
				curr.setN(nnode);
				curr.unlock();
				return true;
			} 
			next = curr.getN();
			next.lock();
			curr.unlock();
			curr = next;
		}
	}

}
