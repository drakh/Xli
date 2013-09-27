package com.xli.glwindow;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

public class GLWindow extends android.app.NativeActivity
{
    static 
    {
        System.loadLibrary("GLWindow");
    }
	
	public void move_it(BufferedInputStream in, BufferedOutputStream out, byte[] buf, int buf_size) throws IOException {
		int len;
		while((len = in.read(buf, 0, buf_size)) > 0) {
			out.write(buf, 0, len);
	    }	
		out.close();
		in.close();
	}
}
