package src;

import java.io.File;
import java.io.BufferedInputStream;
import java.io.InputStream;
import java.io.FileInputStream;

import java.io.FileNotFoundException;
import java.io.IOException;

import java.nio.ByteBuffer;

import java.lang.Exception;

public class StlInterpretor {

	private String StLName = "";
	private float vector[][], vertex1[][], vertex2[][], vertex3[][];
	private byte data[];
	private boolean readdata = false;
	private long numfaces; //to do maybe change to big number? since
	//java dosen't do unsigned variables
	/**
	 * @param args
	 */
	public static void main(String[] args) {

	}

	/**
	 * Reads in a binary file and returns a byte array.
	 * @param Filename name of the file
	 * @return a reference to the data array
	 * */
	public byte[] read(String FileName){
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
	 * takes your binary array and interprets your array into useful information
	 * @return boolean representing whether function was successful
	 */
	public boolean interpretData(){
		int i,j,k,depth;
		ByteBuffer buf;
		if(!readdata){
			System.out.println("data not read from Stl file");
			return false;
		}
		for(i = 0; i < 80; i++){
			StLName += ((char)data[i]);
		}
		for(j = 0; j < 4; i++, j++){
			int offset = 3 - j;
			numfaces += data[i] & 0xFF << offset; 
		}
		for(j = 0; j < numfaces; j++){
			for(k = 0; k < 4; k++){
				for(depth = 0; depth < 3; depth++, i += 4){
					buf = ByteBuffer.wrap(data, i, 4);
					switch(k){
						case 0:
							vector[k][depth] = buf.getFloat();
							break;
						case 1:
							vertex1[k][depth] = buf.getFloat();
							break;
						case 2:
							vertex2[k][depth] = buf.getFloat();
							break;
						default:
							vertex3[k][depth] = buf.getFloat();
							break;
					}
				}
			}
			i += 2; //to skip the attribute byte that occurs after each face
		}
		return true;
	}

	/**
	 * @return string representing the name of file. Blank if you did not read anything
	 */
	public String getName(){
		return StLName;
	}
	
	public long getNumfaces(){
		return readdata?numfaces:0;
	}
	
	/**
	 * @return two dimensional array of floats with the normal vectors of each triangle
	 * @throws Exception if no file is read
	 */
	public float[][] getVector() throws Exception{
		if (!readdata){
			throw new Exception("file not read");
		}else{
			return vector;
		}
	}

	/**
	 * @return two dimensional array of floats with vertex1 of each triangle
	 * @throws Exception if no file is read
	 */
	public float[][] getVertex1() throws Exception{
		if(!readdata){
			throw new Exception("File not read");
		}else{
			return vertex1;
		}
	}
	
	/**
	 * @return two dimensional array of floats with vertex2 of each triangle
	 * @throws Exception if no file is read
	 */
	public float[][] getVertex2() throws Exception{
		if(!readdata){
			throw new Exception("File not read");
		}else{
			return vertex2;
		}
	}

	/**
	 * @return two dimensional array of floats with vertex3 of each triangle
	 * @throws Exception if no file is read
	 */
	public float[][] getVertex3() throws Exception{
		if(!readdata){
			throw new Exception("File not read");
		}else{
			return vertex3;
		}
	}
}
