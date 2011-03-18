import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.AbstractMap;
import java.util.Set;


public class CSVMap<T> extends AbstractMap<String, T> {	

	public CSVMap(Class<T> classType) {
		try {
			this.classType = classType;
			id = gid++;
			nazwaPliku = "."+File.separator + "Baza" + id;
			File plik = new File(nazwaPliku);
			plik.createNewFile();
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	public Set<java.util.Map.Entry<String, T>> entrySet() {
		return null;
	}
	
	public int size() {
		return rozmiar;
	}
	
	public T get(String key) {
		try {
			File plik = new File(nazwaPliku);
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(plik)));
			String tmp;
			String fd[] = null;
			while((tmp=in.readLine())!=null) {
				fd = tmp.split(",");
				if(fd.length!=2)
					throw new IOException("Nieobsługiwany format pliku!");
				if(fd[0].equals(key))
					break;
			}
			in.close();
			if(fd!=null && fd[0].equals(key)) {
				Constructor<T> cnstrctr = classType.getConstructor(String.class);
				return cnstrctr.newInstance(fd[1].trim());
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public T put(String key, T value) {
		try {
			File plik = new File(nazwaPliku);
			File newFile = new File("."+File.separator+"tmpBaza"+id);
			newFile.createNewFile();
			BufferedWriter out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(newFile)));
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(plik)));
			String tmp;
			while((tmp=in.readLine())!=null) {
				out.write(tmp);
				out.newLine();
			}
			in.close();
			out.write(key + "," + value);
			out.newLine();
			out.close();
			rozmiar++;
			plik.delete();
			newFile.renameTo(new File(nazwaPliku));
			plik = newFile;
			return value;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	
	
	String nazwaPliku;
	private int rozmiar;
	private static int gid = 0;
	private int id;
	Class<T> classType;
	
	
	public static void main(String[] args) {
		CSVMap<Integer> c1 = new CSVMap<Integer>(Integer.class);
		c1.put("jeden",1);
		c1.put("dwa", 2);
		c1.put("pięćdziesiąt pięć tysięcy",55000);
		System.out.println("Rozmiar c1: "+c1.size());
		System.out.println(c1.get("jeden"));
		CSVMap<Integer> c2 = new CSVMap<Integer>(Integer.class);
	}

}
