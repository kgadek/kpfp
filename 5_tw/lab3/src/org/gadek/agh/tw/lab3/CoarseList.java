package org.gadek.agh.tw.lab3;

public class CoarseList implements MyList {

	static class Node {
		private Object x;
		private Node n;
		public Node(Object x) {
			super();
			assert(x != null);
			this.x = x;
			this.n = null;
		}
		public Node getN() {
			return n;
		}
		public void setN(Node n) {
			this.n = n;
		}
		public Object getX() {
			return x;
		}
	}
	
	private Node head;
	
	public CoarseList() {
		super();
		head = new Node(this);
	}

	@Override
	public synchronized boolean contains(Object o) {
		assert(o != null);
		Node curr = head;
		Node next = head.getN();
		while(next != null) {
			if(o.equals(next.getX()))
				return true;
			curr = next;
			next = curr.getN();
		}
		return false;
	}

	@Override
	public synchronized boolean remove(Object o) {
		assert(o != null);
		Node curr = head;
		Node next = head.getN();
		while(next != null) {
			if(o.equals(next.getX())) {
				curr.setN(next.getN());
				next.setN(null);
				return true;
			}
			curr = next;
			next = curr.getN();
		}
		return false;
	}

	@Override
	public synchronized boolean add(Object o) {
		Node nnode = new Node(o);
		Node curr = head;
		Node next = head.getN();
		while(next != null) {
			curr = next;
			next = curr.getN();
		}
		curr.setN(nnode);
		return false;
	}

}
