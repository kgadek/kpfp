/*
 *      FileEncoding.java
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

import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.io.File;

public class FileEncoding {
	
	public static void main (String args[]) {
		if(args.length < 1) {
			System.out.println("skladnia: java FileEncoding <nazwa_pliku>");
			return;
		}
		
		for (int i = 0; i < args.length; i++) {
			try {
				File in = new File(args[i]);
				InputStreamReader r = new InputStreamReader(new FileInputStream(in));
				System.out.println(args[i] + " -> " + r.getEncoding());
			} catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
}

