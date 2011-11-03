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

	public FineList() {
		head = new Node(this);
	}

	@Override
	public boolean contains(Object o) {
		assert(o != null);
		Node curr = head;
		curr.lock();
		Node next = head.getN();
		while(next != null) {
			next.lock();
			if(o.equals(next.getX()))
				break;
			curr.unlock();
			curr = next;
			next = curr.getN();
		}
		curr.unlock();
		if(next == null)
			return false;
		next.unlock();
		return true;
	}

	@Override
	public boolean remove(Object o) {
		assert(o != null);
		Node curr = head;
		curr.lock();
		Node next = head.getN();
		while(next != null) {
			next.lock();
			if(o.equals(next.getX()))
				break;
			curr.unlock();
			curr = next;
			next = curr.getN();
		}
		if(next == null) {
			curr.unlock();
			return false;
		}
		curr.setN(next.getN());
		next.setN(null);
		curr.unlock();
		next.unlock();
		return true;
	}

	@Override
	public boolean add(Object o) {
		Node nnode = new Node(o);
		Node curr = head;
		curr.lock();
		Node next = head.getN();
		while(next != null) {
			next.lock();
			curr.unlock();
			curr = next;
			next = curr.getN();
		}
		curr.setN(nnode);
		curr.unlock();
		return true;
	}

}
