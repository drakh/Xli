
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import android.os.AsyncTask;
import android.util.Log;


public class StreamHelper {
   public static String InputStreamToString(InputStream stream) throws IOException, UnsupportedEncodingException
    {
		@SuppressWarnings("resource")
		java.util.Scanner s = new java.util.Scanner(stream).useDelimiter("\\A");
        return s.hasNext() ? s.next() : "";
    }

    @SuppressWarnings("rawtypes")
	public static AsyncTask AsyncInputStreamToString(InputStream stream, long requestPointer) throws IOException, UnsupportedEncodingException
    {
        AsyncTask<Object, Void, String> a = new ASyncInputStreamToStringTask();
        a.execute(stream, requestPointer);
        return a;
    }

    @SuppressWarnings("rawtypes")
	public static AsyncTask AsyncInputStreamToByteArray(InputStream stream, long requestPointer)
    {
        AsyncTask<Object, Void, byte[]> a = new ASyncInputStreamToBytesTask();
        a.execute(stream, requestPointer);
        return a;
    }

    public static byte[] ReadAllBytesFromInputStream(InputStream stream) throws IOException
    {    	
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		byte[] data = new byte[16384];
		while ((nRead = stream.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		}
		buffer.flush();
		return buffer.toByteArray();
    }


    public static byte[] ReadBytesFromInputStream(BufferedInputStream stream, int bytesToRead)
    {    	
    	byte[] buffer = new byte[bytesToRead];
    	try {
			int bytesRead = stream.read(buffer);
			if (bytesRead>-1)
			{
				return Arrays.copyOf(buffer, bytesRead); //{TODO} only copy in c++?
			} else {
				return null;
			}
		} catch (IOException e) {
			Log.e("XliApp", "read from stream crashed: "+e.getLocalizedMessage());
				return null;	
		}	
    }
}
