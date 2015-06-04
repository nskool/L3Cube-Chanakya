
import java.io.*;
import java.util.*;

public class svc{
static	void copy(String filename) throws IOException
	{
    File input = new File(filename);
    File output = new File("output");
    RandomAccessFile i = new RandomAccessFile(input, "r");
    RandomAccessFile o = new RandomAccessFile(output, "rw");

   
        long x =o.length();
        System.out.println(x);
        i.seek(0);
        o.seek(x);
        int in;
        do
        {
        	in = i.read();
        	System.out.println(in);
        	
        	o.write(in);
        	
        	
        }while(in != -1);
        o.write(60);

	}

public static void main(String args[]) throws IOException {        
    try {
        if(args.length == 0){
        	System.out.println("Proper way of executing program \n1)To commit: java svc input_file\n2)To display version:java svc version_no ");
        	return;
        }
    	
    	int arg_1 = Integer.parseInt(args[0]);
        File output = new File("output");
        if(!output.exists()) {
        	output.createNewFile();
        } 
        RandomAccessFile o = new RandomAccessFile(output, "r");

        int in;
        int count = 0;
        do
        {
        	in = o.read();
        if(in == 60)
        	{
        	    
        		count++;  
        		continue;
        	//	System.out.println(count);
          	
        	}
    
		if(in==255)
		{
			continue;
		}

 
        if(count == arg_1)
      	{
      		char d = (char)in;
      		System.out.print(d);

      	}
        /*
			   if(in == 10)
				{
					System.out.println();
					continue;
				}
        	*/
        }while(in != -1);
        
       
    }
    catch (NumberFormatException e) {
		// TODO Auto-generated catch block
    	String arg_string = args[0].toString();
    	copy(arg_string);
		
	}
    
}
}
