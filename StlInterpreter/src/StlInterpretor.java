import java.io.File;
import java.io.BufferedInputStream;
import java.io.InputStream;
import java.io.FileInputStream;

import java.io.FileNotFoundException;
import java.io.IOException;

public class StlInterpretor {

	private String StLName = "";
	private float vector[][], vertex1[][], vertex2[][], vertex3[][];
	private byte data[];
	private boolean readdata = false;
	private long numfaces;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
	}

	/** 
	 * 	Reads in a binary file and returns a byte array.
	 *  @param Filename name of the file
	 * */ 
	byte[] read(String FileName){
		int read = 0, temp = 0;
		System.out.println("Reading from " + FileName);
		File file = new File(FileName);
		System.out.println("File size: " + file.length());
		data = new byte[(int)file.length()];
		try{
			InputStream reader = new BufferedInputStream(new FileInputStream(file));
			try{
				while(read < data.length){
					temp = reader.read(data,read,data.length - read);
					read += temp;
				}
			}finally{
				reader.close();
				System.out.println("num bytes read " + read);
			}
		}catch(FileNotFoundException ex){
			System.out.println("can't find file");
		}catch(IOException ex){
			System.out.println("can't read/close file");
		}
		readdata = true;
		return data;
	}
	
	/**
	 * 
	 */
	boolean interpretdata(){
		int i,j;
		if(!readdata){
			System.out.println("data not read from Stl file");
			return false;
		}
		for(i = 0; i < 80; i++){
			StLName += ((char)data[i]);
		}
		for(j = 0; j < 4; i++, j++){
			
		}
		return true;
	}
}