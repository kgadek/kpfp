import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.AbstractList;
import java.util.Iterator;
import java.util.NoSuchElementException;


public class DiskList<T extends Serializable> extends AbstractList<T> {
	public class FileIterator implements Iterator<T> {
		private int i = 0;

		public boolean hasNext() {
			return (i<rozmiar)?true:false;
		}

		public T next() {
			try {
				T temp = get(i);
				i++;
				return temp;
			} catch(IndexOutOfBoundsException e) {
				throw new NoSuchElementException();
			}
		}

		public void remove() {}

	}

	public DiskList() throws IOException {
		folder = new File("." + File.separator + "ListaNr" + id++);
		folder.mkdir();
		rozmiarPlik = new File(folder, "rozmiar");
		rozmiarPlik.createNewFile();
		DataOutputStream out = new DataOutputStream( new FileOutputStream( rozmiarPlik ) );
		out.writeInt(rozmiar);
		out.close();
	}
	
	private File rozmiarPlik;
	private int rozmiar = 0;
	private File folder;
	private static int id = 0;
	
	public static void main(String[] args) {
		try {
			DiskList<Integer> L = new DiskList<Integer>();
			for(int i=0;i<10;++i)
				L.add(i);
			L.add(0,333);
			L.add(3,666);
			L.add(11,777);
			
			Iterator<Integer> it = L.iterator();
			
			while(it.hasNext())
				System.out.println(it.next());
			DiskList<Integer> L2 = new DiskList<Integer>();
			L2.add(66666);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

	public T get(int index) {
		try {
			if(index<0 || index>=rozmiar)
				throw new IndexOutOfBoundsException();
			ObjectInputStream in = new ObjectInputStream(new FileInputStream(new File(folder,""+index)));
			@SuppressWarnings("unchecked")
			T obiekt = (T)in.readObject();
			in.close();
			return obiekt;
		} catch(ClassNotFoundException e) {
			e.printStackTrace();
			return null;
		} catch(IOException e) {
			e.printStackTrace();
			return null;
		}
	}

	public int size() {
		return rozmiar;
	}

	public boolean add(T el) {
		try {
			File temp = new File(folder, "" + rozmiar);
			temp.createNewFile();
			ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(temp));
			out.writeObject(el);
			out.close();
			aktualizujRozmiar();
			return true;
		} catch(IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	private void aktualizujRozmiar() throws IOException {
		rozmiar++;
		DataOutputStream out = new DataOutputStream(new FileOutputStream(rozmiarPlik));
		out.writeInt(rozmiar);
		out.close();
	}
	
	public void add(int index, T el) {
		if(index<0 || index>rozmiar)
			throw new IndexOutOfBoundsException();
		for(int i=rozmiar-1; i>=index; --i) {
			File f = new File(folder, ""+i);
			f.renameTo(new File(folder,""+(i+1)));
		}
		try {
			File temp = new File(folder, ""+index);
			temp.createNewFile();
			ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(temp));
			out.writeObject(el);
			out.close();
			aktualizujRozmiar();
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	public Iterator<T> iterator() {
		return new FileIterator();
	}
	
	
}
